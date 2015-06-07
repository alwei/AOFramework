cbuffer CsBuffer : register(b0)
{
	float4 offsetPos;
	float deltaTime;
	float3 dummy;
}


//読み取り用の構造化バッファー
//アンオーダードアクセスバッファとして作成されたバッファ
RWByteAddressBuffer initBuffer		: register(u0);
RWByteAddressBuffer updateBuffer	: register(u1);
RWByteAddressBuffer vectorField		: register(u2);


#define	numThreadX (1000)
#define numThreadY (1)
#define	numThreadZ (1)


[numthreads(numThreadX, numThreadY, numThreadZ)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//インデックス算出
	const uint index = DTid.z * numThreadX * numThreadY + DTid.y * numThreadZ + DTid.x;
	const uint vbInitIndex = index * INIT_STRIDE;
	const uint vbUpdateIndex = index * UPDATE_STRIDE;

	if (index >= MAX_BUFFER)
		return;

	float3	initPos = asfloat(initBuffer.Load3(vbInitIndex + 0));
	float4	initColor = asfloat(initBuffer.Load4(vbInitIndex + 12));
	float3	initSpeed = asfloat(initBuffer.Load3(vbInitIndex + 28));
	float	maxLifeTime = asfloat(initBuffer.Load(vbInitIndex + 40));

	float3	pos = asfloat(updateBuffer.Load3(vbUpdateIndex + 0));
	float4	color = asfloat(updateBuffer.Load4(vbUpdateIndex + 12));
	float3	speed = asfloat(updateBuffer.Load3(vbUpdateIndex + 28));
	float	nowLifeTime = asfloat(updateBuffer.Load(vbUpdateIndex + 40));

	if (nowLifeTime <= 0 )
	{
		//初期化
		pos = offsetPos.xyz + initPos;
		color = initColor;
		speed = initSpeed;
		nowLifeTime = deltaTime;
	}
	else
	{
		//処理中
		nowLifeTime = min(nowLifeTime+deltaTime,maxLifeTime);
		float t = 1-nowLifeTime / maxLifeTime;
		color.a = initColor.a * t;

		//calc vector field index
		uint vecIndex = (uint)abs(pos.x) + (uint)abs(pos.y) * X_VEC + (uint)abs(pos.z) * Y_VEC * X_VEC;
		if (0 <= vecIndex && vecIndex < X_VEC * Y_VEC * Z_VEC)
		{
			float3 vSpeed = asfloat(vectorField.Load3(vecIndex));
			speed.x += vSpeed.x;
			speed.y += vSpeed.y;
			speed.z += vSpeed.z;
		}

		pos.x += speed.x;
		pos.y += speed.y;
		pos.z += speed.z;

		if (nowLifeTime >= maxLifeTime)
			nowLifeTime = 0;
	}


	const uint vbOutIndex = vbUpdateIndex;
	updateBuffer.Store3(vbOutIndex + 0, asuint(pos));
	updateBuffer.Store4(vbOutIndex + 12, asuint(color));
	updateBuffer.Store3(vbOutIndex + 28, asuint(speed));
	updateBuffer.Store(vbOutIndex + 40, asuint(nowLifeTime));
}
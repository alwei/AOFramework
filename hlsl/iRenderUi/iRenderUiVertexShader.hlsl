#include"../Subroutine/GeneralMathSubroutine.hlsl"

struct VsConst0
{
	float4 degree;
};

struct VsConst1
{
	float4 uv[2];
};

struct VsConst2
{
	float2 pos;
	float2 size;
	float4 color;
};


cbuffer ConstBuffer0 : register(b0)
{
	VsConst0 vsInst0[NUM_DEGREE];
};

cbuffer ConstBuffer1 : register(b1)
{
	VsConst1 vsInst1[NUM_INST];
};

cbuffer ConstBuffer2 : register(b2)
{
	VsConst2 vsInst2[NUM_INST];
};

cbuffer ConstBuffer3 : register(b3)
{
	float4x4 matOrtho;
};


//頂点データ
static const float3 vertexData[4] =
{
	{ -0.5f, +0.5f, 0 },
	{ +0.5f, +0.5f, 0 },
	{ -0.5f, -0.5f, 0 },
	{ +0.5f, -0.5f, 0 }
};


struct VSOutput
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: TEXCOORD1;
};


VSOutput main(uint _id : POSITION)
{
	VSOutput Out = (VSOutput)0;

	int index = _id / 4;
	float degree = vsInst0[index].degree[_id % 4];

	//スケールとオフセットの行列作成
	float4x4 osMtx = CreateScaleOffsetMatrix(
		vsInst2[index].pos.x, vsInst2[index].pos.y, 0,
		vsInst2[index].size.x, vsInst2[index].size.y, 0);

	//回転行列作成
	float4x4 rotMtx = CreateZRotationMatrix(degree);

	//全行列を合成
	float4x4 mtx = mul(mul(rotMtx, osMtx), matOrtho);

	//頂点変換
	Out.pos = mul(float4(vertexData[_id % 4], 1), mtx);

	//色格納
	Out.color = vsInst2[index].color;

	//一度UVを展開
	float2 uv[4] = 
	{
		vsInst1[index].uv[0].xy,
		vsInst1[index].uv[0].zw,
		vsInst1[index].uv[1].xy,
		vsInst1[index].uv[1].zw
	};

	//UV格納
	Out.uv = uv[_id%4];

	return Out;
}
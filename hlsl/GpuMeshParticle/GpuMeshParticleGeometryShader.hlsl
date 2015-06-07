cbuffer MatrixBuffer : register(b0)
{
	float4x4 matProj;
	float4x4 matViewProj;
};


struct GSInput
{
	float4 pos			: SV_POSITION;
	float4 col			: TEXCOORD0;
	float nowLifeTime	: TEXCOORD1;
	float size			: TEXCOORD2;
};


struct GSOutput
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


[maxvertexcount(4)]
void main(point GSInput In[1], inout TriangleStream< GSOutput > output)
{
	if (In[0].nowLifeTime <= 0)
		return;
		
	GSOutput Out = (GSOutput)0;

	float4 pos = mul(In[0].pos, matViewProj);

	//Ž‹‘äƒJƒŠƒ“ƒO
	float x = pos.x / pos.w;
	float y = pos.y / pos.w;
	float z = pos.z / pos.w;
	if (!(-1.0f < x && y < 1.0f && -1.0f < y && y < 1.0f && 0 < z && z < 1.0f))
		return;

	float2 s = In[0].size * (pos.z / pos.w);
	float4x4 projMtx = matProj;
	projMtx[3].xyzw = pos;

	Out.pos = mul(float4(-s.x, s.y, 0, 1), projMtx);
	Out.uv = float2(0, 0);
	Out.col = In[0].col;
	output.Append(Out);

	Out.pos = mul(float4(s.x, s.y, 0, 1), projMtx);
	Out.uv = float2(1, 0);
	Out.col = In[0].col;
	output.Append(Out);

	Out.pos = mul(float4(-s.x, -s.y, 0, 1), projMtx);
	Out.uv = float2(0, 1);
	Out.col = In[0].col;
	output.Append(Out);

	Out.pos = mul(float4(s.x, -s.y, 0, 1), projMtx);
	Out.uv = float2(1, 1);
	Out.col = In[0].col;
	output.Append(Out);
}
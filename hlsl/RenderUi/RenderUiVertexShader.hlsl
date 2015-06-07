
cbuffer cbWP : register(b0)
{
	float4x4	g_matWO;
}

struct VSInput
{
	float3 pos	: POSITION;
	float3 n	: NORMAL;
	float2 uv	: UV;
};

struct VSOutput
{
	float4 pos	: SV_POSITION;
	float3 n	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


VSOutput main(VSInput In)
{
	VSOutput Out;

	Out.pos = mul(float4(In.pos, 1), g_matWO);
	Out.n = In.n;
	Out.uv = In.uv;

	return Out;
}
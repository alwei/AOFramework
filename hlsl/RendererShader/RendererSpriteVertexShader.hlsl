

cbuffer GpuBuffer0 : register(b0)
{
	float4x4 matWP;
	float4 color;
}


struct OutputVS
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


static const float3 VB[4] =
{
	{ 0.f, 0.f, 0.f },
	{ 1.f, 0.f, 0.f },
	{ 0.f, -1.f, 0.f },
	{ 1.f, -1.f, 0.f }
};


static const float2 UV[4] =
{
	{ 0.f, 0.f },
	{ 1.f, 0.f },
	{ 0.f, 1.f },
	{ 1.f, 1.f }
};


OutputVS main(uint id : POSITION)
{
	OutputVS Out = (OutputVS)0;

	Out.pos = mul(float4(VB[id], 1), matWP);
	Out.uv = UV[id];
	Out.col = color;

	return Out;
}
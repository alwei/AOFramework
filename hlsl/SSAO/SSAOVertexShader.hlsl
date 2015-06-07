cbuffer MatrixBuffer : register(b0)
{
	float4x4	g_matWVP;
}


struct VSInput
{
	float3 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD;
};


struct VSOutput
{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD0;
	float4 sPos	: TEXCOORD1;
};


VSOutput main( VSInput In )
{
	VSOutput Out = (VSOutput)0;

	Out.pos = mul(float4(In.pos,1),g_matWVP);
	Out.n = In.n;
	Out.uv = In.uv;
	Out.sPos = Out.pos;

	return Out;
}
cbuffer cbColor : register(b0)
{
	float4 g_color;
};

Texture2D		tex		: register(t0);
SamplerState	samp	: register(s0);


struct PSInput
{
	float4 pos	: SV_POSITION;
	float3 n	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


float4 main(PSInput In) : SV_TARGET0
{
	return g_color * tex.Sample(samp, In.uv);
}
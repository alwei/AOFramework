
cbuffer extractBuffer : register (b0)
{
	float4 sub;
};


struct PSInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};


Texture2D		g_texture	: register(t0);
SamplerState	g_sampler	: register(s0);


float4 main(PSInput In) : SV_TARGET
{
	float4 extColor = g_texture.Sample(g_sampler, In.tex);
	
	extColor.r = max(0, (extColor.r - sub.r));
	extColor.g = max(0, (extColor.g - sub.g));
	extColor.b = max(0, (extColor.b - sub.b));
	extColor.a = max(0, (extColor.a - sub.a));
	
	return extColor;
}
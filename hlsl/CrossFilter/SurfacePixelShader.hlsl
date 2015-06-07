
struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
};


Texture2D		g_tex	: register(t0);
SamplerState	g_samp	: register(s0);


float4 main(PSInput In) : SV_TARGET
{
	return g_tex.Sample(g_samp, In.uv);
}
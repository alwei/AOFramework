
Texture2D tex0		: register(t0);
SamplerState samp0	: register(s0);


struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
	float4 col	: TEXCOORD1;
};

float4 main(PSInput In) : SV_TARGET
{
	return In.col * tex0.Sample(samp0, In.uv);
}
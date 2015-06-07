cbuffer PsBuffer : register(b0)
{
	float4 color;
}


struct PSInput
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


Texture2D		sTex0	: register(t0);
SamplerState	sSamp0	: register(s0);


float4 main(PSInput In) : SV_TARGET
{
	return In.col * sTex0.Sample(sSamp0, In.uv) * color;
}

struct InputPS
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
	float2 uv	: TEXCOORD1;
};


Texture2D		tex0	: register(t0);
SamplerState	samp0	: register(s0);


float4 main(InputPS In) : SV_TARGET
{
	return tex0.Sample(samp0, In.uv);// *In.col;
}

cbuffer MaterialBuffer : register(b0)
{
	float4 color;
};


//テクスチャ
Texture2D		sTex0 : register(t0);
SamplerState	samp0 : register(s0);


struct InputPS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD;
};


float4 main(InputPS In) : SV_TARGET
{
	return color * sTex0.Sample(samp0,In.uv);
}
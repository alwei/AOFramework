cbuffer MaterialsBufer : register(b0)
{
	float4 g_color;
};


struct PSInput
{
	float4	pos			: SV_POSITION;
	float3	normal		: NORMAL;
	float2	uv			: TEXCOORD0;
	float4	color		: TEXCOORD1;
};


//テクスチャ
Texture2D		sTex0 : register(t0);
SamplerState	samp0 : register(s0);


float4 main(PSInput In) : SV_TARGET
{
	return g_color * In.color * sTex0.Sample(samp0, In.uv);
}


//定数バッファ													  
cbuffer pixBuffer : register(b0)
{
	float4	g_weight0;
	float4	g_weight1;
	float4	g_weight2;
	float4	g_weight3;
	float4	g_weight4;
	float4	g_weight5;
	float4	g_weight6;
	float4	g_weight7;
};

//テクスチャ＋サンプラーステート									 
Texture2D		g_tex		: register(t0);
SamplerState	g_sampler	: register(s0);


struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv0	: TEXCOORD0;
	float2 uv1	: TEXCOORD1;
	float2 uv2	: TEXCOORD2;
	float2 uv3	: TEXCOORD3;
	float2 uv4	: TEXCOORD4;
	float2 uv5	: TEXCOORD5;
	float2 uv6	: TEXCOORD6;
	float2 uv7	: TEXCOORD7;
};


float4 main(PSInput In) : SV_TARGET
{
	float4 Out = 0;

	Out += (g_tex.Sample(g_sampler, In.uv0)) * g_weight0;
	Out += (g_tex.Sample(g_sampler, In.uv1)) * g_weight1;
	Out += (g_tex.Sample(g_sampler, In.uv2)) * g_weight2;
	Out += (g_tex.Sample(g_sampler, In.uv3)) * g_weight3;
	Out += (g_tex.Sample(g_sampler, In.uv4)) * g_weight4;
	Out += (g_tex.Sample(g_sampler, In.uv5)) * g_weight5;
	Out += (g_tex.Sample(g_sampler, In.uv6)) * g_weight6;
	Out += (g_tex.Sample(g_sampler, In.uv7)) * g_weight7;

	return Out;
}
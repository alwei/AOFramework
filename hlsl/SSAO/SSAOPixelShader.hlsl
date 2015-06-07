
cbuffer MatrixBuffer : register(b2)
{
	float4x4 g_matWV;
}


Texture2D		g_decalMap	: register(t0);
SamplerState	g_sampler	: register(s0);



struct PSInput
{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD0;
	float4 sPos	: TEXCOORD1;
};


struct PSOutput
{
	float4 color0 : SV_TARGET0;
};



PSOutput main(PSInput In)
{
	PSOutput Out = (PSOutput)0;

	//法線ベクトル＋深度マップを出力
	Out.color0.xy = normalize(mul(float4(In.n,0),g_matWV).xyz).xy;
	Out.color0.zw = In.sPos.zw;

	return Out;
}
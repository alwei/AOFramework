cbuffer MaterialsBufer : register(b0)
{
	float4 g_vDiffuse;
	float4 g_vSpecular;
	float4 g_vEmissive;
}


struct PSInput
{
	float4	pos			: SV_POSITION;
	float3	normal		: NORMAL;
	float2	uv			: TEXCOORD0;
	float4	color		: TEXCOORD1;
	float4	light		: TEXCOORD2;
	float4	eyeDir		: TEXCOORD3;
};

struct PSOutput
{
	float4 color0	: SV_TARGET0;
};


//テクスチャ
Texture2D		sTex0 : register(t0);
Texture2D		sTex1 : register(t1);
Texture2D		sTex2 : register(t2);
SamplerState	samp0 : register(s0);
SamplerState	samp1 : register(s1);
SamplerState	samp2 : register(s2);


PSOutput main(PSInput In)
{
	PSOutput Out;

	float3 N = normalize(In.normal);
	float3 L = -normalize(In.light.xyz);

	//色計算
	Out.color0 = (g_vDiffuse * sTex0.Sample(samp0, In.uv));
	Out.color0 *= In.color;

	//ハーフランバード計算
	float H = max(0,dot(N, L));
	H = H * 0.5f + In.light.w;
	H = H * H;
	Out.color0.rgb *= H;

	//スペキュラ+エミッシブ計算
	float3 E = normalize(In.eyeDir.xyz);
	float3 HV = max(0, normalize(E + L));
	float S = pow(max(0, dot(HV, N)), 2) * In.eyeDir.w;
	float3 EM = sTex2.Sample(samp2, In.uv).rgb*g_vEmissive.rgb;
	float3 SP = S*sTex1.Sample(samp1, In.uv).rgb*g_vSpecular.rgb;
	Out.color0.rgb += SP + EM;

	return Out;
}
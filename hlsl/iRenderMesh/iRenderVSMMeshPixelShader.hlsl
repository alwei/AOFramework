#include"../Subroutine/PixelShaderSubroutine.hlsl"


cbuffer PixelBuffer : register(b0)
{
	float4 g_vDiffuse;
	float4 g_vSpecular;
	float4 g_vEmissive;
}


//テクスチャ
Texture2D		sTex0 : register(t0);
Texture2D		sTex1 : register(t1);
Texture2D		sTex2 : register(t2);
SamplerState	samp0 : register(s0);
SamplerState	samp1 : register(s1);
SamplerState	samp2 : register(s2);

Texture2D		zMapTexture	: register(t5);
SamplerState	zMapSampler	: register(s5);


struct	PSInput
{
	float4	pos			: SV_POSITION;
	float3	normal		: NORMAL;
	float2	uv			: TEXCOORD0;
	float4  zMap		: TEXCOORD1;
	float4	color		: TEXCOORD2;
	float4	lightDir	: TEXCOORD3;
	float4	eyeDir		: TEXCOORD4;
};



float4 main(PSInput In) : SV_TARGET0
{
	//ハーフランバート計算
	float3 L = -In.lightDir.xyz;
	float3 N = normalize(In.normal);
	float HL = max(0, dot(N, L));
	HL = HL * 0.5f + In.lightDir.w;
	HL = HL * HL;

	//スペキュラ+エミッシブ計算
	float3 HV = max(0, normalize(In.eyeDir.xyz + L));
	float S = pow(max(0, dot(HV, N)), 2)*In.eyeDir.w;

	float4 sMap = sTex1.Sample(samp1, In.uv);
	float4 eMap = sTex2.Sample(samp2, In.uv);
	float3 SP = sMap.rgb * g_vSpecular.rgb * S;
	float3 EM = eMap.rgb * g_vEmissive.rgb;

	float4 Out = g_vDiffuse * sTex0.Sample(samp0, In.uv) * In.color;

	//射影空間のXY座標をテクスチャ座標に変換
	float2 transTexCoord;
	transTexCoord.x = (1.0f + In.zMap.x / In.zMap.w) * 0.5f;
	transTexCoord.y = (1.0f - In.zMap.y / In.zMap.w) * 0.5f;

	float2 rZ = zMapTexture.Sample(zMapSampler, transTexCoord).rg;

	Out.rgb += SP;
	Out.rgb *= VSM(transTexCoord, rZ, In.zMap.zw, HL);
	Out.rgb += EM;

	return Out;
}
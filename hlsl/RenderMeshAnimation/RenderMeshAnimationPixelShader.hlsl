
cbuffer MaterialBuffer : register(b0)
{
	float4 diffuse;
	float4 specular;
	float3 emissive;
}

cbuffer LightBuffer : register(b1)
{
	float4 lightDir;
	float4 eyePos;
	float4 color;
	int disableLighting;
}


//テクスチャ
Texture2D		sTex0 : register(t0);
Texture2D		sTex1 : register(t1);
Texture2D		sTex2 : register(t2);
SamplerState	samp0 : register(s0);
SamplerState	samp1 : register(s1);
SamplerState	samp2 : register(s2);


struct InputPS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD;
};


float4 main(InputPS In) : SV_TARGET
{
	if (disableLighting)
		return diffuse * sTex0.Sample(samp0, In.uv) * color;

	//ハーフランバート計算
	float HL = max(0,dot(In.n, -lightDir.xyz));
	HL = HL * 0.5f + lightDir.w;
	HL = HL * HL;

	//スペキュラ+エミッシブ計算
	float3 HV = max(0, normalize(eyePos.xyz + lightDir.xyz));
	float S = pow(max(0, dot(HV, In.n)), 2)*eyePos.w;
	float3 SP = sTex1.Sample(samp1, In.uv).rgb * specular.rgb * S;
	float3 EM = sTex2.Sample(samp2, In.uv).rgb * emissive.rgb;

	float4 Out = diffuse *sTex0.Sample(samp0, In.uv);
	Out.rgb *= HL;
	Out.rgb += SP + EM;

	Out *= color;

	return Out;
}
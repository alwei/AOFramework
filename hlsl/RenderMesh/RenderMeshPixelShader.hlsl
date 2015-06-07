
cbuffer MaterialBuffer : register(b0)
{
	float4 diffuse;
	float4 specular;
	float3 emissive;
}

cbuffer LightBuffer : register(b1)
{
	float4 lightDir;
	float4 dColor;
	float4 sColor;
	float4 eColor;
	int disableLighting;
	int specularRange;
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
	float2 uv	: TEXCOORD0;
	float4 eye	: TEXCOORD1;
};


float4 main(InputPS In) : SV_TARGET
{
	if (disableLighting)
	{
		float4 Out = float4(0, 0, 0, 0);
		float4 eMap = sTex2.Sample(samp2, In.uv);
		float3 EM = eMap.rgb * emissive.rgb;
		Out = diffuse * sTex0.Sample(samp0, In.uv) * dColor;
		Out.rgb += EM;
		Out.a += eMap.a * eColor.a;
		return Out;
	}

	//ハーフランバート計算
	float3 L = -lightDir.xyz;
	float3 N = normalize(In.n);
	float HL = max(0,dot(N, L));
	HL = HL * 0.5f + lightDir.w;
	HL = HL * HL;
	
	//スペキュラ+エミッシブ計算
	float3 HV = max(0, normalize(In.eye.xyz + L));
	float S = pow(max(0, dot(HV, N)), specularRange)*In.eye.w;

	float4 sMap = sTex1.Sample(samp1, In.uv);
	float4 eMap = sTex2.Sample(samp2, In.uv);
	float3 SP = sMap.rgb * specular.rgb * sColor.rgb * S;
	float3 EM = eMap.rgb * emissive.rgb * eColor.rgb;
	
	float4 Out = diffuse *sTex0.Sample(samp0, In.uv);
	Out.rgb *= HL;
	Out.rgb += SP + EM;
	
	Out *= dColor;
	Out.a += sMap.a * sColor.a;
	Out.a += eMap.a * eColor.a;

	return Out;
}
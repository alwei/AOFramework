
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


struct InputPS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD0;
	float4 zMap : TEXCOORD1;
};


float4 main(InputPS In) : SV_TARGET
{
	//ハーフランバート計算
	float HL = max(0, dot(In.n, -lightDir.xyz));
	HL = HL * 0.5f + lightDir.w;
	HL = HL * HL;

	//スペキュラ+エミッシブ計算
	float3 HV = max(0, normalize(eyePos.xyz + lightDir.xyz));
	float S = pow(max(0, dot(HV, In.n)), 2)*eyePos.w;
	float3 SP = sTex1.Sample(samp1, In.uv).rgb * specular.rgb * S;
	float3 EM = sTex2.Sample(samp2, In.uv).rgb * emissive.rgb;

	float4 Out = diffuse * sTex0.Sample(samp0, In.uv);
	Out.rgb *= HL;
	Out.rgb += SP + EM;

	Out *= color;


	//射影空間のXY座標をテクスチャ座標に変換
	float2 transTexCoord;
	transTexCoord.x = (1.0f + In.zMap.x / In.zMap.w) * 0.5f;
	transTexCoord.y = (1.0f - In.zMap.y / In.zMap.w) * 0.5f;

	//影用の減衰値
	float decay = 0;

	//深度マップ範囲外は影なし
	if (transTexCoord.x < 0 || transTexCoord.x > 1 || transTexCoord.y < 0 || transTexCoord.y > 1)
		decay = 1.0f;
	else
	{
		//リアルZ値抽出
		float2 rZ = zMapTexture.Sample(zMapSampler, transTexCoord).xy;

		//ライト目線によるZ値の再算出
		float zValue = In.zMap.z / In.zMap.w;

		//算出点がzMapのZ値よりも大きければ影
		if (rZ.g < zValue)
		{
			//バイアス調整
			float variance = rZ.g - rZ.r * rZ.r * 0.95 + 0.005f;
			//チェビシェフの不等式σ^2 / ( σ^2 + ( t- E(x) ) )
			decay = max(0.3,variance / (variance + zValue - rZ.g));
		}
		else
			decay = 1.0f;
	}
	Out.rgb *= min(max(decay, lightDir.w), HL);


	return Out;
}
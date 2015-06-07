static const float2 RCP_FRAME = float2(_1TEXEL_U, _1TEXEL_V);

// ディフューズマップテクスチャ
Texture2D		tex0	: register(t0);
SamplerState	samp0	: register(s0);

struct PSInput{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};


float FXAALuma(float4 rgba) {
	//(ax * bx + ay * by + az * bz) NTSCグレースケール化
	return dot(rgba.rgb, float3(0.299, 0.587, 0.114));
}

float4 main(PSInput In) : SV_TARGET
{
	float luma_LT = FXAALuma(tex0.Sample(samp0, In.uv + (float2(-0.5, -0.5) * RCP_FRAME)));
	float luma_LB = FXAALuma(tex0.Sample(samp0, In.uv + (float2(-0.5, +0.5) * RCP_FRAME)));
	float luma_RT = FXAALuma(tex0.Sample(samp0, In.uv + (float2(+0.5, -0.5) * RCP_FRAME))) + 0.002604167;
	float luma_RB = FXAALuma(tex0.Sample(samp0, In.uv + (float2(+0.5, +0.5) * RCP_FRAME)));
	float4 luma_C = tex0.Sample(samp0, In.uv); //中央

	//周囲の最大最小
	float luma_Max = max(max(luma_RT, luma_RB), max(luma_LT, luma_LB));
	float luma_Min = min(min(luma_RT, luma_RB), min(luma_LT, luma_LB));

	//最大照度の1/8
	float lumaMaxScaledClamped = max(0.05, luma_Max * 0.125);

	//グレースケール
	float luma_M = FXAALuma(luma_C);
	//照度差
	float lumaMaxSubMinM = max(luma_Max, luma_M) - min(luma_Min, luma_M);

	//変化を比較
	if (lumaMaxSubMinM < lumaMaxScaledClamped)
	{
		//変化が少ない場合は元の色を返す
		return luma_C;
	}
	else
	{
		//各方向の照度差
		float dirSwMinusNe = luma_LB - luma_RT;
		float dirSeMinusNw = luma_RB - luma_LT;

		//照度ベクトル
		float2 dir1 = normalize(float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw));
		//照度ベクトルの差
		float2 dirAbsMinTimesC = dir1 / (8.0 * min(abs(dir1.x), abs(dir1.y)));
		//範囲に収める
		float2 dir2 = clamp(dirAbsMinTimesC, -2.0, 2.0) * (RCP_FRAME*2.0f);

		//移動量算出
		dir1 *= (RCP_FRAME/2.0f);
		//各方向取得
		float4 rgbyN1 = tex0.Sample(samp0, In.uv - dir1); //半ドット×照度ベクトル 左上
		float4 rgbyP1 = tex0.Sample(samp0, In.uv + dir1); //半ドット×照度ベクトル 右下
		float4 rgbyN2 = tex0.Sample(samp0, In.uv - dir2); //２ドット×照度ベクトル 左上
		float4 rgbyP2 = tex0.Sample(samp0, In.uv + dir2); //２ドット×照度ベクトル 右下
		//加算した値
		float4 rgbyA = rgbyN1 + rgbyP1; //加算して結果を保存
		//1/4した値
		float4 rgbyB = (rgbyN2 + rgbyP2 + rgbyA) * 0.25;

		//グレースケール
		float rgbyBM = FXAALuma(rgbyB);
		//半値分岐
		if ((rgbyBM < luma_Min) || (rgbyBM > luma_Max))
		{
			//1/2を返す
			return rgbyA * 0.5;
		}
		else
		{
			//1/4を返す
			return rgbyB;
		}
	}
}
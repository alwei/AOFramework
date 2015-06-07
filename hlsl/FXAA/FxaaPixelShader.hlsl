static const float2 RCP_FRAME = float2(_1TEXEL_U, _1TEXEL_V);

// �f�B�t���[�Y�}�b�v�e�N�X�`��
Texture2D		tex0	: register(t0);
SamplerState	samp0	: register(s0);

struct PSInput{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};


float FXAALuma(float4 rgba) {
	//(ax * bx + ay * by + az * bz) NTSC�O���[�X�P�[����
	return dot(rgba.rgb, float3(0.299, 0.587, 0.114));
}

float4 main(PSInput In) : SV_TARGET
{
	float luma_LT = FXAALuma(tex0.Sample(samp0, In.uv + (float2(-0.5, -0.5) * RCP_FRAME)));
	float luma_LB = FXAALuma(tex0.Sample(samp0, In.uv + (float2(-0.5, +0.5) * RCP_FRAME)));
	float luma_RT = FXAALuma(tex0.Sample(samp0, In.uv + (float2(+0.5, -0.5) * RCP_FRAME))) + 0.002604167;
	float luma_RB = FXAALuma(tex0.Sample(samp0, In.uv + (float2(+0.5, +0.5) * RCP_FRAME)));
	float4 luma_C = tex0.Sample(samp0, In.uv); //����

	//���͂̍ő�ŏ�
	float luma_Max = max(max(luma_RT, luma_RB), max(luma_LT, luma_LB));
	float luma_Min = min(min(luma_RT, luma_RB), min(luma_LT, luma_LB));

	//�ő�Ɠx��1/8
	float lumaMaxScaledClamped = max(0.05, luma_Max * 0.125);

	//�O���[�X�P�[��
	float luma_M = FXAALuma(luma_C);
	//�Ɠx��
	float lumaMaxSubMinM = max(luma_Max, luma_M) - min(luma_Min, luma_M);

	//�ω����r
	if (lumaMaxSubMinM < lumaMaxScaledClamped)
	{
		//�ω������Ȃ��ꍇ�͌��̐F��Ԃ�
		return luma_C;
	}
	else
	{
		//�e�����̏Ɠx��
		float dirSwMinusNe = luma_LB - luma_RT;
		float dirSeMinusNw = luma_RB - luma_LT;

		//�Ɠx�x�N�g��
		float2 dir1 = normalize(float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw));
		//�Ɠx�x�N�g���̍�
		float2 dirAbsMinTimesC = dir1 / (8.0 * min(abs(dir1.x), abs(dir1.y)));
		//�͈͂Ɏ��߂�
		float2 dir2 = clamp(dirAbsMinTimesC, -2.0, 2.0) * (RCP_FRAME*2.0f);

		//�ړ��ʎZ�o
		dir1 *= (RCP_FRAME/2.0f);
		//�e�����擾
		float4 rgbyN1 = tex0.Sample(samp0, In.uv - dir1); //���h�b�g�~�Ɠx�x�N�g�� ����
		float4 rgbyP1 = tex0.Sample(samp0, In.uv + dir1); //���h�b�g�~�Ɠx�x�N�g�� �E��
		float4 rgbyN2 = tex0.Sample(samp0, In.uv - dir2); //�Q�h�b�g�~�Ɠx�x�N�g�� ����
		float4 rgbyP2 = tex0.Sample(samp0, In.uv + dir2); //�Q�h�b�g�~�Ɠx�x�N�g�� �E��
		//���Z�����l
		float4 rgbyA = rgbyN1 + rgbyP1; //���Z���Č��ʂ�ۑ�
		//1/4�����l
		float4 rgbyB = (rgbyN2 + rgbyP2 + rgbyA) * 0.25;

		//�O���[�X�P�[��
		float rgbyBM = FXAALuma(rgbyB);
		//���l����
		if ((rgbyBM < luma_Min) || (rgbyBM > luma_Max))
		{
			//1/2��Ԃ�
			return rgbyA * 0.5;
		}
		else
		{
			//1/4��Ԃ�
			return rgbyB;
		}
	}
}
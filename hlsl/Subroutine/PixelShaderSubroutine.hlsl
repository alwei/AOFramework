

/*!-----------------------------------------------------------
//	@brief		VSM���Z�֐�
//	@param[in]	_uv		UV�l
//	@param[in]	_rZ		���A��Z�l	
//	@param[in]	_zw		���C�g�ڐ���Z�v�f�i�������ꂽ�l�j
//	@param[in]	_factor	�A���r�G���g�v�f
//	@return		�Z�o���ꂽ�����l
//	@author		Tatsunori Aoyama
//	@date		2015/02/05
------------------------------------------------------------*/
inline float VSM(
	uniform float2 _uv=float2(0,0),
	uniform float2 _rZ=float2(0,0),	
	uniform float2 _zw = float2(0, 0),
	uniform float _factor = 0.0f)
{
	//�[�x�}�b�v�͈͊O�͉e�Ȃ�
	float shadow = _factor;
	if (!(_uv.x < 0 || _uv.x > 1 || _uv.y < 0 || _uv.y > 1))
	{
		//���C�g�ڐ��ɂ��Z�l�̍ĎZ�o
		float zValue = _zw.x / _zw.y;

		//�Z�o�_���[�xMAP��Z���傫����Ήe�A��������Ήe����Ȃ�
		float bias = 0.005f;
		if (_rZ.r < zValue)
		{
			//�`�F�r�V�F�t�̕s����

			//��^2 = E(x^2) - E(x)^2
			float variance = (_rZ.g - _rZ.r * _rZ.r) + bias;	

			// e = ( t- E(x) )
			float tu = zValue - _rZ.r;

			//��^2 / ( ��^2 + e )
			shadow = min(_factor, variance / (variance + tu));
		}
	}

	return shadow;
}
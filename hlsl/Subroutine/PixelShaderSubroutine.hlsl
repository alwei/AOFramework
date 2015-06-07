

/*!-----------------------------------------------------------
//	@brief		VSM演算関数
//	@param[in]	_uv		UV値
//	@param[in]	_rZ		リアルZ値	
//	@param[in]	_zw		ライト目線のZ要素（分解された値）
//	@param[in]	_factor	アンビエント要素
//	@return		算出された減衰値
//	@author		Tatsunori Aoyama
//	@date		2015/02/05
------------------------------------------------------------*/
inline float VSM(
	uniform float2 _uv=float2(0,0),
	uniform float2 _rZ=float2(0,0),	
	uniform float2 _zw = float2(0, 0),
	uniform float _factor = 0.0f)
{
	//深度マップ範囲外は影なし
	float shadow = _factor;
	if (!(_uv.x < 0 || _uv.x > 1 || _uv.y < 0 || _uv.y > 1))
	{
		//ライト目線によるZ値の再算出
		float zValue = _zw.x / _zw.y;

		//算出点が深度MAPのZより大きければ影、小さければ影じゃない
		float bias = 0.005f;
		if (_rZ.r < zValue)
		{
			//チェビシェフの不等式

			//σ^2 = E(x^2) - E(x)^2
			float variance = (_rZ.g - _rZ.r * _rZ.r) + bias;	

			// e = ( t- E(x) )
			float tu = zValue - _rZ.r;

			//σ^2 / ( σ^2 + e )
			shadow = min(_factor, variance / (variance + tu));
		}
	}

	return shadow;
}
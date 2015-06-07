/*************************************************************
*	@file   	iUiObject.hpp
*	@brief  	iUiObjectクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObject_hpp_	// インクルードガード
#define _Include_iUiObject_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Object/GameObject.hpp"
#include"../Math/MathDefine.hpp"
#include"../Api/DirectX11Resource.hpp"


namespace ao
{
	class float2;
	class float4;
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObject
	//	@brief  UIのインスタンスレンダリング用オブジェクト
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObject : public GameObject
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		ファクトリから生成する
		//	@param[in]	_pUiInstance	UIインスタンス
		//	@param[in]	_leftTop		左上のUV座標
		//	@param[in]	_rightTop		右上のUV座標
		//	@param[in]	_leftBottom		左下のUV座標
		//	@param[in]	_rightBottom	右下のUV座標
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObject(
			ao::UiInstance* _pInstance,
			const ao::float2& _leftTop,
			const ao::float2& _rightTop,
			const ao::float2& _leftBottom,
			const ao::float2& _rightBottom);

		~iUiObject();

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_x		X座標
		//  @param[in]  _y		Y座標
		//  @param[in]  _w		横幅
		//  @param[in]  _h		縦幅
		//	@param[in]	_degree	回転角度(デグリー値)[省略可]
		//	@param[in]	_r		赤色[省略可]
		//	@param[in]	_g		緑色[省略可]
		//	@param[in]	_b		青色[省略可]
		//	@param[in]	_a		透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree,
			const float _r, const float _g, const float _b, const float _a);

		/*!-----------------------------------------------------------
		//	@brief		レンダリング
		//	@param[in]	_x		X座標
		//  @param[in]  _y		Y座標
		//  @param[in]  _w		横幅
		//  @param[in]  _h		縦幅
		//	@param[in]	_degree	回転角度(デグリー値)[省略可]
		//	@param[in]	_r		赤色[省略可]
		//	@param[in]	_g		緑色[省略可]
		//	@param[in]	_b		青色[省略可]
		//	@param[in]	_a		透過率[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree = 0,
			const float4& _color = float4::one);

	private:
		struct CutUV
		{
			float2 lt;
			float2 rt;
			float2 lb;
			float2 rb;
			CutUV(
				const float2& _lt, const float2& _rt,
				const float2& _lb, const float2& _rb)
			{
				this->lt = _lt;
				this->rt = _rt;
				this->lb = _lb;
				this->rb = _rb;
			}
		};

	private:
		CutUV uv;
		ao::UiInstance* pInstance;
	};
}

#endif // _Include_iUiObject_hpp_
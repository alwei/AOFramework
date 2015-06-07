/*************************************************************
*	@file   	UiObject.hpp
*	@brief  	UiObjectクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/02/10
*************************************************************/

#ifndef _Include_UiObject_hpp_	// インクルードガード
#define _Include_UiObject_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"GameObject.hpp"
#include"../Math/MathDefine.hpp"
#include"../Api/DirectX11Resource.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  UiObject
	//	@brief  UI用のオブジェクト
	//	@author	Tatsunori Aoyama
	//	@date	2014/02/10
	------------------------------------------------------------*/
	class UiObject : public GameObject
	{
	public:
		/// コンストラクタ
		UiObject();

		/// デストラクタ
		~UiObject();

		/*!-----------------------------------------------------------
		//	@brief		リソース読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_filter		フィルター[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/02/10
		------------------------------------------------------------*/
		virtual void LoadResource(const ao::string& _filePath,const SamplerState _filter = SamplerState::POINT);

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
		//	@date		2014/03/02
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree,
			const float _r,const float _g,const float _b,const float _a);

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
		//	@date		2014/02/10
		------------------------------------------------------------*/
		virtual void Render(
			const int _x, const int _y,
			const float _w, const float _h,
			const float _degree = 0,
			const float4& _color=float4::one);

	private:
		BaseTexture* pTexture;
	};
}

#endif // _Include_UiObject_hpp_
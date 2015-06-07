/*************************************************************
*	@file   	CutSprite.hpp
*	@brief  	CutSpriteクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/03/12
*************************************************************/

#ifndef _Include_CutSprite_hpp_	// インクルードガード
#define _Include_CutSprite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"Sprite.hpp"
#include"../Math/MathDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  CutSprite
	//	@brief  UV指定したスプライト
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/12
	------------------------------------------------------------*/
	class CutSprite : public Sprite
	{
	public:
		CutSprite(){};
		~CutSprite(){};

		/*!-----------------------------------------------------------
		//	@brief		左上基点のUV指定して頂点バッファを生成
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_leftTop		UV左上
		//	@param[in]	_rightTop		UV右上
		//	@param[in]	_leftButtom		UV左下
		//	@param[in]	_rightButtom	UV右下
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCornerCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftButtom,
			const float2& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		中心基点のUV指定して頂点バッファを生成
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_leftTop		UV左上
		//	@param[in]	_rightTop		UV右上
		//	@param[in]	_leftButtom		UV左下
		//	@param[in]	_rightButtom	UV右下
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCenterCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftButtom,
			const float2& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		左上基点のUV指定して頂点バッファを生成
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_leftTop		UV左上
		//	@param[in]	_rightTop		UV右上
		//	@param[in]	_leftButtom		UV左下
		//	@param[in]	_rightButtom	UV右下
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCornerCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2&& _leftTop,
			const float2&& _rightTop,
			const float2&& _leftButtom,
			const float2&& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		中心基点のUV指定して頂点バッファを生成
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_leftTop		UV左上
		//	@param[in]	_rightTop		UV右上
		//	@param[in]	_leftButtom		UV左下
		//	@param[in]	_rightButtom	UV右下
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		HRESULT	CreateCenterCutVertexBuffer(
			ID3D11Device* _pd3dDevice,
			const float2&& _leftTop,
			const float2&& _rightTop,
			const float2&& _leftButtom,
			const float2&& _rightButtom);

		/*!-----------------------------------------------------------
		//	@brief		三角形の頂点数取得
		//  @return     レンダリングする頂点数
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const u32 GetVertexCount() const override {
			return 4;
		}
	};
}

#endif // _Include_CutSprite_hpp_
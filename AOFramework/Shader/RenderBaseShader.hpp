/*************************************************************
*	@file   	RenderBaseShader.hpp
*	@brief  	RenderBaseShaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_RenderBaseShader_hpp_	// インクルードガード
#define _Include_RenderBaseShader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"BaseShader.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  RenderBaseShader
	//	@brief  レンダリング基底シェーダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class RenderBaseShader : public BaseShader
	{
	public:
		RenderBaseShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) : 
			BaseShader(_pd3dDevice,_pd3dDeviceContext){};

		virtual	~RenderBaseShader()override{};

		/*!-----------------------------------------------------------
		//	@brief		プッシュバック開始時の処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		virtual	void	OnPushbackEnter(){};

		/*!-----------------------------------------------------------
		//	@brief		プッシュバック終了時の処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/09
		------------------------------------------------------------*/
		virtual	void	OnPushbackQuit(){};

		/*!-----------------------------------------------------------
		//	@brief		描画メソッド
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		virtual	void	OnDraw(){};

		/*!-----------------------------------------------------------
		//	@brief		リザルト処理
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		virtual	void	OnResult(){};

		/*!-----------------------------------------------------------
		//	@brief		クリア処理
		//	@author		Tatsunori Aoyama
		//	@date		2015/01/03
		------------------------------------------------------------*/
		virtual void	OnClear(){};
	};
}

#endif // _Include_RenderBaseShader_hpp_
/*************************************************************
*	@file   	iUiObjectFactory.hpp
*	@brief  	iUiObjectFactoryクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/03/29
*************************************************************/

#ifndef _Include_iUiObjectFactory_hpp_	// インクルードガード
#define _Include_iUiObjectFactory_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include<Windows.h>
#include"../Define/SystemDefine.hpp"
#include"../Api/DirectX11Resource.hpp"


namespace ao
{
	class iUiObject;
	class iUiObjectExecutor;
	class float2;
	struct UiInstance;

	/*!-----------------------------------------------------------
	//	@class  iUiObjectFactory
	//	@note	一つのアトラスのみ対応
	//	@brief  UIインスタンスファクトリ
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/29
	------------------------------------------------------------*/
	class iUiObjectFactory
	{
	public:
		iUiObjectFactory();
		~iUiObjectFactory();

		/*!-----------------------------------------------------------
		//	@brief		インスタンス作成
		//	@param[in]	_filePath	読み込むファイルパス
		//	@param[in]	_numAlloc	確保するバッファ数
		//	@return		レンダリング実行クラス
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObjectExecutor* CreateInstance(
			const string _filePath,
			const UINT _numAlloc = 100,
			const SamplerState _sampState = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		UIインスタンスオブジェクト作成
		//	@param[in]	_leftTop		左上UV座標（ピクセル単位）
		//	@param[in]	_rightTop		右上UV座標（ピクセル単位）
		//	@param[in]	_leftBottom		左下UV座標（ピクセル単位）
		//	@param[in]	_rightBottom	右下UV座標（ピクセル単位）
		//	@return		生成されたUIインスタンスオブジェクト
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		iUiObject* CreateObject(
			const float2& _leftTop,
			const float2& _rightTop,
			const float2& _leftBottom,
			const float2& _rightBottom);

		/*!-----------------------------------------------------------
		//	@brief		UIインスタンスオブジェクト作成
		//	@param[in]	_ppOutArray		出力するUIオブジェクト配列
		//	@param[in]	_xNum			横の分割数
		//	@param[in]	_yNum			縦の分割数
		//	@param[in]	_xSize			一枚分の横サイズ
		//	@param[in]	_ySize			一枚分の縦サイズ
		//	@param[in]	_atlasWidth		アトラスの横幅
		//	@param[in]	_atlasHeight	アトラスの縦幅
		//	@return		生成されたUIインスタンスオブジェクト
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		HRESULT CreateObjectArray(
			iUiObject** const _ppOutArray,
			const int _xNum, const int _yNum,
			const int _xSize, const int _ySize,
			const int _atlasWidth, const int _atlasHeight);

	private:
		UiInstance* pInstance;
		iUiObjectExecutor* pExecutor;
		std::list<iUiObject*> pBufferList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		バッファ解放
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/29
		------------------------------------------------------------*/
		inline void DeleteBuffer();
	};
}

#endif // _Include_iUiObjectFactory_hpp_
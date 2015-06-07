/*************************************************************
*	@file   	DirectX11Resource.hpp
*	@brief  	DirectX11Resourceクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_DirectX11Resource_hpp_	// インクルードガード
#define _Include_DirectX11Resource_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<map>
#include<mutex>
#include"DirectX11BaseApi.hpp"
#include"../Resource/BaseTexture.hpp"
#include"../Resource/Sprite.hpp"
#include"../Resource/Mesh.hpp"
#include"../Motion/StaMeshMotionController.hpp"
#include"../Define/RenderingDefine.hpp"

namespace ao
{
	class CustomSprite;

	/*!-----------------------------------------------------------
	//	@class  DirectX11Resource
	//	@brief  リソース管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class DirectX11Resource : public DirectX11BaseApi
	{
	public:
		DirectX11Resource(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11Resource()override final;

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		HRESULT	Initialize();

		/*!-----------------------------------------------------------
		//	@brief		テクスチャの読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_filterType	フィルタータイプ[省略可]
		//	@return		生成されたテクスチャポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		BaseTexture* LoadTextureFromFile(
			const string _filePath,
			const SamplerState _filterType=SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		スプライトの読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_pivot		スプライトのピボット[省略可]
		//	@param[in]	_filterType	フィルタータイプ[省略可]
		//	@return		生成されたスプライトハンドルポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		Sprite*	LoadSpriteFromFile(
			const string _filePath,
			const SpritePivot _pivot=SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		UV指定ありのスプライト読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_top		上座標
		//	@param[in]	_left		左座標
		//	@param[in]	_buttom		下座標
		//	@param[in]	_right		右座標
		//	@param[in]	_xSize		全体の横サイズ
		//	@param[in]	_ySize		全体の縦サイズ
		//  @param[in]  _useCache	キャッシュ使用フラグ[省略可]
		//	@param[in]	_pivot		スプライトのピボット[省略可]
		//	@param[in]	_filterType	フィルタータイプ[省略可]
		//	@return		生成されたスプライトハンドル
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		Sprite* LoadSpriteFromFile(
			const string _filePath,
			const int _top, const int _left,
			const int _buttom, const int _right,
			const int _xSize, const int _ySize,
			const BOOL _useCache = FALSE,
			const SpritePivot _pivot = SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		スプライトの読み込み
		//	@param[out]	_ppOut		出力するスプライト配列
		//  @param[in]  _xDiv		横分割数
		//  @param[in]  _yDiv		縦分割数
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_pivot		スプライトのピボット[省略可]
		//	@param[in]	_filterType	フィルタータイプ[省略可]
		//	@return		生成されたスプライト配列
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		Sprite*	LoadDivSpriteFromFile(
			Sprite** _ppOut,
			int _xDiv,
			int _yDiv,
			const string _filePath,
			const SpritePivot _pivot = SpritePivot::Center,
			const SamplerState _filterType = SamplerState::LINEAR);

		/*!-----------------------------------------------------------
		//	@brief		Xファイルの読み込み
		//	@param[in]	_filePath	ファイルパス
		//  @param[in]  _edgeType	エッジタイプ[省略可]
		//	@return		生成されたメッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		StMesh* LoadMeshFromX(const string _filePath);

		/*!-----------------------------------------------------------
		//	@brief		PMDの読み込み
		//	@param[in]	_filePath		ファイルパス
		//	@param[in]	_meshType		メッシュタイプ
		//  @param[in]  _enableFixPath	パス修正の有無[省略可]
		//	@return		生成されたメッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		BaseMesh* LoadMeshFromPmd(
			const string _filePath,
			const ResourceType _meshType,
			const BOOL _enableFixPath=TRUE);

		/*!-----------------------------------------------------------
		//	@brief		Objファイルの読み込み
		//	@note		バグっているので凍結中
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_mtlPath	MTLファイルパス[省略可]
		//	@return		生成されたメッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		StMesh* LoadMeshFromObj(const string _filePath, const string _mtlPath = _T(""));

		/*!-----------------------------------------------------------
		//	@brief		FBXの読み込み
		//	@param[in]	_filePath		ファイルパス
		//  @param[in]  _meshType		メッシュタイプ
		//  @param[in]  _enablefixPath	パスの修正フラグ[省略可]
		//  @param[out] _pMotionCtrl	出力するモーションコントローラー[省略可]
		//  @return     生成されたメッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/10
		------------------------------------------------------------*/
		BaseMesh* LoadMeshFromFbx(
			const string _filePath,
			const ResourceType _meshType,
			const BOOL enableFixPath=TRUE,
			IMeshMotionController* _pMotionCtrl = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		FBXモーションを読み込み
		//	@return		_pOut		出力するモーションコントローラー
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_meshType	メッシュタイプ
		//	@param[in]	_isAdd		追加フラグ[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		HRESULT	LoadMotionFromFbx(
			IMeshMotionController* _pOut,
			const string _filePath,
			const ResourceType _meshType,
			const BOOL _isAdd=TRUE);

		/*!-----------------------------------------------------------
		//	@brief		ボックスメッシュ読み込み
		//	@return		生成されたBoxメッシュ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/14
		------------------------------------------------------------*/
		StMesh* LoadBoxMesh();

		/*!-----------------------------------------------------------
		//	@brief		スフィアメッシュ読み込み
		//	@param[in]	_division 分割数[省略可]
		//	@return		生成されたスフィアメッシュ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/14
		------------------------------------------------------------*/
		StMesh* LoadSphereMesh(int _division = 16);

	private:
		typedef std::vector<Sprite*> SpriteArray;

	private:
		SmartComPtr<ID3D11SamplerState> pDefaultSamplerState[(int)SamplerState::MAX];
		Sprite leftTopSprite;
		Sprite centerSprite;
		std::map<string,BaseTexture*> pTextureMap;
		std::map<string, Sprite*> pSpriteMap;
		std::map<string, SpriteArray*> pSpriteArrayMap;
		std::map<string, BaseMesh*> pMeshMap;
		std::map<string, StaMotionData*> pStaMotionMap;
		std::mutex textureMutex;
		std::mutex spriteMutex;
		std::mutex meshMutex;

	private:
		/*!-----------------------------------------------------------
		//	@brief		サンプラーステート作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ作成
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/16
		------------------------------------------------------------*/
		inline	HRESULT	CreateSpriteVertexBuffer();
	};
}

#endif // _Include_DirectX11Resource_hpp_
/*************************************************************
*	@file   	Sprite.hpp
*	@brief  	Spriteクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_Sprite_hpp_	// インクルードガード
#define _Include_Sprite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"

namespace ao
{
	enum class SpritePivot : int
	{
		LeftTop = 0,	//実装済み
		RightTop,		//TODO:未実装
		LeftButtom,		//TODO:未実装
		RightButtom,	//TODO:未実装
		Center,			//実装済み
		TypeMax
	};

	/*!-----------------------------------------------------------
	//	@class  Sprite
	//	@brief  スプライト
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class Sprite : public BaseGeometry
	{
	public:
		Sprite();
		virtual	~Sprite(){};

		struct Vertex
		{
			float x, y, z;
			float nx, ny, nz;
			float u, v;
		};

		union UV
		{
			struct
			{
				float x, y;
			};
			struct
			{
				float f[2];
			};
		};

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ作成(左上原点)
		//	@param[in]	デバイスポインタ
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCornerVertexBuffer(ID3D11Device* _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ作成(中心原点)
		//	@param[in]	デバイスポインタ
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCenterVertexBuffer(ID3D11Device* _pd3dDevice);

		/*!-----------------------------------------------------------
		//	@brief		三角形の頂点数取得
		//  @return     レンダリングする頂点数
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const u32 GetVertexCount() const override {
			return 3;
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点ストライド取得
		//	@return		頂点ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		const u32 GetStride() const override {
			return sizeof(Vertex);
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点データサイズ取得
		//	@return		頂点データサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetVertexBufferSize() const override{
			return this->vBufferSize;
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override {
			return ResourceType::SPRITE;
		}

		/*!-----------------------------------------------------------
		//	@brief		ピボットタイプ取得
		//  @return     ピボットタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/08
		------------------------------------------------------------*/
		const SpritePivot GetPivotType() const {
			return this->pivotType;
		}
			
		/*!-----------------------------------------------------------
		//	@brief		確保した頂点データを取得
		//  @param[out]  _pOutBufferData	出力するCPU頂点バッファデータ[省略可]
		//  @return     出力するCPU頂点バッファデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/08
		------------------------------------------------------------*/
		const void* GetVertexBufferData(void* const _pOutBufferData=nullptr);

	public:
		BaseTexture* pTexture;

	protected:
		SpritePivot pivotType;
		u32 vBufferSize;

		/*!-----------------------------------------------------------
		//	@brief		共通の頂点バッファ作成
		//	@param[out]	_pOutVBuffer	出力する頂点バッファ
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_vBufferSize	頂点バッファサイズ
		//	@param[in]	_pSrcData		作成する元データ
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		HRESULT CreateCommonVertexBuffer(
			ID3D11Buffer** _ppOutVBuffer,
			ID3D11Device* const _pd3dDevice,
			const UINT _vBufferSize,
			const void* const _pSrcData);
	};
}

#endif // _Include_Sprite_hpp_
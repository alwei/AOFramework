/*************************************************************
*	@file   	CustomSprite.hpp
*	@brief  	CustomSpriteクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/03/08
*************************************************************/

#ifndef _Include_CustomSprite_hpp_	// インクルードガード
#define _Include_CustomSprite_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"Sprite.hpp"
#include"../Math/MathDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  CustomSprite
	//	@brief  カスタムスプライト
	//	@author	Tatsunori Aoyama
	//	@date	2015/03/08
	------------------------------------------------------------*/
	template<class T>
	class CustomSprite : public Sprite
	{
	public:
		CustomSprite():pBuffer(nullptr){};
		~CustomSprite(){
			if( this->pBuffer)
				delete this->pBuffer
		};

		/*!-----------------------------------------------------------
		//	@brief		三角形の頂点数取得
		//  @return     レンダリングする頂点数
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		const size_t GetVertexCount() const override {
			return this->nVertexCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点ストライド取得
		//	@return		頂点ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/13
		------------------------------------------------------------*/
		const u32 GetStride() const override {
			return this->nStride;
		}

		/*!-----------------------------------------------------------
		//	@brief		マニュアルで頂点をバッファ作成
		//	@param[in]	_pd3dDevice		デバイスポインタ
		//	@param[in]	_pSrcData		頂点データ
		//  @param[in]  _nVertexCount	頂点数
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/08
		------------------------------------------------------------*/
		HRESULT CreateCustomVertexBuffer(
			ID3D11Device* const _pd3dDevice,
			const T* const _pSrcData,
			const size_t _nVertexCount)
		{
			this->nStride = _nVertexCount;
			this->vBufferSize = sizeof(T)*_nVertexCount;
			this->pBuffer = new T[_nVertexCount];
			::CopyMemory(this->pBuffer, _pSrcData, this->vBufferSize);

			this->CreateCommonVertexBuffer(
				this->pGpuVertexBuffer.ToCreator(),
				_pd3dDevice,
				this->vBufferSize,
				_pSrcData);
		}

		/*!-----------------------------------------------------------
		//	@brief		確保した頂点データを取得
		//  @param[out]  _pOutBufferData	出力するCPU頂点バッファデータ[省略可]
		//  @return     出力するCPU頂点バッファデータ
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/13
		------------------------------------------------------------*/
		const void* GetVertexBufferData(void* const _pOutBufferData = nullptr)
		{
			if (_pOutBufferData == nullptr)
				return this->pGpuVertexBuffer;

			::CopyMemory(_pOutBufferData, this->pGpuVertexBuffer, this->vBufferSize);
			return _pOutBufferData;
		}

	public:
		size_t nStride;
		size_t nVertexCount;
		T* pCpuVertexBuffer;
	};
}

#endif // _Include_CustomSprite_hpp_
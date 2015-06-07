/*************************************************************
*	@file   	BaseGeometry.hpp
*	@brief  	BaseGeometryクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_BaseGeometry_hpp_	// インクルードガード
#define _Include_BaseGeometry_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<d3d11.h>
#include"IResource.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseGeometry
	//	@brief  ジオメトリ基底クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class BaseGeometry : public IResource
	{
	public:
		BaseGeometry(){};
		virtual	~BaseGeometry(){};

		/*!-----------------------------------------------------------
		//	@brief		頂点数取得
		//  @return     頂点数
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/13
		------------------------------------------------------------*/
		virtual const u32 GetVertexCount() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点ストライド取得
		//	@return		頂点ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		virtual const u32 GetStride() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点データサイズ取得
		//	@return		頂点データサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual const u32 GetVertexBufferSize() const {
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ取得
		//	@return		頂点バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/07
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetVertexBufferPtr() final{
			return this->pGpuVertexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファを外部で作成用にポインタを渡す
		//  @return     頂点バッファポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** ToVertexBufferCreator() final {
			return this->pGpuVertexBuffer.ToCreator();
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファの参照権をコピー
		//	@note		アプリケーション部分では使用しないでください
		//	@param[in]	_srcSprite	参照元のスプライト
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/08
		------------------------------------------------------------*/
		template<class T>
		void CopyVertexBufferRef(T& _src){
			this->pGpuVertexBuffer = _src.pGpuVertexBuffer;
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const override {
			return ResourceType::GEOMETRY;
		}

	protected:
		SmartComPtr<ID3D11Buffer> pGpuVertexBuffer;
	};
}

#endif // _Include_BaseGeometry_hpp_
/*************************************************************
*	@file   	Geometry.hpp
*	@brief  	Geometryクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Geometry_hpp_	// インクルードガード
#define _Include_Geometry_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"Vertex.hpp"

namespace ao
{
	/// 基底メッシュジオメトリ
	class BaseMeshGeometry : public BaseGeometry
	{
	public:
		BaseMeshGeometry(){}
		virtual ~BaseMeshGeometry(){}

		virtual void* GetVertexBuffer()
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックバッファのデータサイズ取得
		//  @return     インデックスバッファのデータサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual const u32 GetIndexBufferSize() const final
		{
			return static_cast<u32>(this->index.size() * sizeof(u32));
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックスバッファのデータを取得
		//  @return     インデックスバッファのデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/03/12
		------------------------------------------------------------*/
		virtual const u32* GetIndexBuffer() const final
		{
			return this->index.data();
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ取得
		//	@return		頂点バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetIndexBufferPtr() final
		{
			return this->pGpuIndexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックスバッファを外部で作成用にポインタを渡す
		//	@param[in]	インデックスポインタ
		//  @return     頂点バッファポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		virtual ID3D11Buffer** ToIndexBufferCreator() final 
		{
			return this->pGpuIndexBuffer.ToCreator();
		}

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファの参照権をコピー
		//	@note		アプリケーション部分では使用しないでください
		//	@param[in]	_srcSprite	参照元のスプライト
		//	@author		Tatsunori Aoyama
		//	@date		2015/03/12
		------------------------------------------------------------*/
		template<class T>
		void CopyIndexBufferRef(T& _src)
		{
			this->pGpuIndexBuffer = _src.pGpuIndexBuffer;
		}

	public:
		std::vector<u32> index;
		SmartComPtr<ID3D11Buffer> pGpuIndexBuffer;
	};

	/// 静的メッシュジオメトリ
	class StMeshGeometry : public BaseMeshGeometry
	{
	public:
		void* GetVertexBuffer() final
		{
			return this->vertex.data();
		}

		const u32 GetStride() const override final
		{
			return sizeof(StVertex);
		}

		const u32 GetVertexBufferSize() const override final
		{
			return static_cast<u32>(sizeof(StVertex)*this->vertex.size());
		}

		const ResourceType GetType() const override final
		{
			return ResourceType::ST_GEOMETRY;
		}

	public:
		std::vector<StVertex> vertex;
	};


	/// 動的メッシュジオメトリ
	class SkMeshGeometry : public BaseMeshGeometry
	{
	public:
		void* GetVertexBuffer() final
		{
			return this->vertex.data();
		}

		const u32 GetStride() const override final
		{
			return sizeof(SkVertex);
		}

		const u32 GetVertexBufferSize() const override final
		{
			return static_cast<u32>(sizeof(SkVertex)*this->vertex.size());
		}

		const ResourceType GetType() const override final 
		{
			return ResourceType::SK_GEOMETRY;
		}

	public:
		std::vector<SkVertex> vertex;
	};
}

#endif // _Include_Geometry_hpp_
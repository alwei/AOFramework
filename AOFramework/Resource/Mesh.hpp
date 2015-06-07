/*************************************************************
*	@file   	Mesh.hpp
*	@brief  	Meshクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/19
*************************************************************/

#ifndef _Include_Mesh_hpp_	// インクルードガード
#define _Include_Mesh_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include<vector>
#include"../Define/SystemDefine.hpp"
#include"BaseGeometry.hpp"
#include"BaseTexture.hpp"
#include"Material.hpp"
#include"MeshGeometry.hpp"

namespace ao
{
	enum class EdgeType : int
	{
		Soft,
		Hard
	};

	/*!-----------------------------------------------------------
	//	@class  BaseMesh
	//	@brief  基底メッシュクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BaseMesh : public IResource
	{
	public:
		BaseMesh(){};
		virtual ~BaseMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ取得
		//	@return		頂点バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual ID3D11Buffer** GetVertexBuffer()
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックスバッファ取得
		//	@return		インデックスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		virtual ID3D11Buffer* GetIndexBuffer() 
		{
			return nullptr;
		}

		/*!-----------------------------------------------------------
		//	@brief		ストライド取得
		//	@return		ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual const u32 GetStride() const 
		{
			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const override 
		{
			return ResourceType::BASE_MESH;
		}

	public:	/// 定義
		enum class TextureType : int
		{
			Diffuse = 0,
			Specular,
			Emissive,
			NormalMap,
			BumpMap,
			Max
		};

	public:
		/// サブセット
		class Subset : public IResource
		{
		public:
			const ResourceType GetType() const override final
			{
				return ResourceType::SUBSET;
			}

			Material material;
			std::vector<u32> index;
			std::array<BaseTexture*, (int)TextureType::Max> pTextures;
		};

	public:
		typedef std::vector<Subset> Subsets;
		Subsets subset;
	};


	/*!-----------------------------------------------------------
	//	@class  StMesh
	//	@brief  静的メッシュクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class StMesh : public BaseMesh
	{
	public:
		StMesh(){};
		~StMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ取得
		//	@return		頂点バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer** GetVertexBuffer() override final
		{
			return this->geometry.GetVertexBufferPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックスバッファ取得
		//	@return		インデックスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer* GetIndexBuffer() final
		{
			return this->geometry.pGpuIndexBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		const ResourceType GetType() const override
		{
			return ResourceType::ST_MESH;
		}

		/*!-----------------------------------------------------------
		//	@brief		ストライド取得
		//	@return		ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetStride() const override final
		{
			return sizeof(StVertex);
		}

	public:
		StMeshGeometry geometry;
	};


	/*!-----------------------------------------------------------
	//	@class  StaMesh
	//	@brief  静的アニメーションメッシュクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class StaMesh : public StMesh
	{
	public:
		StaMesh(){};
		~StaMesh() final{};

		/*!-----------------------------------------------------------
		//	@brief		メッシュインデックスバッファ取得
		//	@return		メッシュインデックスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		ID3D11Buffer** GetMeshIndexBuffer()
		{
			return this->meshIndexBuffer.GetComPtrPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		アニメーションインデックスのサイズ取得
		//  @return     アニメーションインデックスのサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		const size_t GetMeshIndexSize() const 
		{
			return sizeof(int)*this->meshIndex.size();
		}

		/*!-----------------------------------------------------------
		//	@brief		アニメーションインデックスのポインタ取得
		//  @return     メッシュIDポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		void* GetMeshIndexData()
		{
			return this->meshIndex.data();
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			return ResourceType::STA_MESH;
		}

	public:
		std::vector<s32> meshIndex;
		SmartComPtr<ID3D11Buffer> meshIndexBuffer;
	};


	/*!-----------------------------------------------------------
	//	@class  SkMesh
	//	@brief  動的メッシュクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class SkMesh : public BaseMesh
	{
	public:
		SkMesh(){};
		~SkMesh(){};

		/*!-----------------------------------------------------------
		//	@brief		頂点バッファ取得
		//	@return		頂点バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer** GetVertexBuffer() override final 
		{
			return this->geometry.GetVertexBufferPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		インデックスバッファ取得
		//	@return		インデックスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		ID3D11Buffer* GetIndexBuffer() override final
		{
			return this->geometry.pGpuIndexBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			return ResourceType::ST_MESH;
		}

		/*!-----------------------------------------------------------
		//	@brief		ストライド取得
		//	@return		ストライド
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/05
		------------------------------------------------------------*/
		const u32 GetStride() const override final
		{
			return sizeof(SkVertex);
		}

	public:
		SkMeshGeometry geometry;
	};
}

#endif // _Include_Mesh_hpp_
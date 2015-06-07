#if 1
/*************************************************************
*	@file   	FbxLoader.hpp
*	@brief  	FbxLoaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/28
*************************************************************/

#ifndef _Include_FbxLoader_hpp_	// インクルードガード
#define _Include_FbxLoader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<fbxsdk.h>
#include<string>
#include<list>
#include<map>
#include<vector>
#include<array>
#include<Windows.h>

#ifdef UNICODE
typedef std::wstring fbxString;
typedef wchar_t fbxChar;
#else
typedef std::string fbxString;
typedef char ffbxChar;
#endif

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libfbxsdk.lib")

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  FbxLoader
	//	@brief  Fbxローダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/28
	------------------------------------------------------------*/
	class FbxLoader
	{
	public:
		enum Type{
			Mesh = 0x1,
			Motion = 0x2
		};

	public:
		FbxLoader();
		~FbxLoader();

		/*!-----------------------------------------------------------
		//	@brief		読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_loadType	読み込みタイプ[省略可]
		//	@return		true：読み込み成功　false：読み込み失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		bool Load(const fbxString _filePath, int _loadType = Type::Mesh | Type::Motion);

	private:
		struct float2{
			float x, y;
		};

		struct float3{
			float x, y, z;
		};

		struct Vertex{
			union{
				struct{
					float	x, y, z;
					float	nx, ny, nz;
					float	u, v;
				};
				struct{
					float baseData[8];
				};
			};
			union{
				struct{
					float	weight[3];
					int		index[4];
				};
				struct{
					float skinData[7];
				};
			};
		};

		struct FbxMaterial
		{
			std::string name;					///< 名前
			std::array<float, 4>	diffuse;		///< ディフューズ
			std::array<float, 3>	ambient;		///< アンビエント
			std::array<float, 3>	specular;	///< スペキュラ
			std::array<float, 3>	emissive;	///< エミッシブ
			std::string diffuseTexPath;		///< ディフューズのテクスチャパス
			std::string specularTexPath;	///< スペキュラのテクスチャ
			std::string emissiveTexPath;	///< エミッシブのテクスチャパス
			std::string normalTexPath;		///< ノーマルマップのテクスチャパス
			std::string bumpTexPath;		///< バンプマップのテクスチャパス
			unsigned long indexOffset;		///< インデックスオフセット
			unsigned long indexCount;		///< インデックス数
			unsigned long faceCount;		///< ポリゴン数(三角形)
		};

		struct BuildMesh{
			std::vector<Vertex> vertex;
			std::vector<DWORD> index;
			std::vector<DWORD> meshIndex;
			std::vector<FbxMaterial> material;
		};

		struct Float4x4{
			union{
				struct{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				struct{
					float m[16];
				};
			};
		};

		struct Frame{
			std::vector<Float4x4> mtx;
		};

		struct Animation{
			int beginFrame;				///< アニメーションの開始フレーム
			int endFrame;				///< アニメーションの終了フレーム
			int frameCount;				///< フレーム数
			std::vector<Frame> frame;	///< フレーム情報
		};

		struct MeshAnimation{
			FbxNode* pNode;					///< ノードポインタ
			DWORD materialId;				///< マテリアルID
		};

	public:
		BuildMesh buildMesh;				///< 構築後のメッシュ
		std::vector<Animation> animation;	///< アニメーション情報
		int numMesh;						///< メッシュ数
		BOOL enableAnimation;				///< アニメーションの有無
	private:
		typedef std::vector<DWORD> IndexBuffer;
		typedef std::vector<Vertex> VertexBuffer;
		typedef std::vector<DWORD> MeshIdBuffer;
		FbxManager* lSdkManager;
		FbxIOSettings* ios;
		FbxImporter* lImporter;
		FbxScene* lScene;
		DWORD sumVertexCount;
		DWORD sumIndexCount;
		int materialCount;
		int nowMaterialId;
		int nowMeshId;
		std::map<DWORD, IndexBuffer> indexBuffer;		///< マテリアルごとのインデックスデータ
		std::map<DWORD, VertexBuffer> vertexBuffer;		///< マテリアルごとの頂点データ
		std::map<DWORD, VertexBuffer> uvVertexBuffer;	///< UVベースの頂点バッファ
		std::map<DWORD, MeshIdBuffer> meshIdBuffer;		///< メッシュIDバッファ
		std::map<DWORD, MeshIdBuffer> uvMeshIdBuffer;	///< UVベースのメッシュIDバッファ
		std::vector<DWORD> vertexCount;					///< マテリアルごとの頂点数
		std::vector<DWORD> uvVertexCount;				///< UV頂点数
		std::vector<MeshAnimation> meshAnimationInfo;	///< メッシュアニメーション情報
		std::vector<FbxMaterial> materialBuffer;		///< マテリアル

	private:
		/*!-----------------------------------------------------------
		//	@brief		フォン情報読み込み
		//	@param[out]	_pOut		出力するマテリアル
		//	@param[in]	_pMaterial	マテリアル
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void ReadPhongMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		ランバート情報読み込み
		//	@param[out]	_pOut		出力するマテリアル
		//	@param[in]	_pMaterial	マテリアル
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void ReadLambertMaterial(FbxMaterial* _pOut, FbxSurfaceMaterial* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		マテリアル取得
		//	@param[in]	_node	ノード
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void GetMaterial(FbxNode* _node);

		/*!-----------------------------------------------------------
		//	@brief		メッシュ取得
		//	@param[in]	_node	ノード
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetMesh(FbxNode* _node);

		/*!-----------------------------------------------------------
		//	@brief		ジオメトリ情報取得
		//	@param[in]	_mesh	メッシュ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void GetGeometry(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		頂点取得
		//	@param[in]	_mesh	メッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetVertex(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		法線取得
		//	@param[in]	_mesh	メッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetNormal(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		UV取得
		//	@param[in]	_mesh	メッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/28
		------------------------------------------------------------*/
		void GetVertexUV(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		ウェイト値取得
		//	@param[in]	_mesh	メッシュポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		void GetWeight(FbxMesh* _mesh);

		/*!-----------------------------------------------------------
		//	@brief		メッシュアニメーション読み込み
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/30
		------------------------------------------------------------*/
		void GetMeshAnimation();

		/*!-----------------------------------------------------------
		//	@brief		メッシュの構築
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/29
		------------------------------------------------------------*/
		void BuildMesh();
	};
}

#endif // _Include_FbxLoader_hpp_

#endif
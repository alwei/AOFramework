#if 0	//objの仕様がバラバラなのでローダーは凍結

/*************************************************************
*	@file   	ObjLoader.hpp
*	@brief  	ObjLoaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/26
*************************************************************/

#ifndef _Include_ObjLoader_hpp_	// インクルードガード
#define _Include_ObjLoader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<vector>

#ifdef UNICODE
typedef std::wstring objString;
typedef wchar_t objChar;
#else
typedef std::objString objString;
typedef char objChar;
#endif


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ObjLoader
	//	@brief  Objローダー
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/26
	------------------------------------------------------------*/
	class ObjLoader
	{
	public:
		ObjLoader();
		~ObjLoader();

		/*!-----------------------------------------------------------
		//	@brief		読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@param[in]	_mtlPath	MTLファイルパス[省略可]
		//	@return		true：読み込み成功　false：読み込み失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		bool Load(const objString _filePath,const objString _mtlPath = _T(""));

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
					float x, y, z;
					float nx, ny, nz;
					float u, v;
				};
				struct{
					float pos[3];
					float n[3];
					float uv[2];
				};
			};
		};

		struct Material{
			std::array<float, 3>	ambient;	///< 環境光
			std::array<float, 4>	diffuse;	///< Diffuse成分
			std::array<float, 4>	specular;	///< 鏡面反射の成分(a:スペキュラ強度）
			std::array<objString,4>	textures;	///< テクスチャ配列
			objString name;						///< マテリアル名
		};

	private:
		std::vector<float3> positionList;
		std::vector<float3> normalList;
		std::vector<float2> texcoordList;
		std::vector<Vertex> vertexList;
		std::vector<DWORD>	indexList;
		std::vector<Material*> materialList;
		objString directoryPath;
		objString materialName;
		DWORD dwFaceIndex;
		DWORD dwFaceCount;
		DWORD dwCurSubset;
		int iMtlCount;
		BOOL loadFaild;
	private:

		/*!-----------------------------------------------------------
		//	@brief		頂点データ読み込み
		//	@param[in]	_file	ファイルIO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadPosition(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		法線ベクトル読み取り
		//	@param[in]	_file	ファイルIO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadNormal(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ座標読み込み
		//	@param[in]	_file	ファイルIO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadTexcoord(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		フェイス読み込み
		//	@param[in]	_file	ファイルIO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void ReadFace(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		CreateMaterial
		//	@param[in]	_file	ファイルIO
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void CreateMaterial(std::ifstream& _file);

		/*!-----------------------------------------------------------
		//	@brief		バッファチェック
		//	@param[in]	_file	ファイルIO
		//	@param[in]	_buffer	読み込み用のバッファ
		//	@return		true：処理続行　false：処理終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	bool CheckBuffer(std::ifstream& _file, char* _buffer);

		/*!-----------------------------------------------------------
		//	@brief		マテリアル初期化
		//	@param[in]	_pMaterial	初期化マテリアル
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	void InitMaterial(Material* _pMaterial);

		/*!-----------------------------------------------------------
		//	@brief		ジオメトリ読み込み処理
		//	@param[in]	_filePath	ファイルパス
		//	@return		true：処理続行　false：処理終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	bool ReadGeometry(const objString& _filePath);

		/*!-----------------------------------------------------------
		//	@brief		MTLファイルの読み込み
		//	@param[in]	_mtlPath	ファイルパス
		//	@return		true：読み込み成功　false：読み込み失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	bool LoadMtl(const objString& _mtlPath);

		/*!-----------------------------------------------------------
		//	@brief		Mtl読み込み
		//	@param[in]	_file	ファイルIO
		//	@param[in]	_buffer	読み込み用のバッファ
		//	@return		true：処理続行	false：処理終了
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/27
		------------------------------------------------------------*/
		inline	bool ReadMtl(std::ifstream& _file);
	};
}

#endif // _Include_ObjLoader_hpp_

#endif
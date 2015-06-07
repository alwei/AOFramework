/*************************************************************
*	@file   	XFileLoader.hpp
*	@brief  	XFileLoaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/08/18
*************************************************************/

#ifndef _Include_XFileLoader_hpp_	// インクルードガード
#define _Include_XFileLoader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<fstream>
#include<Windows.h>
#include<string>
#include<vector>
#include<algorithm>
#include<functional>
#include<map>
#include<array>

#ifdef UNICODE
typedef std::wstring xString;
typedef wchar_t xChar;
#else
typedef std::string xString;
typedef char xChar;
#endif

namespace EXTRACT_BIT
{
	const long VERTEX	= 0x00000001;
	const long INDEX	= 0x00000002;
	const long NORMAL	= 0x00000004;
	const long MATERIAL	= 0x00000008;
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XFileLoader
	//	@brief  Xファイルローダー
	//	@author	Tatsunori Aoyama
	//	@date	2013/10/19
	------------------------------------------------------------*/
	class XFileLoader
	{
	public:
		struct Vertex{
			union{
				struct{
					float x, y, z;
					float nx, ny, nz;
					float u, v;
				};
				struct{
					float baseData[8];
				};
			};
		};

		struct float3{
			union{
				float f[3];
				struct{
					float x, y, z;
				};
			};
		};

		struct Material
		{
			std::array<float, 4>	diffuse;	///< Diffuse成分
			std::array<float, 4>	specular;	///< 鏡面反射の成分(a:スペキュラ強度）
			std::array<float, 3>	emissive;	///< 自己発光の成分
			std::string texture;			///< テクスチャ配列
			int		indexCount;				///< マテリアルに依存しているインデックス数
		};

		class SortIndex
		{
		public:
			int		index;			///< マテリアルのインデックス番号
			int		indexArray[4];	///< 1要素のインデックス配列
			bool	isQuad;			///< 四角形かどうか？
		public:
			bool	operator < (const SortIndex& inst){
				return (this->index < inst.index);
			}
			bool	operator >(const SortIndex& inst){
				return (this->index > inst.index);
			}
			bool	operator <= (const SortIndex& inst){
				return (this->index <= inst.index);
			}
			bool	operator >= (const SortIndex& inst){
				return (this->index >= inst.index);
			}
			bool	operator == (const SortIndex& inst){
				return (this->index == inst.index);
			}
		};

	public:
		/// コンストラクタ
		XFileLoader();

		/// デストラクタ
		~XFileLoader();

		/*!-----------------------------------------------------------
		//	@brief		読み込み処理
		//	@param[in]	_filePath	ファイルパス
		//	@return		true：抽出成功　false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2013/10/19
		------------------------------------------------------------*/
		bool	Load(const xString _filePath);

		typedef std::vector<INT> SectionIndex;
		std::vector<Vertex> serialVertex;
		std::vector<INT> serialIndex;
		std::vector<SectionIndex> sectionIndex;
		std::vector<Material> material;
		std::vector<SortIndex>	sorter;

	private:
		/*!-----------------------------------------------------------
		//	@brief		stringから頂点データ抽出
		//	@param[in]	_buffer		文字列バッファ
		//	@return		true：抽出成功	false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadVertex(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		テクスチャ座標読み込み
		//	@param[in]	_buffer	文字列バッファ
		//	@return		true：抽出成功　false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		inline	bool	LoadTexcoord(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		stringからマテリアルデータ抽出
		//	@param[in]	_buffer	文字列バッファ
		//	@return		true：抽出成功	false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadMaterial(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		stringから結線データ抽出
		//	@param[in]	_buffer	文字列バッファ
		//	@return		true：抽出成功	false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	LoadIndex(const std::string& _buffer);

		/*!-----------------------------------------------------------
		//	@brief		頂点データから法線を算出
		//	@return		true：抽出成功	false：抽出失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	CreateNormal();

		/*!-----------------------------------------------------------
		//	@brief		インデックスをマテリアルインデックスに基づいてソート格納
		//	@return		true：格納ソート成功	false：失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/18
		------------------------------------------------------------*/
		inline	bool	StoreSortIndex();
	};
}

#endif // _Include_XFileLoader_hpp_
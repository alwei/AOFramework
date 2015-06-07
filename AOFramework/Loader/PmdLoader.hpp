/*************************************************************
*	@file   	PmdLoader.hpp
*	@brief  	PmdLoaderクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/03
*************************************************************/

#ifndef _Include_PMDLoader_hpp_	// インクルードガード
#define _Include_PMDLoader_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<array>

#ifdef UNICODE
typedef std::wstring pmdString;
typedef wchar_t pmdChar;
#else
typedef std::string pmdString;
typedef char pmdChar;
#endif


namespace ao
{
	//------------------------------------------------------------
	// 定数定義
	//------------------------------------------------------------
	const float PMD_MESH_SCALE = 1.0f;

	class Pmd
	{
	public:
#pragma pack(push,1)	//アラインメント制御をオフ

		/*!-----------------------------------------------------------
		//	@struct	PMDHeader
		//	@brief	ヘッダー情報
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Header
		{
			unsigned char	magic[3];
			float			version;
			unsigned char	modelName[20];
			unsigned char	comment[256];
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDVertex
		//	@brief	頂点データ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Vertex
		{
			float			pos[3];			///< 頂点座標
			float			vNormal[3];		///< 法線ベクトル
			float			uv[2];			///< テクスチャ座標
			unsigned short	boneIndex[2];	///< 影響するボーンインデックス
			unsigned char	boneWeight;		///< ボーンのウェイト値
			unsigned char	edgeFlag;		///< エッジフラグ
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDMaterial
		//	@brief	材質データ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Material
		{
			float diffuseColor[3];		///< ディフューズ色
			float alpha;					///< アルファ値
			float specularity;			///< スペキュラ強度
			float specularColor[3];		///< スペキュラカラー
			float mirrorColor[3];			///< ミラーカラー
			unsigned char toonIndex;		///< トゥーンインデックス
			unsigned char edgeFlag;		///< エッジフラグ
			unsigned long indexNum;		///< マテリアル（サブセット）ごとのインデックス数
			char textureFileName[20];		///< テクスチャファイル名
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDBone
		//	@brief	ボーンデータ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Bone
		{
			char			boneName[20];			///< ボーン名
			unsigned short	parentBoneIndex;		///< ない場合は0xFFFF
			unsigned short	tailPosBoneIndex;		///< ない場合は0xFFFF
			unsigned char	boneType;				///< 0:回転 1:回転と移動 2:IK 3:不明 4:IK影響下 5:回転影響下 6:IK接続先 7:非表示 8:捻り 9:回転運動 (8, 9はMMD4.0以降)
			unsigned short	ikParentBoneIndex;	///< ない場合は0
			float			boneHeadPos[3];		///< ボーン先頭座標
		};

		/*!-----------------------------------------------------------
		//	@struct	PMDIKBone
		//	@brief	IKボーン
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct IKBone
		{
			WORD	boneIndex;			///< IKボーン番号（インデックス）
			WORD	effectorBoneIndex;	///< IKボーンの位置に、このボーンを一致させるようにIK処理が行われる
			BYTE	chainLength;		///< IKチェーンの長さ
			WORD	numIterations;		///< 再帰演算回数
			FLOAT	controlWeight;		///< ボーンの単位制限角 1.0->4.0[rad]. また「ひざ」という名前のボーンはX軸方向にしか動かない制限がある
		};
#pragma pack(pop)
	};


	class Vmd
	{
	public:
#pragma pack(push,1)	//アラインメント制御をオフ
		/*!-----------------------------------------------------------
		//	@struct	VMDHeader
		//	@brief	VMD構造体ヘッダ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Header
		{
			char vmdHeader[30];
			char vmdModelName[20];
		};

		/*!-----------------------------------------------------------
		//	@struct	VMDMotion
		//	@brief	VMD構造体
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Motion
		{
			char			boneName[15];		///< 動かすボーンの名前
			unsigned long	frameNo;			///< フレーム番号
			float			location[3];		///< 移動量
			float			localRotation[4];		///< モデルローカル座標系
			unsigned char	interpolation[64];	///<[4][4][4]モーション補間形式
		};
#pragma pack(pop)
	};


	/*!-----------------------------------------------------------
	//	@class  PmdLoader
	//	@brief  PMD読み込みクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/03
	------------------------------------------------------------*/
	class PmdLoader
	{
		struct IKBone
		{
			WORD	boneIndex;			///< IKボーン番号（インデックス）
			WORD	effectorBoneIndex;	///< IKボーンの位置に、このボーンを一致させるようにIK処理が行われる
			BYTE	chainLength;		///< IKチェーンの長さ
			WORD	numIterations;		///< 再帰演算回数
			FLOAT	controlWeight;		///< ボーンの単位制限角 1.0->4.0[rad]. また「ひざ」という名前のボーンはX軸方向にしか動かない制限がある
			std::vector<USHORT>  childBoneIndexVector;	///< 子供IKのインデックスvector
		};

		/*!-----------------------------------------------------------
		//	@struct	VERTEX
		//	@brief	スキンメッシュ用の頂点データ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct Vertex
		{
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

		struct Material
		{
			std::array<float, 4>	diffuse;	///< Diffuse成分
			std::array<float, 4>	specular;	///< 鏡面反射の成分(a:スペキュラ強度）
			std::array<float, 3>	emissive;	///< 自己発光の成分
			std::string texture;			///< テクスチャ配列
			int		faceCount;				///< 面数
			int		indexCount;				///< マテリアルに依存しているインデックス数
		};

		/*!-----------------------------------------------------------
		//	@struct	BoneRelation
		//	@brief	データを運ぶためのボーンデータ
		//	@author	Tatsunori Aoyama
		//	@date	2014/10/03
		------------------------------------------------------------*/
		struct BoneRelation
		{
			char		name[30];		///< ボーン名
			WORD		firstChildID;	///< 第一子ボーンインデックス(なければ0xffff)
			WORD		siblingID;		///< 兄弟ボーンインデックス(なければ0xffff)
			WORD		parentID;		///< 親ボーンインデックス(なければ0xffff)
			float		length;			///< ボーンの長さ
			float		initMtx[4][4];	///< ボーン初期行列
		};

	public:
		PmdLoader();
		~PmdLoader();

		/*!-----------------------------------------------------------
		//	@brief		読み込み
		//	@param[in]	_filePath	ファイルパス
		//	@return		true：読み込み成功　false：読み込み失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		bool Load(const pmdString _filePath);

	public:
		std::vector<Vertex> vertexBuffer;
		std::vector<DWORD> indexBuffer;
		std::vector<Material> materialBuffer;
		std::vector<BoneRelation> bone;
		std::vector<IKBone> ikBone;

	private:

		/*!-----------------------------------------------------------
		//	@brief		ボーンデータ作成
		//	@param[in]	_pOut	出力するボーンデータ
		//	@param[in]	_bone	入力するボーンデータ
		//	@param[in]	_maxNum	ボーン最大数
		//	@param[in]	_index	ボーンインデックス
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	void CreateBone(
			BoneRelation* _pOut,
			const Pmd::Bone* _bone,
			WORD _maxNum,
			WORD _index);
	};
}

#endif // _Include_PMDLoader_hpp_
/*************************************************************
*	@file   	Bone.hpp
*	@brief  	Boneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Bone_hpp_	// インクルードガード
#define _Include_Bone_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"IResource.hpp"
#include"../Math/Math.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BoneRelation
	//	@brief  ボーンデータ運搬用
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BoneRelation
	{
	public:
		BoneRelation(){};
		virtual ~BoneRelation();

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE_RELATION;
		}

	public:
		TCHAR		name[30];		///< ボーン名
		WORD		firstChildID;	///< 第一子ボーンインデックス(なければ0xffff)
		WORD		siblingID;		///< 兄弟ボーンインデックス(なければ0xffff)
		WORD		parentID;		///< 親ボーンインデックス(なければ0xffff)
		float		length;			///< ボーンの長さ
		float		initMtx[4][4];	///< ボーン初期行列
	};

	/*!-----------------------------------------------------------
	//	@class  Bone
	//	@brief  ボーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class Bone
	{
	public:
		Bone(){};
		virtual ~Bone();

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE;
		}
	};

	/*!-----------------------------------------------------------
	//	@class  BoneEx
	//	@brief  拡張ボーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BoneEx
	{
	public:
		BoneEx();
		virtual ~BoneEx();

		/*!-----------------------------------------------------------
		//	@brief		初期姿勢を親から相対姿勢に変換
		//	@param[in]	_me		自身のボーン
		//	@param[in]	_parent	親ボーン
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		static void InitRelativeBone(BoneEx* const _me, const BoneEx* const _parent);

		/*!-----------------------------------------------------------
		//	@brief		親空間にあるボーンをモデルローカル空間に変換
		//	@param[in]	_me		自身のボーン
		//	@param[in]	_parent	親ボーン
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		static void UpdateWorldBone(BoneEx* const _me, const BoneEx* const _parent);

		/*!-----------------------------------------------------------
		//	@brief		親空間基準での姿勢を更新
		//	@param[in]	なし
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		void UpdateBone(const XMFLOAT4X4& _relativeMtx);

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE_EX;
		}

	public:
		WORD		id;				///< ボーンのID
		BYTE		type;			///< ボーンのタイプ
		BOOL		isKnee;			///< ひざかどうか
		string		boneName;		///< ボーン名
		BoneEx*		pFirstChild;	///< 第一子ボーン
		BoneEx*		pSibling;		///< 兄弟ボーン
		BoneEx*		pParent;		///< 親ボーン
		XMFLOAT4X4	initMtx;		///< 初期姿勢行列（ボーンローカル座標系）
		XMFLOAT4X4	offsetMtx;		///< オフセット行列（モデルローカル座標系）
		XMFLOAT4X4	localMtx;		///< 姿勢行列（ボーンローカル座標系：毎F新規作成）
		XMFLOAT4X4	worldMtx;		///< 姿勢行列（ワールド座標系：毎F新規作成）
		XMFLOAT4X4*	pCombArray;		///< 変換行列配列へのポインタ
	};
}

#endif // _Include_Bone_hpp_
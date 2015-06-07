/*************************************************************
*	@file   	IKBone.hpp
*	@brief  	IKBoneクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_IKBone_hpp_	// インクルードガード
#define _Include_IKBone_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<vector>
#include"IResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  IKBone
	//	@brief  IKボーン
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class IKBone : IResource
	{
	public:
		IKBone() :
			boneIndex(0),
			effectorBoneIndex(0),
			chainLength(0),
			numIterations(0),
			controlWeight(0){};
		virtual ~IKBone(){};

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::IK_BONE;
		}

	public:
		WORD	boneIndex;			///< IKボーン番号（インデックス）
		WORD	effectorBoneIndex;	///< IKボーンの位置に、このボーンを一致させるようにIK処理が行われる
		BYTE	chainLength;		///< IKチェーンの長さ
		WORD	numIterations;		///< 再帰演算回数
		FLOAT	controlWeight;		///< ボーンの単位制限角 1.0->4.0[rad]. また「ひざ」という名前のボーンはX軸方向にしか動かない制限がある
		std::vector<USHORT>  childBoneIndexList;	///< 子供IKのインデックスList
	};
}

#endif // _Include_IKBone_hpp_
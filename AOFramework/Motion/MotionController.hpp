/*************************************************************
*	@file   	MotionController.hpp
*	@brief  	MotionControllerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_MotionController_hpp_	// インクルードガード
#define _Include_MotionController_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include<vector>
#include"../Math/Math.hpp"
#include"../Math/MathDefine.hpp"
#include"../Define/SystemDefine.hpp"
#include"../Resource/IResource.hpp"
#include"../Resource/Bone.hpp"
#include"../Resource/IKBone.hpp"

namespace ao
{
	enum class MotionType : int{
		Once,
		Blend,
		Loop,
		Max
	};

	/*!-----------------------------------------------------------
	//	@class  MotionController
	//	@brief  モーション制御クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class MotionController : public IResource
	{
	public:
		MotionController(
			WORD _numBones,
			WORD _numIkBones,
			const BoneRelation* _boneRelation,
			const IKBone* _ikBone,
			const float _frameRate = 60);
		~MotionController();

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const final{
			return ResourceType::MOTION_CTRL;
		}

	private:
		FLOAT playFrameRate;		///< 再生フレームレート
		std::vector< XMFLOAT4X4 >	calcedMatrix;	///< 計算済み行列
		std::vector< XMFLOAT4X4 >	motionMatrix;	///< 算出されたモーション行列
		std::vector< BoneEx >		modelBones;		///< モデルボーン
	private:
		/*!-----------------------------------------------------------
		//	@brief		ボーンの初期化
		//	@param[in]	_idx			インデックス
		//	@param[in]	_boneRelation	ボーンデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		inline void InitBone(int _idx, const BoneRelation* _boneRelation);

		/*!-----------------------------------------------------------
		//	@brief		ボーン構築
		//	@param[in]	_numBones		ボーン数
		//	@param[in]	_boneRelation	ボーンデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		inline void BuildBone(
			WORD _numBones,
			const BoneRelation* _boneRelation);

		/*!-----------------------------------------------------------
		//	@brief		IKボーン構築
		//	@param[in]	_numIkBones	IKボーン数
		//	@param[in]	_ikBones	IKボーンデータ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		inline void BuildIkBone(
			WORD _numIkBones,
			const IKBone* _ikBone);

		MotionController() = delete;
	};

	/*!-----------------------------------------------------------
	//	@class  MotionFactory
	//	@brief  モーションファクトリークラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class MotionFactory
	{
		MotionFactory();
		~MotionFactory();

		/*!-----------------------------------------------------------
		//	@brief		モーション制御クラス作成
		//	@param[in]	なし
		//	@return		モーション制御クラス
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		MotionController* CreateMotionController(
			WORD _numBones,
			WORD _numIkBones,
			const BoneRelation* _boneRelation,
			const IKBone* _ikBone,
			const float _frameRate = 60);

	private:
		std::list<MotionController*> motionCtrlList;
	};
}
#endif // _Include_MotionController_hpp_
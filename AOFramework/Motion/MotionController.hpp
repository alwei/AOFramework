/*************************************************************
*	@file   	MotionController.hpp
*	@brief  	MotionController�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_MotionController_hpp_	// �C���N���[�h�K�[�h
#define _Include_MotionController_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���[�V��������N���X
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
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const final{
			return ResourceType::MOTION_CTRL;
		}

	private:
		FLOAT playFrameRate;		///< �Đ��t���[�����[�g
		std::vector< XMFLOAT4X4 >	calcedMatrix;	///< �v�Z�ςݍs��
		std::vector< XMFLOAT4X4 >	motionMatrix;	///< �Z�o���ꂽ���[�V�����s��
		std::vector< BoneEx >		modelBones;		///< ���f���{�[��
	private:
		/*!-----------------------------------------------------------
		//	@brief		�{�[���̏�����
		//	@param[in]	_idx			�C���f�b�N�X
		//	@param[in]	_boneRelation	�{�[���f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		inline void InitBone(int _idx, const BoneRelation* _boneRelation);

		/*!-----------------------------------------------------------
		//	@brief		�{�[���\�z
		//	@param[in]	_numBones		�{�[����
		//	@param[in]	_boneRelation	�{�[���f�[�^
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		inline void BuildBone(
			WORD _numBones,
			const BoneRelation* _boneRelation);

		/*!-----------------------------------------------------------
		//	@brief		IK�{�[���\�z
		//	@param[in]	_numIkBones	IK�{�[����
		//	@param[in]	_ikBones	IK�{�[���f�[�^
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
	//	@brief  ���[�V�����t�@�N�g���[�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class MotionFactory
	{
		MotionFactory();
		~MotionFactory();

		/*!-----------------------------------------------------------
		//	@brief		���[�V��������N���X�쐬
		//	@param[in]	�Ȃ�
		//	@return		���[�V��������N���X
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
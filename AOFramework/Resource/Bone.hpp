/*************************************************************
*	@file   	Bone.hpp
*	@brief  	Bone�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Bone_hpp_	// �C���N���[�h�K�[�h
#define _Include_Bone_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include"IResource.hpp"
#include"../Math/Math.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BoneRelation
	//	@brief  �{�[���f�[�^�^���p
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BoneRelation
	{
	public:
		BoneRelation(){};
		virtual ~BoneRelation();

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE_RELATION;
		}

	public:
		TCHAR		name[30];		///< �{�[����
		WORD		firstChildID;	///< ���q�{�[���C���f�b�N�X(�Ȃ����0xffff)
		WORD		siblingID;		///< �Z��{�[���C���f�b�N�X(�Ȃ����0xffff)
		WORD		parentID;		///< �e�{�[���C���f�b�N�X(�Ȃ����0xffff)
		float		length;			///< �{�[���̒���
		float		initMtx[4][4];	///< �{�[�������s��
	};

	/*!-----------------------------------------------------------
	//	@class  Bone
	//	@brief  �{�[��
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class Bone
	{
	public:
		Bone(){};
		virtual ~Bone();

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE;
		}
	};

	/*!-----------------------------------------------------------
	//	@class  BoneEx
	//	@brief  �g���{�[��
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class BoneEx
	{
	public:
		BoneEx();
		virtual ~BoneEx();

		/*!-----------------------------------------------------------
		//	@brief		�����p����e���瑊�Ύp���ɕϊ�
		//	@param[in]	_me		���g�̃{�[��
		//	@param[in]	_parent	�e�{�[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		static void InitRelativeBone(BoneEx* const _me, const BoneEx* const _parent);

		/*!-----------------------------------------------------------
		//	@brief		�e��Ԃɂ���{�[�������f�����[�J����Ԃɕϊ�
		//	@param[in]	_me		���g�̃{�[��
		//	@param[in]	_parent	�e�{�[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		static void UpdateWorldBone(BoneEx* const _me, const BoneEx* const _parent);

		/*!-----------------------------------------------------------
		//	@brief		�e��Ԋ�ł̎p�����X�V
		//	@param[in]	�Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		void UpdateBone(const XMFLOAT4X4& _relativeMtx);

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::BONE_EX;
		}

	public:
		WORD		id;				///< �{�[����ID
		BYTE		type;			///< �{�[���̃^�C�v
		BOOL		isKnee;			///< �Ђ����ǂ���
		string		boneName;		///< �{�[����
		BoneEx*		pFirstChild;	///< ���q�{�[��
		BoneEx*		pSibling;		///< �Z��{�[��
		BoneEx*		pParent;		///< �e�{�[��
		XMFLOAT4X4	initMtx;		///< �����p���s��i�{�[�����[�J�����W�n�j
		XMFLOAT4X4	offsetMtx;		///< �I�t�Z�b�g�s��i���f�����[�J�����W�n�j
		XMFLOAT4X4	localMtx;		///< �p���s��i�{�[�����[�J�����W�n�F��F�V�K�쐬�j
		XMFLOAT4X4	worldMtx;		///< �p���s��i���[���h���W�n�F��F�V�K�쐬�j
		XMFLOAT4X4*	pCombArray;		///< �ϊ��s��z��ւ̃|�C���^
	};
}

#endif // _Include_Bone_hpp_
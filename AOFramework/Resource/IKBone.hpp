/*************************************************************
*	@file   	IKBone.hpp
*	@brief  	IKBone�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_IKBone_hpp_	// �C���N���[�h�K�[�h
#define _Include_IKBone_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<vector>
#include"IResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  IKBone
	//	@brief  IK�{�[��
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
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::IK_BONE;
		}

	public:
		WORD	boneIndex;			///< IK�{�[���ԍ��i�C���f�b�N�X�j
		WORD	effectorBoneIndex;	///< IK�{�[���̈ʒu�ɁA���̃{�[������v������悤��IK�������s����
		BYTE	chainLength;		///< IK�`�F�[���̒���
		WORD	numIterations;		///< �ċA���Z��
		FLOAT	controlWeight;		///< �{�[���̒P�ʐ����p 1.0->4.0[rad]. �܂��u�Ђ��v�Ƃ������O�̃{�[����X�������ɂ��������Ȃ�����������
		std::vector<USHORT>  childBoneIndexList;	///< �q��IK�̃C���f�b�N�XList
	};
}

#endif // _Include_IKBone_hpp_
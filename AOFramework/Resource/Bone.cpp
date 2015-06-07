#include"stdafx.hpp"
#include"Bone.hpp"


using namespace ao;


BoneEx::BoneEx() :
id(0),
type(0),
isKnee(FALSE),
boneName(_T("no name")),
pFirstChild(nullptr),
pSibling(nullptr),
pParent(nullptr),
pCombArray(nullptr)
{

}


BoneEx::~BoneEx()
{

}


//------------------------------------------------------------------------------
/**	@brief		�����p����e���瑊�Ύp���ɕϊ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	20:42	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void BoneEx::InitRelativeBone(BoneEx* const _me, const BoneEx* const _parent)
{
	//�q���T��
	if (_me->pFirstChild)
		BoneEx::InitRelativeBone(_me->pFirstChild, _me);

	//�Z��T��
	if (_me->pSibling)
		BoneEx::InitRelativeBone(_me->pSibling, _parent);

	//�����̐e�̃��[�J����ԍ��W�ɕϊ�
	if (_parent)
		MatrixMultiply(&_me->initMtx, _me->initMtx, _parent->offsetMtx);
}


//------------------------------------------------------------------------------
/**	@brief		�e��Ԃɂ���{�[�������f�����[�J����Ԃɕϊ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	21:00	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void BoneEx::UpdateWorldBone(BoneEx* const _me, const BoneEx* const _parent)
{
	//�K�w�I�ɐώZ���ă��f�����[�J����Ԃɕϊ�->���_�����_�ɃI�t�Z�b�g���ĕϊ�
	MatrixMultiply(&_me->worldMtx, _me->localMtx, _parent->worldMtx);
	MatrixMultiply(&_me->pCombArray[_me->id], _me->offsetMtx, _me->worldMtx);

	//�K�w�I�Ɏq��������
	if (_me->pFirstChild)
		BoneEx::UpdateWorldBone(_me->pFirstChild, _parent);

	//�K�w�I�ɌZ�������
	if (_me->pSibling)
		BoneEx::UpdateWorldBone(_me->pSibling, _parent);
}
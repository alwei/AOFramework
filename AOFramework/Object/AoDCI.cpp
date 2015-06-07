#include"stdafx.hpp"
#include"AoDCI.hpp"


u32 AoDCI::mAllInheritanceCount = 0;


AoDCI::AoDCI() : 
mInheritanceCount(0),
mRootClassIndex(0),
mEndClassIndex(0),
mClassIndex(0),
mpClassName(nullptr),
mpRoot(nullptr),
mpParent(nullptr)
{

}


AoDCI::AoDCI(AOLPCSTR pName,AoDCI* pDCI) : 
mInheritanceCount(0),
mRootClassIndex(0),
mEndClassIndex(0),
mClassIndex(0),
mpClassName(nullptr),
mpRoot(nullptr),
mpParent(nullptr)
{
	SetName(pName);
	SetParent(pDCI);

	if (pDCI)
	{
		pDCI->SetRoot(GetRoot());

		//�p���J�E���g++
		++mInheritanceCount;

		//���ʃN���X�C���f�b�N�X��ݒ�
		++mAllInheritanceCount;
		mClassIndex = mAllInheritanceCount;

		//���[�̃N���X�C���f�b�N�X�Ɖ���
		mEndClassIndex = mClassIndex;

		//�e�C���f�b�N�X���L��
		mRootClassIndex = pDCI->mRootClassIndex;

		//���[�̃C���f�b�N�X��e�����ɒʒm
		TransmitEndIndexToRootClass(pDCI, mClassIndex);
	}
	else
	{
		SetRoot(this);

		//���ʃN���X�C���f�b�N�X��ݒ�
		mClassIndex = mAllInheritanceCount;

		//�e�C���f�b�N�X���i�[
		mRootClassIndex = mClassIndex;
	}
}


AoDCI::~AoDCI()
{
	if (mpClassName)
	{
		delete[] mpClassName;
	}
}


//------------------------------------------------------------------------------
/**	@brief		�N���X���ݒ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:27	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool AoDCI::SetName(AOLPCSTR const pName)
{
	if (pName == nullptr)
	{
		return false;
	}

	if (mpClassName)
	{
		delete[] mpClassName;
	}


	size_t bufferSize = ao_strlen(pName) + sizeof(AOCHAR);
	mpClassName = new AOCHAR[bufferSize];

	const size_t t = static_cast<size_t>(ao_sprintf_s(mpClassName, bufferSize, pName));

	if (t != ao_strlen(pName))
	{
		return false;
	}


	return true;
}


//------------------------------------------------------------------------------
/**	@brief		�h����������Root�N���X�����ׂ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:50	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool AoDCI::IsA(const AoDCI* const pClass)
{
	AoDCI* pStartType(this);

	//Root�ɑk��܂ŒT��
	while (pStartType)
	{
		if (pStartType == pClass)
		{
			//�ǂ����p���̓r����������Root�N���X�Ƃ��Č�������
			return true;
		}
		else
		{
			//���̐e�N���X��
			pStartType = pStartType->GetParent();
		}
	}

	//�p������Ă��Ȃ�
	return false;
}


//------------------------------------------------------------------------------
/**	@brief		�h����������Root�N���X�����ׂ� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:50	T.Aoyama	�쐬
//------------------------------------------------------------------------------
bool AoDCI::IsA(AOLPCSTR const pName)
{
	AoDCI* pStartType = this;

	//Root�ɑk��܂ŒT��
	while (pStartType)
	{
		if (ao_strcmp(pStartType->GetName(),pName)==0)
		{
			//�ǂ����p���̓r����������Root�N���X�Ƃ��Č�������
			return true;
		}
		else
		{
			//���̐e�N���X��
			pStartType = pStartType->GetParent();
		}
	}

	//�p������Ă��Ȃ�
	return false;
}


//------------------------------------------------------------------------------
/**	@brief		���S�ȓ��I�^�L���X�g */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:22	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void* AoDCI::Cast(const AoDCI& castToDci)
{
	if (IsA(&castToDci))
	{
		//�L���X�g�ł���N���X�Ȃ̂Ŏ��g�̃|�C���^��n��
		return this;
	}

	//�L���X�g�ł��Ȃ��N���X�Ȃ̂�nullptr��Ԃ�
	return nullptr;
}


#define INHERITANCE_STRING(_str)								\
	for (u32 i = 0; i < count; ++i)								\
	{															\
		ao::debug::PrintfColor(ConsoleColor::H_WHITE, _str);	\
	}		


//------------------------------------------------------------------------------
/**	@brief		�g���[�X */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:28	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void AoDCI::Trace(ConsoleColor::Enum color)
{
	ao::debug::PrintfColor(color, _T("[ Trace %s ]\n\n"), GetName());

	u32 count = 1;
	AoDCI* pStartType = this;

	ao::debug::PrintfColor(color, _T("+---- %s\n"), pStartType->GetName());
	pStartType = pStartType->GetParent();

	//�h����ɂ��ǂ蒅���܂Ń��[�v
	while (pStartType)
	{
		//�h���N���X
		INHERITANCE_STRING(_T("\t"));
		ao::debug::PrintfColor(color, _T("��\n"));

		INHERITANCE_STRING(_T("\t"));
		ao::debug::PrintfColor(color, _T("��+--- %s \n"), pStartType->GetName());

		//���̔h���N���X��
		pStartType = pStartType->GetParent();

		++count;
	}

	//�]���ɒi���i���₷���悤�Ɂj
	ao::debug::PrintfColor(ConsoleColor::H_WHITE, _T("\n"));
}


//------------------------------------------------------------------------------
/**	@brief		�e�N���X�ɖ��[�̃C���f�b�N�X��ʒm */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/05	00:26	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void AoDCI::TransmitEndIndexToRootClass(AoDCI* const pDCI, const u32 endIndex)
{
	if (pDCI)
	{
		pDCI->mEndClassIndex = endIndex;
		TransmitEndIndexToRootClass(pDCI->mpParent, endIndex);
	}

	return;
}
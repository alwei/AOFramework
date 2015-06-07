/*************************************************************
*	@file   	AoDCI.hpp
*	@brief  	AoDCI�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/05/27
*************************************************************/

#ifndef _Include_AoDCI_hpp_	// �C���N���[�h�K�[�h
#define _Include_AoDCI_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"Define\SystemDefine.hpp"
#include"../Debug/DebugSystem.hpp"


/*!-----------------------------------------------------------
//	@class  AoDCI
//	@brief  ���I�^���(Dynamic Class Infomation)
//	@author	Tatsunori Aoyama
//	@date	2015/05/27
------------------------------------------------------------*/
class AoDCI
{
public:
	AoDCI();
	AoDCI(AOLPCSTR pName, AoDCI* _pDCI);
	virtual ~AoDCI();

	/*!-----------------------------------------------------------
	//	@brief		�^�̖��O�ݒ�
	//	@param[in]	pName	�ݒ肷�閼�O
	//	@return		true�F�ݒ萬���@false�F�ݒ莸�s
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline bool SetName(AOLPCSTR const pName);

	/*!-----------------------------------------------------------
	//	@brief		ROOT��AoDCI�ݒ�
	//	@param[in]	pRoot	���[�gDCI
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline void SetRoot(AoDCI* const pRoot)
	{
		mpRoot = pRoot;
	}

	/*!-----------------------------------------------------------
	//	@brief		�e��AoDCI�ݒ�
	//	@param[in]	pParent	�eDCI
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline void SetParent(AoDCI* const pParent)
	{
		mpParent = pParent;
	}

	/*!-----------------------------------------------------------
	//	@brief		�^�̖��O�擾
	//	@return		���O
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline const AOLPSTR GetName()const
	{
		return mpClassName;
	}

	/*!-----------------------------------------------------------
	//	@brief		���[�g�N���X�擾
	//	@return		���[�g�N���X�|�C���^
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline AoDCI* GetRoot()const
	{
		return (mpParent) ? mpParent->GetRoot() : mpRoot;
	}

	/*!-----------------------------------------------------------
	//	@brief		�e�N���X�擾
	//	@return		�e�N���X�|�C���^
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline AoDCI* GetParent()const
	{
		return mpParent;
	}

	/*!-----------------------------------------------------------
	//	@brief		IsA�̍�������
	//	@param[in]	pClass	����N���X��DCI�|�C���^
	//	@return		ture�F�h����������Root�N���X�ł���@false�F�p������Ă��Ȃ�
	//	@author		Tatsunori Aoyama
	//	@date		2015/06/05
	------------------------------------------------------------*/

	/* �s���S
	inline bool IsAFast(const AoDCI& pClass)
	{
		return (mRootClassIndex <= pClass.mClassIndex && pClass.mClassIndex <= mClassIndex);
	}
	*/
	

	/*!-----------------------------------------------------------
	//	@brief		�h����������Root�N���X�����ׂ�
	//	@param[in]	pClass	����N���X��DCI�|�C���^
	//	@return		true�F�h����������Root�N���X�ł��� false�F�p������Ă��Ȃ�
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	bool IsA(const AoDCI* const pClass);

	/*!-----------------------------------------------------------
	//	@brief		�h����������Root�N���X�����ׂ�
	//	@return		true�F�h����������Root�N���X�ł��� false�F�p������Ă��Ȃ�
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	bool IsA(AOLPCSTR const pName);

	/*!-----------------------------------------------------------
	//	@brief		���S�ȓ��I�^�L���X�g
	//	@param[in]	_pCast	�L���X�g�Ώۃ|�C���^
	//	@return		�L���X�g��̃|�C���^ or nullptr�F�p���֌W�ɂȂ�
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	void* Cast(const AoDCI& dci);

	/*!-----------------------------------------------------------
	//	@brief		�R���\�[���Ɍp���֌W���g���[�X
	//	@param[in]	_color	�`�悷�镶���F[�ȗ���]
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	void Trace(ConsoleColor::Enum _color=ConsoleColor::H_WHITE);


	//���Z�q�̃I�[�o�[���[�h
public:
	inline bool operator==(const AoDCI& dci)
	{
		return mClassIndex == dci.mClassIndex;
	}

	inline bool operator!=(const AoDCI& dci)
	{
		return mClassIndex != dci.mClassIndex;
	}

	inline bool operator>=(const AoDCI& dci)
	{
		return IsA(&dci) && dci.mInheritanceCount >= mRootClassIndex;
	}

	inline bool operator<=(const AoDCI& dci)
	{
		return IsA(&dci) && mInheritanceCount <= dci.mInheritanceCount;
	}

	inline bool operator>(const AoDCI& dci)
	{
		return IsA(&dci) && mInheritanceCount > dci.mInheritanceCount;
	}

	inline bool operator<(const AoDCI& dci)
	{
		return IsA(&dci) && mInheritanceCount < dci.mInheritanceCount;
	}

	inline bool operator==(AOLPCSTR const pName)
	{
		return IsA(pName);
	}

	inline bool operator!=(const AOLPCSTR pName)
	{
		return !IsA(pName);
	}

private:
	static u32 mAllInheritanceCount;

	u16 mInheritanceCount;
	u16 mRootClassIndex;
	u16 mEndClassIndex;
	u16 mClassIndex;
	AOLPSTR mpClassName;
	AoDCI* mpRoot;
	AoDCI* mpParent;

private:
	/*!-----------------------------------------------------------
	//	@brief		�e�N���X�ɖ��[�̃C���f�b�N�X��ʒm
	//	@param[in]	pDCI		�e��DCI�|�C���^
	//	@param[in]	endIndex	���[��index
	//	@author		Tatsunori Aoyama
	//	@date		2015/06/05
	------------------------------------------------------------*/
	static void TransmitEndIndexToRootClass(AoDCI* const pDCI, const u32 endIndex);
};


#define AO_ROOT_CLASS_DEFINE	\
	public:						\
		static AoDCI DCI;


#define AO_CLASS_DEFINE(__super__)		\
	public:								\
		static AoDCI DCI;				\
		typedef __super__ Super;

#define AO_ROOT_CLASS_IMPLEMENT(__class__)	AoDCI __class__::DCI(_T(#__class__),nullptr);
#define AO_CLASS_IMPLEMENT(__class__)	AoDCI __class__::DCI(_T(#__class__), &Super::DCI);


#endif // _Include_AoDCI_hpp_
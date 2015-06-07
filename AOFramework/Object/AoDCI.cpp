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

		//継承カウント++
		++mInheritanceCount;

		//共通クラスインデックスを設定
		++mAllInheritanceCount;
		mClassIndex = mAllInheritanceCount;

		//末端のクラスインデックスと仮定
		mEndClassIndex = mClassIndex;

		//親インデックスを記憶
		mRootClassIndex = pDCI->mRootClassIndex;

		//末端のインデックスを親たちに通知
		TransmitEndIndexToRootClass(pDCI, mClassIndex);
	}
	else
	{
		SetRoot(this);

		//共通クラスインデックスを設定
		mClassIndex = mAllInheritanceCount;

		//親インデックスも格納
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
/**	@brief		クラス名設定 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:27	T.Aoyama	作成
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
/**	@brief		派生もしくはRootクラスか調べる */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:50	T.Aoyama	作成
//------------------------------------------------------------------------------
bool AoDCI::IsA(const AoDCI* const pClass)
{
	AoDCI* pStartType(this);

	//Rootに遡るまで探す
	while (pStartType)
	{
		if (pStartType == pClass)
		{
			//どこか継承の途中もしくはRootクラスとして見つかった
			return true;
		}
		else
		{
			//次の親クラスへ
			pStartType = pStartType->GetParent();
		}
	}

	//継承されていない
	return false;
}


//------------------------------------------------------------------------------
/**	@brief		派生もしくはRootクラスか調べる */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	00:50	T.Aoyama	作成
//------------------------------------------------------------------------------
bool AoDCI::IsA(AOLPCSTR const pName)
{
	AoDCI* pStartType = this;

	//Rootに遡るまで探す
	while (pStartType)
	{
		if (ao_strcmp(pStartType->GetName(),pName)==0)
		{
			//どこか継承の途中もしくはRootクラスとして見つかった
			return true;
		}
		else
		{
			//次の親クラスへ
			pStartType = pStartType->GetParent();
		}
	}

	//継承されていない
	return false;
}


//------------------------------------------------------------------------------
/**	@brief		安全な動的型キャスト */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:22	T.Aoyama	作成
//------------------------------------------------------------------------------
void* AoDCI::Cast(const AoDCI& castToDci)
{
	if (IsA(&castToDci))
	{
		//キャストできるクラスなので自身のポインタを渡す
		return this;
	}

	//キャストできないクラスなのでnullptrを返す
	return nullptr;
}


#define INHERITANCE_STRING(_str)								\
	for (u32 i = 0; i < count; ++i)								\
	{															\
		ao::debug::PrintfColor(ConsoleColor::H_WHITE, _str);	\
	}		


//------------------------------------------------------------------------------
/**	@brief		トレース */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/05/27	01:28	T.Aoyama	作成
//------------------------------------------------------------------------------
void AoDCI::Trace(ConsoleColor::Enum color)
{
	ao::debug::PrintfColor(color, _T("[ Trace %s ]\n\n"), GetName());

	u32 count = 1;
	AoDCI* pStartType = this;

	ao::debug::PrintfColor(color, _T("+---- %s\n"), pStartType->GetName());
	pStartType = pStartType->GetParent();

	//派生先にたどり着くまでループ
	while (pStartType)
	{
		//派生クラス
		INHERITANCE_STRING(_T("\t"));
		ao::debug::PrintfColor(color, _T("│\n"));

		INHERITANCE_STRING(_T("\t"));
		ao::debug::PrintfColor(color, _T("└+--- %s \n"), pStartType->GetName());

		//次の派生クラスへ
		pStartType = pStartType->GetParent();

		++count;
	}

	//余分に段落（見やすいように）
	ao::debug::PrintfColor(ConsoleColor::H_WHITE, _T("\n"));
}


//------------------------------------------------------------------------------
/**	@brief		親クラスに末端のインデックスを通知 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2015/06/05	00:26	T.Aoyama	作成
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
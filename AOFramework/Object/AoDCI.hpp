/*************************************************************
*	@file   	AoDCI.hpp
*	@brief  	AoDCIクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/27
*************************************************************/

#ifndef _Include_AoDCI_hpp_	// インクルードガード
#define _Include_AoDCI_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"Define\SystemDefine.hpp"
#include"../Debug/DebugSystem.hpp"


/*!-----------------------------------------------------------
//	@class  AoDCI
//	@brief  動的型情報(Dynamic Class Infomation)
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
	//	@brief		型の名前設定
	//	@param[in]	pName	設定する名前
	//	@return		true：設定成功　false：設定失敗
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline bool SetName(AOLPCSTR const pName);

	/*!-----------------------------------------------------------
	//	@brief		ROOTのAoDCI設定
	//	@param[in]	pRoot	ルートDCI
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline void SetRoot(AoDCI* const pRoot)
	{
		mpRoot = pRoot;
	}

	/*!-----------------------------------------------------------
	//	@brief		親のAoDCI設定
	//	@param[in]	pParent	親DCI
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline void SetParent(AoDCI* const pParent)
	{
		mpParent = pParent;
	}

	/*!-----------------------------------------------------------
	//	@brief		型の名前取得
	//	@return		名前
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline const AOLPSTR GetName()const
	{
		return mpClassName;
	}

	/*!-----------------------------------------------------------
	//	@brief		ルートクラス取得
	//	@return		ルートクラスポインタ
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline AoDCI* GetRoot()const
	{
		return (mpParent) ? mpParent->GetRoot() : mpRoot;
	}

	/*!-----------------------------------------------------------
	//	@brief		親クラス取得
	//	@return		親クラスポインタ
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	inline AoDCI* GetParent()const
	{
		return mpParent;
	}

	/*!-----------------------------------------------------------
	//	@brief		IsAの高速化版
	//	@param[in]	pClass	相手クラスのDCIポインタ
	//	@return		ture：派生もしくはRootクラスである　false：継承されていない
	//	@author		Tatsunori Aoyama
	//	@date		2015/06/05
	------------------------------------------------------------*/

	/* 不完全
	inline bool IsAFast(const AoDCI& pClass)
	{
		return (mRootClassIndex <= pClass.mClassIndex && pClass.mClassIndex <= mClassIndex);
	}
	*/
	

	/*!-----------------------------------------------------------
	//	@brief		派生もしくはRootクラスか調べる
	//	@param[in]	pClass	相手クラスのDCIポインタ
	//	@return		true：派生もしくはRootクラスである false：継承されていない
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	bool IsA(const AoDCI* const pClass);

	/*!-----------------------------------------------------------
	//	@brief		派生もしくはRootクラスか調べる
	//	@return		true：派生もしくはRootクラスである false：継承されていない
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	bool IsA(AOLPCSTR const pName);

	/*!-----------------------------------------------------------
	//	@brief		安全な動的型キャスト
	//	@param[in]	_pCast	キャスト対象ポインタ
	//	@return		キャスト先のポインタ or nullptr：継承関係にない
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	void* Cast(const AoDCI& dci);

	/*!-----------------------------------------------------------
	//	@brief		コンソールに継承関係をトレース
	//	@param[in]	_color	描画する文字色[省略可]
	//	@author		Tatsunori Aoyama
	//	@date		2015/05/27
	------------------------------------------------------------*/
	void Trace(ConsoleColor::Enum _color=ConsoleColor::H_WHITE);


	//演算子のオーバーロード
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
	//	@brief		親クラスに末端のインデックスを通知
	//	@param[in]	pDCI		親のDCIポインタ
	//	@param[in]	endIndex	末端のindex
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
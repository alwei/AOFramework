/*************************************************************
*	@file   	OwnerComposite.hpp
*	@brief  	OwnerCompositeクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/02/28
*************************************************************/

#ifndef _Include_OwnerComposite_hpp_	// インクルードガード
#define _Include_OwnerComposite_hpp_

/*!-----------------------------------------------------------
//	@class  OwnerComposite
//	@brief  コンポジットされる継承用クラス
//	@author	Tatsunori Aoyama
//	@date	2015/02/28
------------------------------------------------------------*/
template<class T>
class OwnerComposite
{
public:
	OwnerComposite(T* _pOwner) : pOwner(_pOwner){};
	virtual ~OwnerComposite(){};

protected:
	T* pOwner;

private:
	OwnerComposite() = delete;
	OwnerComposite(const OwnerComposite&) = delete;
	void operator = (const OwnerComposite&) = delete;
};

#endif // _Include_OwnerComposite_hpp_
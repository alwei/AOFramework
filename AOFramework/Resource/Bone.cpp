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
/**	@brief		初期姿勢を親から相対姿勢に変換 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	20:42	T.Aoyama	作成
//------------------------------------------------------------------------------
void BoneEx::InitRelativeBone(BoneEx* const _me, const BoneEx* const _parent)
{
	//子供探索
	if (_me->pFirstChild)
		BoneEx::InitRelativeBone(_me->pFirstChild, _me);

	//兄弟探索
	if (_me->pSibling)
		BoneEx::InitRelativeBone(_me->pSibling, _parent);

	//自分の親のローカル空間座標に変換
	if (_parent)
		MatrixMultiply(&_me->initMtx, _me->initMtx, _parent->offsetMtx);
}


//------------------------------------------------------------------------------
/**	@brief		親空間にあるボーンをモデルローカル空間に変換 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	21:00	T.Aoyama	作成
//------------------------------------------------------------------------------
void BoneEx::UpdateWorldBone(BoneEx* const _me, const BoneEx* const _parent)
{
	//階層的に積算してモデルローカル空間に変換->頂点を原点にオフセットして変換
	MatrixMultiply(&_me->worldMtx, _me->localMtx, _parent->worldMtx);
	MatrixMultiply(&_me->pCombArray[_me->id], _me->offsetMtx, _me->worldMtx);

	//階層的に子供を処理
	if (_me->pFirstChild)
		BoneEx::UpdateWorldBone(_me->pFirstChild, _parent);

	//階層的に兄弟を処理
	if (_me->pSibling)
		BoneEx::UpdateWorldBone(_me->pSibling, _parent);
}
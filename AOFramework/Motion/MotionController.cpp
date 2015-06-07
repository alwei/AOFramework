#include"stdafx.hpp"
#include"MotionController.hpp"


using namespace ao;


MotionController::MotionController(
	WORD _numBones,
	WORD _numIkBones,
	const BoneRelation* _boneRelation,
	const IKBone* _ikBone,
	const float _frameRate)
{
	this->playFrameRate = _frameRate;
	this->BuildBone(_numBones, _boneRelation);
	this->BuildIkBone(_numIkBones, _ikBone);
}


MotionController::~MotionController()
{

}

//------------------------------------------------------------------------------
/**	@brief		ボーンの初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	21:23	T.Aoyama	作成
//------------------------------------------------------------------------------
inline void MotionController::InitBone(int _idx, const BoneRelation* _boneRelation)
{
	this->modelBones[_idx].id = _idx;
	this->modelBones[_idx].boneName.assign(_boneRelation[_idx].name);
	this->modelBones[_idx].isKnee = (this->modelBones[_idx].boneName.find(_T("ひざ")) != std::string::npos);
	
	if (_boneRelation[_idx].firstChildID != 0xFFFF)
		this->modelBones[_idx].pFirstChild = &this->modelBones[_boneRelation[_idx].firstChildID];
	if (_boneRelation[_idx].siblingID != 0xFFFF)
		this->modelBones[_idx].pSibling = &this->modelBones[_boneRelation[_idx].siblingID];
	if (_boneRelation[_idx].parentID != 0xFFFF)
		this->modelBones[_idx].pParent = &this->modelBones[_boneRelation[_idx].parentID];

	::CopyMemory(&this->modelBones[_idx].initMtx, _boneRelation[_idx].initMtx, sizeof(float) * 16);
	MatrixInverse(&this->modelBones[_idx].offsetMtx, this->modelBones[_idx].initMtx);
	MatrixIdentity(&this->calcedMatrix[_idx]);
	MatrixIdentity(&this->motionMatrix[_idx]);

	this->modelBones[_idx].pCombArray = &this->calcedMatrix[0];

	//親行列から相対的に行列を初期化（ボーン座標系に変換）
	BoneEx::InitRelativeBone(&this->modelBones[0], nullptr);
}


//------------------------------------------------------------------------------
/**	@brief		ボーン構築 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	20:05	T.Aoyama	作成
//------------------------------------------------------------------------------
inline void MotionController::BuildBone(
	WORD _numBones,
	const BoneRelation* _boneRelation)
{
	this->calcedMatrix.resize(_numBones);
	this->motionMatrix.resize(_numBones);
	this->modelBones.resize(_numBones);

	for (int i = 0; i < _numBones; ++i)
		this->InitBone(i,_boneRelation);
}


//------------------------------------------------------------------------------
/**	@brief		IKボーン構築 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	20:22	T.Aoyama	作成
//------------------------------------------------------------------------------
inline void MotionController::BuildIkBone(
	WORD _numIkBones,
	const IKBone* _ikBone)
{
	// TODO: IKボーンの初期化
}



MotionFactory::MotionFactory()
{

}


MotionFactory::~MotionFactory()
{
	for (auto& it : this->motionCtrlList)
		delete it;
}


//------------------------------------------------------------------------------
/**	@brief		モーション制御クラスの作成 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/04	22:04	T.Aoyama	作成
//------------------------------------------------------------------------------
MotionController* MotionFactory::CreateMotionController(
	WORD _numBones,
	WORD _numIkBones,
	const BoneRelation* _boneRelation,
	const IKBone* _ikBone,
	const float _frameRate)
{
	MotionController* pCtrl = new MotionController(
		_numBones,
		_numIkBones,
		_boneRelation,
		_ikBone,
		_frameRate);

	this->motionCtrlList.push_back(pCtrl);
	return pCtrl;
}
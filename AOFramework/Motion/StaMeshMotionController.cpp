#include"stdafx.hpp"
#include"StaMeshMotionController.hpp"


using namespace ao;


StaMeshMotionController::StaMeshMotionController() :
enable(FALSE),
numMesh(0),
nowFrame(0),
nowMotionIndex(0),
motionType(MotionType::Once)
{

}


StaMeshMotionController::~StaMeshMotionController()
{
	for (auto& mIt : this->motionData.list)
	{
		for (auto& fIt : mIt.frame)
			fIt.mtx.clear();

		mIt.frame.clear();
	}

	this->motionData.list.clear();
}


//------------------------------------------------------------------------------
/**	@brief		初期化 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	14:06	T.Aoyama	作成
//------------------------------------------------------------------------------
HRESULT StaMeshMotionController::Initialize()
{
	this->enable = FALSE;
	this->numMesh = 0;
	this->nowFrame = 0;
	this->nowMotionIndex = 0;
	this->motionType = MotionType::Once;
	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/10/31	18:26	T.Aoyama	作成
//------------------------------------------------------------------------------
void StaMeshMotionController::Update()
{
	if (this->enable == FALSE)
		return;

	for (int i = 0; i < this->numMesh; ++i){
		auto& mtx = this->motionData.list[this->nowMotionIndex].frame[this->nowFrame].mtx[i];
		ao::MatrixTranspose(&this->mapData[i], mtx);
	}

	int maxFrame = this->motionData.list[this->nowMotionIndex].frameCount;

	switch (this->motionType)
	{
	case MotionType::Once:
		this->nowFrame = min(this->nowFrame + 1, maxFrame - 1);
		break;
	case MotionType::Loop:
		this->nowFrame = (this->nowFrame + 1) % maxFrame;
		break;
	case MotionType::InverceOnce:
		this->nowFrame = max(0, this->nowFrame - 1);
		break;
	case MotionType::InverceLoop:
		this->nowFrame = (this->nowFrame + (maxFrame - 1)) % maxFrame;
		break;
	case MotionType::SeamlessOnce:
		this->nowFrame = min((this->nowFrame + 1), maxFrame - 1);
		if (this->nowFrame == maxFrame - 1)
		{
			this->nowMotionIndex = this->nextMotionIndex;
			this->motionType = MotionType::Once;
			this->nowFrame = 0;
		}
		break;
	case MotionType::SeamlessLoop:
		this->nowFrame = min(this->nowFrame + 1, maxFrame - 1);
		if (this->nowFrame == maxFrame - 1)
		{
			this->nowMotionIndex = this->nextMotionIndex;
			this->motionType = MotionType::Loop;
			this->nowFrame = 0;
		}
		break;
	case MotionType::SeamlessInverceOnce:
		this->nowFrame = max(0,this->nowFrame -1);
		if (this->nowFrame <= 0)
		{
			this->nowMotionIndex = this->nextMotionIndex;
			this->motionType = MotionType::InverceOnce;
			this->nowFrame = this->motionData.list[this->nowMotionIndex].frameCount-1;
		}
		break;
	case MotionType::SeamlessInverceLoop:
		this->nowFrame = max(0, this->nowFrame - 1);
		if (this->nowFrame <= 0)
		{
			this->nowMotionIndex = this->nextMotionIndex;
			this->motionType = MotionType::InverceLoop;
			this->nowFrame = this->motionData.list[this->nowMotionIndex].frameCount - 1;
		}
		break;
	}
}


//------------------------------------------------------------------------------
/**	@brief		モーションの変更 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/01	12:55	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL StaMeshMotionController::ChangeMotion(int _index, MotionType _type)
{
	if (_index > (int)this->motionData.list.size())
		return FALSE;

	this->motionType = _type;
	if (this->nowMotionIndex == _index)
	{
		switch (_type)
		{
		case ao::MotionType::SeamlessOnce:
			this->motionType = MotionType::Once;
			break;
		case ao::MotionType::SeamlessLoop:
			this->motionType = MotionType::Loop;
			break;
		case ao::MotionType::SeamlessInverceOnce:
			this->motionType = MotionType::InverceOnce;
			break;
		case ao::MotionType::SeamlessInverceLoop:
			this->motionType = MotionType::InverceLoop;
			break;
		}
	}

	switch (this->motionType)
	{
	case ao::MotionType::Stop:
		this->nowMotionIndex = _index;
		break;
	case ao::MotionType::Once:
	case ao::MotionType::Loop:
		this->nowMotionIndex = _index;
		this->nowFrame = 0;
		break;
	case ao::MotionType::InverceOnce:
	case ao::MotionType::InverceLoop:
		this->nowMotionIndex = _index;
		this->nowFrame = this->motionData.list[_index].frameCount-1;
		break;
	case ao::MotionType::SeamlessOnce:
	case ao::MotionType::SeamlessLoop:
	case ao::MotionType::SeamlessInverceOnce:
	case ao::MotionType::SeamlessInverceLoop:
		this->nextMotionIndex = _index;
		break;
	}


	for (int i = 0; i < numMesh; ++i){
		auto& mtx = this->motionData.list[this->nowMotionIndex].frame[this->nowFrame].mtx[i];
		ao::MatrixTranspose(&this->mapData[i], mtx);
	}

	return TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		計算された行列データの取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/11/01	13:03	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL StaMeshMotionController::GetMoitonData(XMFLOAT4X4* _pOut, const size_t _size)
{
	if (_size > this->mapData.size() * sizeof(XMFLOAT4X4))
		return FALSE;

	::CopyMemory(_pOut, this->mapData.data(), _size);

	return TRUE;
}


//------------------------------------------------------------------------------
/**	@brief		モーションが再生中かどうかチェック */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/05	14:37	T.Aoyama	作成
//------------------------------------------------------------------------------
BOOL StaMeshMotionController::IsPlayNow()
{
	auto nowMotion = this->motionData.list[this->nowMotionIndex];
	return (this->nowFrame != 0 && this->nowFrame != nowMotion.frameCount);
}


//------------------------------------------------------------------------------
/**	@brief		再生処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/06	21:36	T.Aoyama	作成
//------------------------------------------------------------------------------
void StaMeshMotionController::Play(int _setFrame)
{
	int maxFrame = this->motionData.list[this->nowMotionIndex].frameCount;
	if (_setFrame < -1 ||_setFrame > maxFrame)
		return;

	if (_setFrame != -1)
		this->nowFrame = _setFrame;
}


//------------------------------------------------------------------------------
/**	@brief		停止処理 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/12/06	21:38	T.Aoyama	作成
//------------------------------------------------------------------------------
void StaMeshMotionController::Stop(int _setFrame)
{
	if (_setFrame != -1)
		this->nowFrame = _setFrame;

	this->motionType = MotionType::Stop;
}
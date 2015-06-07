#include"stdafx.hpp"
#include"FpsController.hpp"
#include"CountTime.hpp"


using namespace ao;


FpsController::FpsController() :
hThread(nullptr),
threadId(0),
frameRate(60),
ms1Frame(0.0166666),
overTime(0),
aveFps(0),
isKilled(false),
bUpdate(false)
{

}


FpsController::~FpsController()
{
	this->Destroy();
}


//------------------------------------------------------------------------------
/**	@brief		fps�p�̃T�u�X���b�h�쐬 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	02:55	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT FpsController::Create(DWORD _frameRate)
{
	this->frameRate = _frameRate;
	this->ms1Frame = (1.0 / _frameRate);

	//�ȑO�̃X���b�h��j��
	this->Destroy();

	//�t���O������
	this->isKilled = false;
	this->bUpdate = false;

	//�J�E���g�l������
	this->aveFps = 0;
	this->frameCount = 0;

	//�X���b�h�쐬
	this->hThread = (HANDLE)::_beginthreadex(
		nullptr,
		0,
		FpsController::CallProc,
		this,
		0,
		&this->threadId);

	//�G���[�`�F�b�N
	if (this->hThread == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�T�u�X���b�h�̔j�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	02:57	T.Aoyama	�쐬
//------------------------------------------------------------------------------
HRESULT FpsController::Destroy()
{
	BOOL isSafeEnd = 0;

	//���ɔj������Ă���΃X�L�b�v
	if (this->hThread == nullptr)
	{
		return S_OK;
	}

	this->isKilled = true;
	::WaitForSingleObject(this->hThread, INFINITE);
	isSafeEnd = ::CloseHandle(this->hThread);
	this->hThread = nullptr;

	if (isSafeEnd == 0)
	{
		return E_FAIL;
	}

	return S_OK;
}


//------------------------------------------------------------------------------
/**	@brief		�`�F�b�N�X�^�[�g */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:07	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void FpsController::Begin()
{
	//�N���b�N����J�n
	this->_1FTimer.Begin();
	if (this->frameCount == 0)
	{
		this->loopTimer.Begin();
	}
}


//------------------------------------------------------------------------------
/**	@brief		�v���I�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:09	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void FpsController::End()
{
	this->_1FTimer.End();

	//�f���^�^�C���擾
	this->deltaTime = max(this->ms1Frame, this->_1FTimer.GetMilliSeconds<double>() / 1000.0f);

	//�t���[�����[�g���̃I�[�o�[���Z�o
	this->overTime = this->deltaTime + this->overTime;
	this->overTime = max(0, this->overTime - this->ms1Frame);


	//�f���^�C���l�擾
	this->deltaFix = 1.0 + this->overTime / this->deltaTime;

	//FPS�v��
	this->CountFps();

	//�X�V�s�\�ɂ���
	this->bUpdate = false;
}


//------------------------------------------------------------------------------
/**	@brief		fps�J�E���g */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:00	T.Aoyama	�쐬
//------------------------------------------------------------------------------
inline	void FpsController::CountFps()
{
	//�����t���[�����t���[�����[�g���~���b�����p�ɔ{��������
	static double divFrame = static_cast<double>(this->frameRate)*1000;

	if (this->frameCount >= this->frameRate)
	{
		this->loopTimer.End();
		this->aveFps = static_cast<double>(divFrame) / this->loopTimer.GetMilliSeconds<double>();
		this->loopTimer.Begin();
		this->frameCount = 0;
	}

	++this->frameCount;
}


//------------------------------------------------------------------------------
/**	@brief		fps�v�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:14	T.Aoyama	�쐬
//------------------------------------------------------------------------------
unsigned FpsController::Process()
{
	static DWORD timeTable[] = { 17, 17, 16 };
	unsigned int mod = _countof(timeTable);
	unsigned int nowTimeTablePos = 0;

	//�I������܂Ń��[�v
	while (!this->isKilled)
	{
		::Sleep(timeTable[nowTimeTablePos]);
		nowTimeTablePos = (nowTimeTablePos + 1) % mod;
		this->bUpdate = true;
	}

	return false;
}


//------------------------------------------------------------------------------
/**	@brief		�T�u�X���b�h�ɏ�������v���V�[�W�� */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/31	03:15	T.Aoyama	�쐬
//------------------------------------------------------------------------------
unsigned WINAPI FpsController::CallProc(void* _argList)
{
	FpsController* p = static_cast<FpsController*>(_argList);
	return p->Process();
}
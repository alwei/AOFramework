#include"stdafx.hpp"
#include"Mouse.hpp"
#include"../Debug/DebugSystem.hpp"


using namespace ao;


Mouse::Mouse()
{
	this->hWnd = nullptr;
	this->isNowActive = TRUE;
	this->nowCursorPos.x = 0;
	this->nowCursorPos.y = 0;
	this->logCursorPos.x = 0;
	this->logCursorPos.y = 0;
	this->inputState = 0;
}


Mouse::~Mouse()
{

}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/02	23:52	T.Aoyama	作成
//------------------------------------------------------------------------------
void Mouse::Update(CONST HWND _hWnd)
{
	//カーソル座標の更新
	this->UpdateCursorPos();

	HWND hFGWnd = GetForegroundWindow();
	if (_hWnd == nullptr)
		this->isNowActive = (this->hWnd != hFGWnd) ? FALSE : TRUE;
	else
		this->isNowActive = (_hWnd != hFGWnd) ? FALSE : TRUE;

	//アクティブでなければreturn
	if (this->isNowActive == FALSE)
		return;

	//過去の入力状態にシフト
	this->inputState = (this->inputState << 12) & 0xF000;

	//現在の入力状態取得
	this->inputState |= (GetKeyState(VK_LBUTTON) & 0x80) >> 5;
	this->inputState |= (GetKeyState(VK_MBUTTON) & 0x80) >> 6;
	this->inputState |= (GetKeyState(VK_RBUTTON) & 0x80) >> 7;

	//トリガーリリース検出
	this->inputState |= (((this->inputState << 4) ^ (this->inputState >> 8))&(this->inputState << 4)) & 0x0070;
	this->inputState |= (((this->inputState << 8) ^ (this->inputState >> 4))&(this->inputState >> 4)) & 0x0700;
}


//------------------------------------------------------------------------------
/**	@brief		マウスの相対座標取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/02	23:59	T.Aoyama	作成
//------------------------------------------------------------------------------
void Mouse::GetRelativeCursorPos(LPPOINT _pPos)
{
	if (_pPos == nullptr)
		return;
	_pPos->x = this->nowCursorPos.x - this->logCursorPos.x;
	_pPos->y = this->nowCursorPos.y - this->logCursorPos.y;
}


//------------------------------------------------------------------------------
/**	@brief		マウスの絶対座標取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:00	T.Aoyama	作成
//------------------------------------------------------------------------------
CONST LPPOINT Mouse::GetAbsoluteCursorPos(LPPOINT _pPos)
{
	this->UpdateCursorPos();

	if (_pPos == nullptr)
		return &this->nowCursorPos;

	_pPos->x = this->nowCursorPos.x;
	_pPos->y = this->nowCursorPos.y;

	return _pPos;
}


//------------------------------------------------------------------------------
/**	@brief		マウスの中央ホイールの回転角度取得 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:02	T.Aoyama	作成
//------------------------------------------------------------------------------
SHORT Mouse::GetWheelRotation()
{
	if (this->zQueue.empty())
		return 0;

	SHORT rtn = this->zQueue.back();
	this->zQueue.pop();

	return rtn;
}


//------------------------------------------------------------------------------
/**	@brief		マウス座標の更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/02	23:54	T.Aoyama	作成
//------------------------------------------------------------------------------
inline	void Mouse::UpdateCursorPos()
{
	this->logCursorPos.x = this->nowCursorPos.x;
	this->logCursorPos.y = this->nowCursorPos.y;

	::GetCursorPos(&this->nowCursorPos);
	ScreenToClient(this->hWnd, &this->nowCursorPos);
}
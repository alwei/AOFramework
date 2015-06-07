#include"stdafx.hpp"
#include"Keyboard.hpp"


using namespace ao;


Keyboard::Keyboard()
{
	SecureZeroMemory(&keyData, sizeof(InputKeyboard));
}


Keyboard::~Keyboard()
{

}


//------------------------------------------------------------------------------
/**	@brief		更新 */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:18	T.Aoyama	作成
//------------------------------------------------------------------------------
void Keyboard::Update()
{
	BYTE	keyLogState[MAX_KEY_CODE];

	//過去のキー情報をバーストコピー
	::memcpy(keyLogState, keyData.prsState, sizeof(BYTE)*MAX_KEY_CODE);
	SecureZeroMemory(keyData.prsState, sizeof(BYTE)*MAX_KEY_CODE);

	//キーボードの入力状態を取得
	GetKeyboardState(keyData.prsState);

	//全キー情報チェック
	for (int i = 0; i < MAX_KEY_CODE; i++)
	{
		//トリガー、リリースの判定
		this->keyData.prsState[i] = this->keyData.prsState[i] & 0x80;
		this->keyData.trgState[i] = (this->keyData.prsState[i] ^ keyLogState[i]) & this->keyData.prsState[i];
		this->keyData.rlsState[i] = (this->keyData.prsState[i] ^ keyLogState[i]) & keyLogState[i];
	}
}
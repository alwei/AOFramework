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
/**	@brief		�X�V */
//------------------------------------------------------------------------------
//	Date		Time	Author		Summary
//	2014/07/03	00:18	T.Aoyama	�쐬
//------------------------------------------------------------------------------
void Keyboard::Update()
{
	BYTE	keyLogState[MAX_KEY_CODE];

	//�ߋ��̃L�[�����o�[�X�g�R�s�[
	::memcpy(keyLogState, keyData.prsState, sizeof(BYTE)*MAX_KEY_CODE);
	SecureZeroMemory(keyData.prsState, sizeof(BYTE)*MAX_KEY_CODE);

	//�L�[�{�[�h�̓��͏�Ԃ��擾
	GetKeyboardState(keyData.prsState);

	//�S�L�[���`�F�b�N
	for (int i = 0; i < MAX_KEY_CODE; i++)
	{
		//�g���K�[�A�����[�X�̔���
		this->keyData.prsState[i] = this->keyData.prsState[i] & 0x80;
		this->keyData.trgState[i] = (this->keyData.prsState[i] ^ keyLogState[i]) & this->keyData.prsState[i];
		this->keyData.rlsState[i] = (this->keyData.prsState[i] ^ keyLogState[i]) & keyLogState[i];
	}
}
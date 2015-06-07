/*************************************************************
*	@file   	Keyboard.hpp
*	@brief  	Keyboardクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/07/23
*************************************************************/

#ifndef _Include_CKeyboard_hpp_	// インクルードガード
#define _Include_CKeyboard_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include"../Utility/NonCopyable.hpp"

enum class KeyCode : DWORD
{
	A = 'A',
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	F1 = VK_F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,
	TAB = VK_TAB,
	SPACE = VK_SPACE,
	RETURN = VK_RETURN,
	ESCAPE = VK_ESCAPE,
	BACK = VK_BACK,
	LCONTROL = VK_LCONTROL,
	RCONTROL = VK_RCONTROL,
	LMENU = VK_LMENU,
	RMENU = VK_RMENU,
	LSHIFT = VK_LSHIFT,
	RSHIFT = VK_RSHIFT,
	DEL = VK_DELETE
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Keyboard
	//	@brief  キーボード入力管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/23
	------------------------------------------------------------*/
	class Keyboard : private NonCopyable
	{
	public:
		~Keyboard();

		/*!-----------------------------------------------------------
		//	@brief		シングルトンによる実体呼び出し
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		static	Keyboard*	GetInstance(){
			static	Keyboard	singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		プレスキー取得
		//	@param[in]	_keyCode	キーコード
		//	@return		0以外：押されている　0：押されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsKey(const KeyCode& _keyCode){
			return keyData.prsState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		トリガーキー取得
		//	@param[in]	_keyCode	キーコード
		//	@return		0以外：押された　0：押されていなかった
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetTrgKey(const KeyCode& _keyCode){
			return keyData.trgState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		リリースキー取得
		//	@param[in]	_keyCode	キーコード
		//	@return		0以外：離された　0：離されなかった
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetRlsKey(const KeyCode& _keyCode){
			return keyData.rlsState[static_cast<DWORD>(_keyCode)];
		}

		/*!-----------------------------------------------------------
		//	@brief		キーボードの状態更新メソッド
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		void	Update();

	private:	/// 定義
		static	const int	MAX_KEY_CODE = 256;

		/*!-----------------------------------------------------------
		//	@struct	KeyboardState
		//	@brief	キーボード格納情報構造体
		//	@author	Tatsunori Aoyama
		//	@date	2013/07/23
		------------------------------------------------------------*/
		typedef struct _InputKeyboard
		{
			BYTE	prsState[256];
			BYTE	trgState[256];
			BYTE	rlsState[256];
		}InputKeyboard;

	private:
		InputKeyboard	keyData;	/// キーボード格納情報

	private:
		Keyboard();
	};

}

#endif // _Include_CKeyboard_hpp_
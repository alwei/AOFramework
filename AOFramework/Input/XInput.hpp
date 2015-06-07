/*************************************************************
*	@file   	XInput.hppao
*	@brief  	XInputクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/07/22
*************************************************************/

#ifndef _Include_XInput_hpp_	// インクルードガード
#define _Include_XInput_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<winnt.h>
#include<XInput.h>
#include"../Utility/NonCopyable.hpp"

//1.4だとWindow8しか実行できないため
//一つ前のバージョンのライブラリを使用
#pragma comment(lib,"Xinput9_1_0.lib")


enum class JoypadId : DWORD
{
	UP = XINPUT_GAMEPAD_DPAD_UP,
	DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
	LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
	RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LT = XINPUT_GAMEPAD_LEFT_THUMB,
	RT = XINPUT_GAMEPAD_RIGHT_THUMB,
	LS = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RS = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  XInput
	//	@brief  ジョイスティック入力クラス
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/22
	------------------------------------------------------------*/
	class XInput : private NonCopyable
	{
	public:
		~XInput();

		/*!-----------------------------------------------------------
		//	@brief		シングルトンによる実体呼び出し
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		static	XInput*	GetInstance(){
			static XInput singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		コントローラーの状態更新
		//	@return		ERROR_SUCCESS：更新成功　それ以外：更新失敗
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/22
		------------------------------------------------------------*/
		DWORD	Update();

		/*!-----------------------------------------------------------
		//	@brief		指定したボタン番号の押下状態取得
		//	@param[in]	_joyId		ジョイスティックID
		//	@param[in]	_buttonId	マスクするボタンID
		//	@return		押下したボタン番号のプレス状態
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetPrsButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].inputState.Gamepad.wButtons)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		指定したボタン番号のトリガー状態取得
		//	@param[in]	_joyId		ジョイスティックID
		//	@param[in]	_buttonId	マスクするボタンID
		//	@return		押下したボタン番号のトリガー状態
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetTrgButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].trgState)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		指定したボタン番号のリリース状態取得
		//	@param[in]	_joyId		ジョイスティックID
		//	@param[in]	_buttonId	マスクするボタンID
		//	@return		押下したボタン番号のリリース状態
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		DWORD	GetRlsButton(BYTE _joyId, const JoypadId& _buttonId){
			return (this->joyData[_joyId].rlsState)&static_cast<DWORD>(_buttonId);
		}

		/*!-----------------------------------------------------------
		//	@brief		左トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		押し込まれている深さ（0～255）
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsLT(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.bLeftTrigger;
		}

		/*!-----------------------------------------------------------
		//	@brief		左トリガーのトリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		0以外：入力あり　0：入力なし
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetTrgLT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x08);
		}

		/*!-----------------------------------------------------------
		//	@brief		左トリガーのトリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		0以外：入力あり　0：入力なし
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetRlsLT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x02);
		}

		/*!-----------------------------------------------------------
		//	@brief		右トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		押し込まれている深さ（0～255）
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		BYTE	GetPrsRT(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.bRightTrigger;
		}

		/*!-----------------------------------------------------------
		//	@brief		右トリガーのトリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		0以外：入力あり　0：入力なし
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetTrgRT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x04);
		}

		/*!-----------------------------------------------------------
		//	@brief		右トリガーのトリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		0以外：入力あり　0：入力なし
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/03
		------------------------------------------------------------*/
		BYTE	GetRlsRT(BYTE _joyId){
			return (this->joyData[_joyId].trgTR & 0x01);
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのX軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックのX軸取得
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisLX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLX;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのY軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックのY軸取得
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisLY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLY;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックの正規化されたX軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックの正規化されたX軸取得
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeLX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLX / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックの正規化されたY軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックの正規化されたY軸取得
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeLY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbLY / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのX軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された　false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisLX(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0008) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのY軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された　false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisLY(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0004) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのX軸リリース取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された　false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisLX(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0008) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのY軸リリース取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された　false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisLY(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0004) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのX軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		右スティックのX軸の傾き
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisRX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRX;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックのY軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックのY軸の傾き
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		SHORT	GetAxisRY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRY;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックの正規化されたX軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		右スティックの正規化されたX軸の傾き
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeRX(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRX / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		左スティックの正規化されたY軸取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		左スティックの正規化されたY軸の傾き
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/23
		------------------------------------------------------------*/
		FLOAT	GetAxisNormalizeRY(BYTE _joyId){
			return this->joyData[_joyId].inputState.Gamepad.sThumbRY / 32768.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		右スティックのX軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された	false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisRX(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0002) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		右スティックのY軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された	false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetTrgAxisRY(BYTE _joyId){
			return (this->joyData[_joyId].trgAxis & 0x0001) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		右スティックのX軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された	false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisRX(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0002) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		右スティックのY軸トリガー取得
		//	@param[in]	_joyId	ジョイスティックID
		//	@return		true：倒された	false：倒されていない
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		BOOL	GetRlsAxisRY(BYTE _joyId){
			return (this->joyData[_joyId].rlsAxis & 0x0001) != 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		バイブレーションの設定
		//	@param[in]	_joyId			ジョイスティックID
		//	@param[in]	_leftVibRate	左振動の値(0～65535)
		//	@param[in]	_rightVibRate	右振動の値(0～65535)
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/24
		------------------------------------------------------------*/
		void	SetVibration(
			BYTE _joyId,
			USHORT _leftVibRate,
			USHORT _rightVibRate);

	private:	//定義
		static	const int MAX_CONTROLLERS = 4;

		/*!-----------------------------------------------------------
		//	@struct	TXJoyStickJoyStick
		//	@brief	コントローラー格納構造体
		//	@author	Tatsunori Aoyama
		//	@date	2013/07/23
		------------------------------------------------------------*/
		typedef struct _JoyStickData
		{
			DWORD				id;				///< ジョイスティックID
			XINPUT_STATE		inputState;		///< 入力状態
			XINPUT_VIBRATION	vibration;		///< 振動状態
			WORD				prsAxis;		///< 軸入力状態
			BYTE				trgPress;		///< トリガーの入力状態
			BYTE				trgTR;			///< トリガーのトリガーリリース
			WORD				trgAxis;		///< トリガー軸
			WORD				rlsAxis;		///< リリース軸
			WORD				trgState;		///< トリガー状態
			WORD				rlsState;		///< リリース状態
		}JoyStickData;

	private:	/// ジョイスティック格納情報
		JoyStickData	joyData[MAX_CONTROLLERS];	///< ジョイスティックデータ（４つ）

	private:
		XInput();
	};
}

#endif // _Include_XInput_hpp_
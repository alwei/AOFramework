/*************************************************************
*	@file   	Mouse.hpp
*	@brief  	Mouseクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/07/31
*************************************************************/

#ifndef _Include_Mouse_hpp_	// インクルードガード
#define _Include_Mouse_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<queue>
#include"../Utility/NonCopyable.hpp"

class MouseCode
{
public:
	enum class Press : long{
		Left = 0x0000004,
		Middle = 0x0000002,
		Right = 0x0000001
	};

	enum class Trigger : long{
		Left = 0x0000040,
		Middle = 0x0000020,
		Right = 0x0000010
	};

	enum class Release : long{
		Left = 0x0000400,
		Middle = 0x0000200,
		Right = 0x0000100
	};
};


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Mouse
	//	@brief  マウス入力管理クラス
	//	@author	Tatsunori Aoyama
	//	@date	2013/07/31
	------------------------------------------------------------*/
	class Mouse : private NonCopyable
	{
	public:
		~Mouse();

		/*!-----------------------------------------------------------
		//	@brief		シングルトンによる呼び出し
		//	@return		自身の参照
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		static	Mouse*	GetInstance(){
			static	Mouse singleton;
			return &singleton;
		}

		/*!-----------------------------------------------------------
		//	@brief		ウィンドウハンドルセット
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		void	SetWindowHandle(HWND _hWnd){
			this->hWnd = _hWnd;
		}

		/*!-----------------------------------------------------------
		//	@brief		マウスのスクリーン相対座標取得
		//	@param[out]	_pPos	出力するマウスの移動相対座標
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/05
		------------------------------------------------------------*/
		void	GetRelativeCursorPos(LPPOINT _pPos);

		/*!-----------------------------------------------------------
		//	@brief		マウスのスクリーン絶対座標取得
		//	@param[in]	_pPos	出力するマウスのスクリーン絶対座標[省略可]
		//	@return		出力するマウスのスクリーン絶対座標
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		CONST LPPOINT	GetAbsoluteCursorPos(LPPOINT _pPos = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		マウスの中央ホイールの回転角度取得
		//	@return		ホイールの回転角度(-1or1)
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		SHORT	GetWheelRotation();

		/*!-----------------------------------------------------------
		//	@brief		マウスの入力状態取得
		//	@param[in]	_mask	マスクコード
		//	@return		入力状態のビット
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetPressButton(MouseCode::Press _mask){
			return (BYTE)(this->inputState&(int)_mask);
		}

		/*!-----------------------------------------------------------
		//	@brief		マウスの入力状態取得
		//	@param[in]	_mask	マスクコード
		//	@return		入力状態のビット
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetTriggerButton(MouseCode::Trigger _mask){
			return (BYTE)(this->inputState&(int)_mask) >> 4;
		}

		/*!-----------------------------------------------------------
		//	@brief		マウスの入力状態取得
		//	@param[in]	_mask	マスクコード
		//	@return		入力状態のビット
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		BYTE	GetReleaseButton(MouseCode::Release _mask){
			return (BYTE)((this->inputState&(int)_mask) >> 8);
		}

		/*!-----------------------------------------------------------
		//	@brief		マウスの入力状態更新
		//	@param[in]	_hWnd	ウィンドウハンドル(プロシージャ側から)[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/31
		------------------------------------------------------------*/
		void	Update(CONST HWND _hWnd = nullptr);

		/*!-----------------------------------------------------------
		//	@brief		マウスホイールの更新
		//	@note		プロシージャで更新しないと精度がガタ落ち
		//	@param[in]	_zDelta	ホイールの状態
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/01
		------------------------------------------------------------*/
		void	UpdateWheel(SHORT _zDelta){
			this->zQueue.push(_zDelta / 120);
		}

	private:
		HWND	hWnd;
		BOOL	isNowActive;
		SHORT	inputState;
		POINT	logCursorPos;
		POINT	nowCursorPos;
		std::queue<SHORT>	zQueue;

	private:
		/*!-----------------------------------------------------------
		//	@brief		マウス座標の更新
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/05
		------------------------------------------------------------*/
		inline	void	UpdateCursorPos();

	private:
		Mouse();
	};
}

#endif // _Include_Mouse_hpp_
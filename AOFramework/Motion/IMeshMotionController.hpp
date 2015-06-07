/*************************************************************
*	@file   	IMeshMotionController.hpp
*	@brief  	IMeshMotionControllerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/11/03
*************************************************************/

#ifndef _Include_IMeshMotionController_hpp_	// インクルードガード
#define _Include_IMeshMotionController_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------

namespace ao
{
	enum class MotionType : int
	{
		Stop = 0,
		Once,
		Loop,
		InverceOnce,
		InverceLoop,
		SeamlessOnce,
		SeamlessLoop,
		SeamlessInverceOnce,
		SeamlessInverceLoop
	};

	/*!-----------------------------------------------------------
	//	@class  IMeshMotionController
	//	@brief  モーションコントローラーインターフェースクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/11/03
	------------------------------------------------------------*/
	class IMeshMotionController
	{
	public:
		IMeshMotionController(){};
		virtual ~IMeshMotionController(){};

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		virtual HRESULT Initialize() = 0;

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		virtual void Update() = 0;
	};
}

#endif // _Include_IMeshMotionController_hpp_
/*************************************************************
*	@file   	IMeshMotionController.hpp
*	@brief  	IMeshMotionController�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/11/03
*************************************************************/

#ifndef _Include_IMeshMotionController_hpp_	// �C���N���[�h�K�[�h
#define _Include_IMeshMotionController_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���[�V�����R���g���[���[�C���^�[�t�F�[�X�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/11/03
	------------------------------------------------------------*/
	class IMeshMotionController
	{
	public:
		IMeshMotionController(){};
		virtual ~IMeshMotionController(){};

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		virtual HRESULT Initialize() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/03
		------------------------------------------------------------*/
		virtual void Update() = 0;
	};
}

#endif // _Include_IMeshMotionController_hpp_
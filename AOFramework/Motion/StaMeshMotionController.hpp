/*************************************************************
*	@file   	StaMeshMotionController.hpp
*	@brief  	StaMeshMotionController�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/31
*************************************************************/

#ifndef _Include_StaMeshMotionController_hpp_	// �C���N���[�h�K�[�h
#define _Include_StaMeshMotionController_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<vector>
#include"../Math/Math.hpp"
#include"IMeshMotionController.hpp"

namespace ao
{
	struct StaFrame{
		std::vector<XMFLOAT4X4> mtx;
	};
	struct StaMotion{
		int beginFrame;				///< �J�n�t���[��
		int endFrame;				///< �I���t���[��
		int frameCount;				///< �t���[����
		std::vector<StaFrame> frame;
	};

	typedef std::vector<StaMotion> StaMotionList;

	struct StaMotionData{
		int numMesh;				///< ���b�V����
		StaMotionList list;			///< ���[�V�������X�g
	};

	/*!-----------------------------------------------------------
	//	@class  StaMeshMotionController
	//	@brief  �ÓI���b�V�����[�V��������N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/31
	------------------------------------------------------------*/
	class StaMeshMotionController : public IMeshMotionController
	{
	public:
		/// �R���X�g���N�^
		StaMeshMotionController();

		/// �f�X�g���N�^
		~StaMeshMotionController();

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	�Ȃ�
		//  @return     S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		���[�V�����ύX
		//	@param[in]	_index	���[�V�����C���f�b�N�X
		//	@param[in]	_type	���[�V�����^�C�v
		//	@return		TRUE�F�ύX�����@FALSE�F�ύX���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/01
		------------------------------------------------------------*/
		BOOL ChangeMotion(int _index, MotionType _type);

		/*!-----------------------------------------------------------
		//	@brief		�v�Z���ꂽ�s��f�[�^�̎擾
		//	@param[in]	_pOut	�o�͂���s��|�C���^
		//	@param[in]	_size	�o�͐�̃o�b�t�@�T�C�Y
		//	@return		TRUE�F�o�͐����@FALSE�F�o�͎��s
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/01
		------------------------------------------------------------*/
		BOOL GetMoitonData(XMFLOAT4X4* _pOut, const size_t _size);

		/*!-----------------------------------------------------------
		//	@brief		���[�V�������Đ������ǂ����`�F�b�N
		//	@return		TRUE�F�Đ����@FALSE�F�Đ����ł͂Ȃ�
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		BOOL IsPlayNow();

		/*!-----------------------------------------------------------
		//	@brief		�Đ�����
		//	@param[in]	����������t���[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		void Play(int _setFrame = -1);

		/*!-----------------------------------------------------------
		//	@brief		��~����
		//	@param[in]	_����������t���[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		void Stop(int _setFrame=-1);

		/*!-----------------------------------------------------------
		//	@brief		���݂̃��[�V�����C���f�b�N�X���擾
		//	@return		���[�V�����C���f�b�N�X
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMotionIndex() const {
			return this->nowMotionIndex;
		}
		
		/*!-----------------------------------------------------------
		//	@brief		���[�V�����̃^�C�v�擾
		//	@return		���[�V�����^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		MotionType GetNowMotionType(){
			return this->motionType;
		}

		/*!-----------------------------------------------------------
		//	@brief		���݂̃t���[���擾
		//	@return		���݂̃��[�V�����t���[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMotionFrame(){
			return this->nowFrame;
		}

		/*!-----------------------------------------------------------
		//	@brief		���݂̃��[�V�����ő�t���[���擾
		//	@return		���݂̃��[�V�����ő�t���[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMaxMotionFrame(){
			return this->motionData.list[this->nowMotionIndex].frameCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		���b�V�����擾
		//	@return		���b�V����
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNumMesh(){
			return this->numMesh;
		}

	public:
		BOOL enable;
		int numMesh;
		int nowFrame;
		int nowMotionIndex;
		int nextMotionIndex;
		MotionType motionType;
		std::vector<XMFLOAT4X4> mapData;
		StaMotionData motionData;
	};
}

#endif // _Include_StaMeshMotionController_hpp_
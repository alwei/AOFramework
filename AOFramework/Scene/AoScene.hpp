/*************************************************************
*	@file   	AoScene.hpp
*	@brief  	AoScene�֘A�̃N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/07/31
*************************************************************/

#ifndef _Include_AoScene_hpp_	// �C���N���[�h�K�[�h
#define _Include_AoScene_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<Windows.h>
#include<process.h>
#include<DirectXMath.h>
#include"../Math/Math.hpp"
#include"../Resource/LightingPackage.hpp"
#include"../Utility/NonCopyable.hpp"
#include"../Define/SystemDefine.hpp"


using namespace DirectX;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ISceneBase
	//	@brief  �V�[���̃C���^�[�t�F�[�X�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/07/31
	------------------------------------------------------------*/
	class ISceneBase : private NonCopyable
	{
	public:
		ISceneBase(){};
		virtual	~ISceneBase()override{};

		/*!-----------------------------------------------------------
		//	@brief		���[�h�O�Ɏ��s����
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	InitPreLoad() = 0;

		/*!-----------------------------------------------------------
		//	@brief		���[�h�X���b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	LoadThread() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����s�O�ɃR�[�������
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual	void	StartCall() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	Update() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/07/31
		------------------------------------------------------------*/
		virtual	void	Render() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�t���[���X�L�b�v����
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/16
		------------------------------------------------------------*/
		virtual void	FrameSkip() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����擾
		//	@return		�V�[����
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() = 0;

	protected:
		/*!-----------------------------------------------------------
		//	@brief		�h����̍X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnStartCall() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�h����̍X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnUpdate() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�h����̃����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void	OnRender() = 0;
	};


	/*!-----------------------------------------------------------
	//	@class  ILoadScene
	//	@brief  ���[�h�p�̃V�[���C���^�[�t�F�[�X�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/21
	------------------------------------------------------------*/
	class ILoadScene : private NonCopyable
	{
	public:
		ILoadScene(){}
		virtual ~ILoadScene()override{}

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Initialize() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Update() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void Render() = 0;

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����擾
		//	@return		�V�[����
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() = 0;
	};


	/*!-----------------------------------------------------------
	//	@class  SceneBase
	//	@brief  �V�[���̊��N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/17
	------------------------------------------------------------*/
	class SceneBase : public ISceneBase
	{
	public:
		SceneBase(AOLPCSTR _sceneName) :
			isLoadEnd(TRUE),
			isStart(FALSE),
			enableFrameSkip(TRUE),
			enableUpdate(TRUE),
			enableRender(TRUE),
			lightDir(0, -1, 0),
			eyePos(0, 0, 0),
			clearColor(0, 0, 0, 0),
			sceneName(_sceneName),
			pLoadScene(nullptr)
		{
			ao::MatrixIdentity(&viewMatrix);
			ao::MatrixIdentity(&projMatrix);
		};
		virtual ~SceneBase()override{};

		/*!-----------------------------------------------------------
		//	@brief		���[�h�O�Ɏ��s����
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		virtual	void	InitPreLoad()override{};

		/*!-----------------------------------------------------------
		//	@brief		���[�h�X���b�h
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		virtual	void	LoadThread()override{};

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����s�O�ɃR�[�������
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	StartCall() final override;

		/*!-----------------------------------------------------------
		//	@brief		�X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	Update() final override;

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	Render() final override;

		/*!-----------------------------------------------------------
		//	@brief		�t���[���X�L�b�v����
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/16
		------------------------------------------------------------*/
		virtual void	FrameSkip()override{};

		/*!-----------------------------------------------------------
		//	@brief		�ˉe�s�񏉊���
		//	@param[in]	_pProjMtx	�ˉe�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	InitializeProjectionMatrix(const XMFLOAT4X4* const _pProjMtx);

		/*!-----------------------------------------------------------
		//	@brief		�r���[�s��X�V
		//	@param[in]	_pViewMtx	�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateViewMatrix(const XMFLOAT4X4* const _pViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g�r���[�s��X�V
		//	@param[in]	_pLightViewMtx	���C�g�r���[�s��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		void	UpdateLightViewMatrix(const XMFLOAT4X4* const _pLightViewMtx);

		/*!-----------------------------------------------------------
		//	@brief		���C�g������`
		//	@param[in]	_x			X����
		//  @param[in]  _y			Y����
		//  @param[in]  _z			Z����
		//  @param[in]  _ambient	�����̋���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateLightDirection(float _x, float _y, float _z,float _ambient=0.5f);

		/*!-----------------------------------------------------------
		//	@brief		���_���W�X�V
		//	@param[in]	_x				X���W
		//  @param[in]  _y				Y���W
		//  @param[in]  _z				Z���W
		//	@param[in]	_specularPower	�X�y�L�����p���[
		//	@param[in]	_specularRange	�X�y�L�����͈�
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		void	UpdateEyePosition(float _x, float _y, float _z,float _specularPower=1.0f,int _specularRange=10);

		/*!-----------------------------------------------------------
		//	@brief		�N���A�J���[��ݒ�
		//	@param[in]	_r	�ԐF[�ȗ���]
		//	@param[in]	_g	�ΐF[�ȗ���]
		//	@param[in]	_b	�F[�ȗ���]
		//	@param[in]	_a	���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		void	SetClearColor(float _r = 0, float _g = 0, float _b = 0, float _a = 0);

		/*!-----------------------------------------------------------
		//	@brief		���[�h�V�[����ݒ�
		//	@param[in]	_pPushLoadScene	���[�h�V�[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		void PushLoadScene(ILoadScene* _pPushLoadScene);

		/*!-----------------------------------------------------------
		//	@brief		�V�[�����擾
		//	@return		�V�[����
		//	@author		Tatsunori Aoyama
		//	@date		2015/04/27
		------------------------------------------------------------*/
		virtual AOLPCSTR GetSceneName() override
		{
			return this->sceneName.c_str();
		}

	public:
		BOOL isLoadEnd;
		BOOL isStart;
		BOOL enableUpdate;
		BOOL enableRender;
		BOOL enableFrameSkip;
		XMFLOAT3 lightDir;
		XMFLOAT3 eyePos;
		XMFLOAT4 clearColor;
		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 lightViewMatrix;
		XMFLOAT4X4 projMatrix;
		string sceneName;
		ILoadScene* pLoadScene;

	protected:
		/*!-----------------------------------------------------------
		//	@brief		�h����̍ŏ��̃R�[��
		//	@author		Tatsunori Aoyama
		//	@date		2015/02/28
		------------------------------------------------------------*/
		virtual void OnStartCall()override{};

		/*!-----------------------------------------------------------
		//	@brief		�h����̍X�V
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void OnUpdate()override{};

		/*!-----------------------------------------------------------
		//	@brief		�h����̃����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		virtual void OnRender()override{};

	private:
		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�J�n
		//  @param[in]  _r	�ԐF[�ȗ���]
		//  @param[in]  _g	�ΐF[�ȗ���]
		//  @param[in]  _b	�F[�ȗ���]
		//  @param[in]  _a	���ߗ�[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		void RenderBegin();

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�I��
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/07
		------------------------------------------------------------*/
		void RenderEnd();
	};
}


#endif // _Include_AoScene_hpp_

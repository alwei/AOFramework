/*************************************************************
*	@file   	DirectX11Profiler.hpp
*	@brief  	DirectX11Profiler�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/05/17
*************************************************************/

#ifndef _Include_DirectX11Profiler_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11Profiler_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"DirectX11BaseApi.hpp"
#include"../Math/MathDefine.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"../Resource/ConstVectorBuffer.hpp"
#include"../System/CountTime.hpp"


enum class ProcessLoadType : u8
{
	INIT = 0,
	LOAD,
	UPDATE,
	RENDER_COMMAND,
	RENDER_BURST,
	RENDER_PRESENT,
	RENDER_CLEAR,
	MAX,
	ALL
};

static const u8 PROC_LOAD_MAX_TYPE = static_cast<u8>(ProcessLoadType::MAX);


namespace ao
{
	class CountTime;
	class DirectX11RenderTarget;
}


typedef std::array<ao::CountTime*, static_cast<u8>(ProcessLoadType::MAX)> ProcLoadTimeArray;
typedef std::array<ao::DirectX11RenderTarget*, static_cast<u8>(ProcessLoadType::MAX)> ProcRenderArray;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Profiler
	//	@brief  �v���t�@�C���V�X�e��
	//	@author	Tatsunori Aoyama
	//	@date	2014/05/17
	------------------------------------------------------------*/
	class DirectX11Profiler : public DirectX11BaseApi
	{
	public:
		/// �R���X�g���N�^
		DirectX11Profiler(
			const u32 _width,
			const u32 _height,
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// �f�X�g���N�^
		~DirectX11Profiler();

		/*!-----------------------------------------------------------
		//	@brief		�v���t�@�C���^�C�v���w��
		//	@param[in]	_setProfileIndex	�v���t�@�C���C���f�b�N�X
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispProfileIndex(ProcessLoadType _setProfileIndex = ProcessLoadType::RENDER_BURST)
		{
			this->selectedProfileIndex = _setProfileIndex;
		}

		/*!-----------------------------------------------------------
		//	@brief		�����_�����O�����ʍ��W
		//	@param[in]	_x	X���W
		//	@param[in]	_y	���W
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispPosition(const f32 _x, const f32 _y)
		{
			this->dispPosition.x = _x;
			this->dispPosition.y = _y;
		}

		/*!-----------------------------------------------------------
		//	@brief		�\������v���t�@�C�����ʂ̃T�C�Y
		//	@param[in]	_width	����
		//	@param[in]	_height	�c��
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispScale(const f32 _width, const f32 _height)
		{
			this->dispScale.x = _width;
			this->dispScale.y = _height;
		}

		/*!-----------------------------------------------------------
		//	@brief		�O���t�̃J���[�ݒ�
		//	@param[in]	_setGraphColor	�O���t�F
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetGraphColor(color _setGraphColor = color::white)
		{
			this->graphColor = _setGraphColor;
		}

		/*!-----------------------------------------------------------
		//	@brief		�w�i�J���[�ݒ�
		//	@param[in]	_setBackColor	�w�i�J���[
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetBackColor(color _setBackColor = color::transparent)
		{
			this->backColor = _setBackColor;
		}

		/*!-----------------------------------------------------------
		//	@brief		�v������t���[�����[�g��ݒ�
		//	@param[in]	_setFrameRate	�ݒ肷��t���[�����[�g[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void SetUseRecordFrameRate(const u32 _setFrameRate = 60)
		{
			this->useRecordFrameRate = _setFrameRate;
		}

		/*!-----------------------------------------------------------
		//	@brief		�v����Ԃ̊J�n
		//	@param[in]	_loadType	���׌v���^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void Begin(ProcessLoadType _procLoadType)
		{
			this->pProcLoadTime[static_cast<u8>(_procLoadType)]->Begin();
		}

		/*!-----------------------------------------------------------
		//	@brief		�v����Ԃ̏I��
		//	@param[in]	_loadType	���׌v���^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void End(ProcessLoadType _procLoadType)
		{
			this->pProcLoadTime[static_cast<u8>(_procLoadType)]->End();
		}

		/*!-----------------------------------------------------------
		//	@brief		������
		//	@param[in]	_pGraphicsRenderer	�����_���[
		//	@param[in]	_pGraphicsState		�O���t�B�b�N�X�e�[�g
		//	@param[in]	_pShader			�V�F�[�_�[�Ǘ��N���X
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/19
		------------------------------------------------------------*/
		HRESULT Initialize(
		class DirectX11GraphicsRenderer* const _pGraphicsRenderer,
		class DirectX11GraphicsState* const _pGraphicsState,
		class DirectX11Shader* const pShader);

		/*!-----------------------------------------------------------
		//	@brief		�v���t�@�C�����ʂ̃����_�����O
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/18
		------------------------------------------------------------*/
		void RenderResult();

	private:
		struct GeometryConstBuffer
		{
			float4 procLoad;
		};

		typedef ConstVectorBuffer<GeometryConstBuffer> LoadProcBuffer;

		u32 renderWidth;
		u32 renderHeight;
		u32 recordCount;
		u32 useRecordFrameRate;
		float2 dispPosition;
		float2 dispScale;
		color graphColor;
		color backColor;
		ProcessLoadType selectedProfileIndex;
		class DirectX11Shader* pShader;
		class DirectX11GraphicsRenderer* pGraphicsRenderer;
		class DirectX11GraphicsState* pGraphicsState;
		class VertexShader* pVertexShader;
		class GeometryShader* pGeometryShader;
		class PixelShader* pPixelShader;
		ProcLoadTimeArray pProcLoadTime;
		ProcRenderArray pProcLoadRT;
		ConstBuffer<float4> geomConstBuffer0;
		std::array<LoadProcBuffer, PROC_LOAD_MAX_TYPE> geomConstBuffer1;
		SmartComPtr<ID3D11Buffer> pVertexBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		���׌��ʂ��O���t�Ń����_�����O
		//	@param[in]	_pRenderTarget	�`�悷���񂾁[�^�[�Q�b�g
		//  @param[in]  _pProcTime		�v�������v���Z�X����
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/18
		------------------------------------------------------------*/
		void RenderProcessLoad(
		class DirectX11RenderTarget* const _pRenderTarget,
		class CountTime* const _pProcTime,
			LoadProcBuffer* const pMapGeomBuffer1);
	};
}

#endif // _Include_DirectX11Profiler_hpp_
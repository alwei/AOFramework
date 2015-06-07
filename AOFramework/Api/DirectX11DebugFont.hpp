/*************************************************************
*	@file   	DirectX11DebugFont.hpp
*	@brief  	DirectX11DebugFont�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/09/24
*************************************************************/

#ifndef _Include_DirectX11DebugFont_hpp_	// �C���N���[�h�K�[�h
#define _Include_DirectX11DebugFont_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<mutex>
#include"DirectX11BaseApi.hpp"
#include"DirectX11RenderTarget.hpp"
#include"DirectX11GraphicsState.hpp"
#include"DirectX11GraphicsRenderer.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"../Resource/Sprite.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DebugChar
	//	@brief  �f�o�b�O�p�̕���
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/24
	------------------------------------------------------------*/
	class DebugChar
	{
	public:
		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���쐬
		//	@param[in]	_pd3dDevice			�f�o�C�X�|�C���^
		//	@param[in]	_pd3dDeviceContext	�f�o�C�X�R���e�L�X�g
		//  @param[in]  _code				�����R�[�h
		//  @param[in]  _lf					�t�H���g���
		//	@param[in]	_fontSize			�t�H���g�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		void	Create(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext,
			const DWORD _code,
			const LOGFONT& _lf,
			int _fontSize);


		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`���擾
		//	@return		�e�N�X�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		ID3D11ShaderResourceView* GetTexture()
		{
			return this->pTexture.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		�n�b�V���l�擾
		//	@return		�n�b�V���l
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		int64_t GetHash()
		{
			return this->hash;
		}

	private:
		SmartComPtr<ID3D11ShaderResourceView> pTexture;
		SmartComPtr<ID3D11Device> pd3dDevice;
		SmartComPtr<ID3D11DeviceContext> pd3dDeviceContext;
		int64_t hash;

	private:
		struct FONT_INFO
		{
			TEXTMETRIC		tm;
			GLYPHMETRICS	gm;
			DWORD			bitSize;
		};

		/*!-----------------------------------------------------------
		//	@brief		�t�H���g���쐬
		//  @param[out] _pFont		�o�͂���t�H���g
		//  @param[in]  _lf			�t�H���g���
		//  @param[in]  _fontSize	�t�H���g�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void CreateFontInfo(HFONT* _pFont,const LOGFONT& _lf, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		�r�b�g��񐶐�
		//  @param[out] _pBuffer	�o�b�t�@
		//	@param[out]	_pFontInfo	�o�͂���t�H���g���
		//  @param[in]  _code		�����R�[�h
		//  @param[in]  _hFont		�t�H���g�n���h��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void GenerateFontBitMap(
			std::vector<BYTE>* _pBuffer,
			FONT_INFO* _pFont,
			const DWORD _code,
			const HFONT& _hFont);

		/*!-----------------------------------------------------------
		//	@brief		�e�N�X�`����������
		//	@param[in]	_pBuffer	�o�b�t�@
		//	@param[in]	_pFont		�t�H���g���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		inline	void WriteBitChar(std::vector<BYTE>* _pBuffer,FONT_INFO* _pFont);
	};

	/*!-----------------------------------------------------------
	//	@class  DirectX11DebugFont
	//	@brief  �f�o�b�O�t�H���g
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/24
	------------------------------------------------------------*/
	class DirectX11DebugFont : public DirectX11BaseApi
	{
	public:
		DirectX11DebugFont(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		~DirectX11DebugFont()override final;

		/*!-----------------------------------------------------------
		//	@brief		������
		//  @param[in]  _pGraphicsState		�O���t�B�b�N�X�e�[�g
		//  @param[in]  _pRenderer			�����_���[
		//	@return		S_OK�F�����������@����ȊO�F���������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT	Initialize(
			const u32 _nWidth,
			const u32 _nHeight,
			DirectX11GraphicsState* const _pGraphicsState,
			DirectX11GraphicsRenderer* const _pRenderer);

		/*!-----------------------------------------------------------
		//	@brief		������ʕ`��
		//	@param[in]	_x			X���W
		//	@param[in]	_y			Y���W
		//	@param[in]	_bWidth		�w�i����
		//	@param[in]	_bHeight	�w�i�c��
		//	@param[in]	_fontSize	�t�H���g�T�C�Y
		//	@param[in]	_numChar	�`�悷�镶����
		//	@param[in]	_r			�ԐF
		//	@param[in]	_g			�ΐF
		//	@param[in]	_b			�F
		//	@param[in]	_a			���ߗ�
		//	@param[in]	_bR			�ԐF�w�i
		//	@param[in]	_bG			�ΐF�w�i
		//	@param[in]	_bB			�F�w�i
		//	@param[in]	_bA			���ߗ��w�i
		//	@param[in]	_str		������
		//	@param[in]	...			�ϗv�f
		//	@return		S_OK�F�`�搬���@����ȊO�F�`�掸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT Draw(
			int _x,
			int _y,
			int _bWidth,
			int _bHeight,
			int _fontSize,
			int _numChar,
			float _r,
			float _g,
			float _b,
			float _a,
			float _bR,
			float _bG,
			float _bB,
			float _bA,
			string _str,
			...);

		/*!-----------------------------------------------------------
		//	@brief		������ʕ`��
		//	@param[in]	_x			X���W
		//	@param[in]	_y			Y���W
		//	@param[in]	_bWidth		�w�i����
		//	@param[in]	_bHeight	�w�i�c��
		//	@param[in]	_fontSize	�t�H���g�T�C�Y
		//	@param[in]	_r			�ԐF
		//	@param[in]	_g			�ΐF
		//	@param[in]	_b			�F
		//	@param[in]	_a			���ߗ�
		//	@param[in]	_bR			�ԐF�w�i
		//	@param[in]	_bG			�ΐF�w�i
		//	@param[in]	_bB			�F�w�i
		//	@param[in]	_bA			���ߗ��w�i
		//	@param[in]	_str		������
		//	@param[in]	...			�ϗv�f
		//	@return		S_OK�F�`�搬���@����ȊO�F�`�掸�s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT Draw(
			int _x,
			int _y,
			int _bWidth,
			int _bHeight,
			int _fontSize,
			float _r,
			float _g,
			float _b,
			float _a,
			float _bR,
			float _bG,
			float _bB,
			float _bA,
			string _str,
			...);

		/*!-----------------------------------------------------------
		//	@brief		�o�[�X�g�`��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		void	RenderBurst();

		/*!-----------------------------------------------------------
		//	@brief		�t�H���g�̕ύX
		//	@param[in]	_fontName	�t�H���g��
		//  @param[in]  _fontPath	�t�H���g�p�X
		//	@return		S_OK�F�ύX�����@����ȊO�F�ύX���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/24
		------------------------------------------------------------*/
		HRESULT	ChangeFont(const string& _fontName, const string& _fontPath);

	private:
		struct FontShader
		{
			SmartComPtr<ID3D11VertexShader> vertex;
			SmartComPtr<ID3D11InputLayout> inputLayout;
			SmartComPtr<ID3D11PixelShader> pixel;
		};

		struct VsConst
		{
			XMFLOAT4X4 matWorld;
			XMFLOAT4X4 matOrtho;
		};

		struct PsConst
		{
			XMFLOAT4 color;
		};

		struct CHAR_COMMAND
		{
			float x, y;
			float bWidth, bHeight;
			float fontSize;
			int numChar;
			float r, g, b, a;
			float bR, bG, bB, bA;
			std::array<AOCHAR, 2048> buffer;
		};

		struct STR_COMMAND
		{
			float x, y;
			float bWidth, bHeight;
			float fontSize;
			float r, g, b, a;
			float bR, bG, bB, bA;
			std::array<AOCHAR, 2048> buffer;
		};

	private:
		string fontName;
		string fontPath;
		LOGFONT fontInfo;
		FontShader fontShader;
		XMFLOAT4X4 surfaceOrtho;
		XMFLOAT4X4 stringOrtho;
		ConstBuffer<VsConst> vsConst;
		ConstBuffer<PsConst> psConst;
		SmartComPtr<ID3D11Buffer> pSpriteBuffer;
		SmartComPtr<ID3D11SamplerState> pSamplerState;
		DirectX11GraphicsState*		pGraphicsState;
		DirectX11GraphicsRenderer*	pRenderer;
		std::map<int64_t, DebugChar*> charTextureMap;
		std::map<string, DirectX11RenderTarget*> strTextureMap;
		std::vector<CHAR_COMMAND> charCommandList;
		std::vector<STR_COMMAND> strCommandList;

	private:
		/*!-----------------------------------------------------------
		//	@brief		�T���v���[�X�e�[�g�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F�쐬���s
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	HRESULT	CreateSamplerState();

		/*!-----------------------------------------------------------
		//	@brief		�f�t�H���g�X�v���C�g�쐬
		//	@return		S_OK�F�쐬�����@����ȊO�F
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	HRESULT	CreateDefaultSprite();

		/*!-----------------------------------------------------------
		//	@brief		�����e�N�X�`���쐬
		//	@param[in]	_code		�����R�[�h
		//  @param[in]  _fontSize	�t�H���g�T�C�Y
		//  @return     �V�F�[�_�[���\�[�X�r���[
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	ID3D11ShaderResourceView* CreateCharacterTexture(UINT _code, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		������e�N�X�`���쐬
		//	@param[in]	_str		������
		//  @param[in]  _fontSize	�t�H���g�T�C�Y
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	BaseTexture* CreateStringTexture(const string& _str, int _fontSize);

		/*!-----------------------------------------------------------
		//	@brief		������̕`��
		//	@param[in]	_commandInfo	�R�}���h���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	void OnDraw(const CHAR_COMMAND& _commandInfo);

		/*!-----------------------------------------------------------
		//	@brief		������̕`��
		//	@param[in]	_commandInfo	�R�}���h���
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/26
		------------------------------------------------------------*/
		inline	void OnDraw(const STR_COMMAND& _commandInfo);
	};
}

#endif // _Include_DirectX11DebugFont_hpp_
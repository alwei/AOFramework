#ifndef __AO_MACRO_HPP__
#define	__AO_MACRO_HPP__

#include<stdlib.h>
#include<Windows.h>
#include<tchar.h>
#include"../Define/SystemDefine.hpp"


namespace ao
{
	namespace macro
	{
		/*!-----------------------------------------------------------
		//	@brief		���v���Z�X��HINSTANCE�擾
		//	@return		���v���Z�X��HINSTANCE
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	HINSTANCE	GetHInstance()
		{
			return	((HINSTANCE)GetModuleHandle(0));
		}

		/*!-----------------------------------------------------------
		//	@brief		float�^����DWORD�^�֕ϊ�����}�N��
		//	@param[in]	_f �ϊ�����float�^�̕�������
		//	@return		�ϊ����ꂽDWORD�^�̌Œ菬��
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	DWORD	FtoDW(float _f)
		{
			return *((DWORD*)&_f);
		}

		/*!-----------------------------------------------------------
		//	@brief		�G���[�x���\��
		//	@param[in]	_errStr �G���[���e�̕�����
		//	@author		Tatsunori Aoyama
		//	@date		2013/03/23
		------------------------------------------------------------*/
		inline	void	ERROR_MASSAGE(AOLPCSTR _errStr)
		{
			MessageBox(NULL, _errStr, _T("ERROR"), MB_ICONWARNING);
		}

		/*!-----------------------------------------------------------
		//	@brief		�F�擾�}�N��
		//	@param[in]	_r	��
		//	@param[in]	_g	��
		//	@param[in]	_b	��
		//	@return		�F�����r�b�g�V�t�g�����l
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/30
		------------------------------------------------------------*/
		inline	DWORD	GetColor(BYTE _r, BYTE _g, BYTE _b)
		{
			DWORD color = 0xFF000000;
			color |= _r << 16;
			color |= _g << 8;
			color |= _b << 0;

			return color;
		}

		/*!-----------------------------------------------------------
		//	@brief		�F�擾�}�N��
		//	@param[in]	_r	��
		//	@param[in]	_g	��
		//	@param[in]	_b	��
		//	@param[in]	_a	����
		//	@return		�F�����r�b�g�V�t�g�����l
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/30
		------------------------------------------------------------*/
		inline	DWORD	GetColorA(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
		{
			DWORD color = 0;
			color |= _a << 24;
			color |= _r << 16;
			color |= _g << 8;
			color |= _b << 0;

			return color;
		}

		/*!-----------------------------------------------------------
		//	@brief		�F�̕��������ϊ�
		//	@param[in]	color		32bit�������F���
		//	@param[in]	shiftBit	�V�t�g����r�b�g��
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/15
		------------------------------------------------------------*/
		inline	float	ColorToFloat(DWORD _color, int _shiftBit)
		{
			return ((_color >> _shiftBit) & 0xFF) / 255.0f;
		}

		/*!-----------------------------------------------------------
		//	@brief		32bit�̐F���𕂓������̐F���ɕϊ�
		//	@param[out]	_pOut		�o�͂���F�z��
		//	@param[in]	_dwColor	�F���
		//	@return		float�z��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/08/29
		------------------------------------------------------------*/
		inline	CONST FLOAT* GetColorDtoF(FLOAT* _pOut, CONST DWORD _dwColor)
		{
			_pOut[0] = ColorToFloat(_dwColor, 24);
			_pOut[1] = ColorToFloat(_dwColor, 16);
			_pOut[2] = ColorToFloat(_dwColor, 8);
			_pOut[3] = ColorToFloat(_dwColor, 0);

			return _pOut;
		}


		/*!-----------------------------------------------------------
		//	@brief		�����R�[�h�ϊ��}�N��
		//	@note		2�o�C�g������1�o�C�g�������`�F�b�N���Ă����
		//	@param[out]	_pOutCharCode		�o�͂��镶���R�[�h
		//	@param[in]	_pInCharData		���͂��镶���f�[�^
		//	@return		TRUE�F2�o�C�g����	FALSE�F1�o�C�g����
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	BOOL	CharToUINT(UINT* _pOutCharCode, const char* _pInCharData)
		{
			//�}���`�o�C�g�����ɂ�鏈��
			if (IsDBCSLeadByte(*_pInCharData))
			{
				(*_pOutCharCode) = (BYTE)(*_pInCharData) << 8 | (BYTE)_pInCharData[1];
				return TRUE;
			}

			(*_pOutCharCode) = (UINT)(*_pInCharData);

			return FALSE;
		}


		/*!-----------------------------------------------------------
		//	@brief		���������p���S�p��������
		//	@param[in]	_wChar	UNICODE����
		//  @return     TRUE�F�S�p�@FALSE�F���p
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/25
		------------------------------------------------------------*/
		inline	BOOL	IsCheckEm(const AOCHAR& _char)
		{
			WORD type = 0;
			GetStringTypeEx(
				LOCALE_SYSTEM_DEFAULT,	// ���j�R�[�h�Ńv���W�F�N�g�ł́A������������       
				CT_CTYPE3,				// �S�p�A���p�A�Ђ炪�ȓ��𔻒肷��^�C�v
				&_char,					// ����ΏۂƂȂ镶���z��
				1,						// ����ΏۂƂȂ镶���z��̃T�C�Y
				&type);					// ���茋�ʂ̔z��

			if (type & C3_HALFWIDTH)
			{
				return FALSE;
			}

			return TRUE;
		}

		/*!-----------------------------------------------------------
		//	@brief		���C�h�����񂩂�}���`�o�C�g�����ɕϊ�
		//	@param[out]	_pOut	�o�͂��镶����o�b�t�@
		//	@param[in]	_src	���͂��镶����o�b�t�@
		//	return		�ϊ����ꂽ������
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	size_t	StringWideToMulti(std::string* _pOut, const std::wstring& _src)
		{
			size_t num;
			size_t size = _src.length()*MB_CUR_MAX + 1;
			char* cnv = new char[size];
			wcstombs_s(&num, cnv, size, _src.c_str(), size);
			_pOut->assign(cnv);
			delete[] cnv;
			return num;
		}

		/*!-----------------------------------------------------------
		//	@brief		�}���`�o�C�g�����񂩂烏�C�h�����ɕϊ�
		//	@param[out]	_pOut	�o�͂��镶����o�b�t�@
		//	@param[in]	_src	���͂��镶����o�b�t�@
		//	return		�ϊ����ꂽ������
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	size_t	StringMultiToWide(std::wstring* _pOut, const std::string& _src)
		{
			size_t num;
			size_t size = _src.length() + 1;
			wchar_t* cnv = new wchar_t[size];
			mbstowcs_s(&num, cnv, size, _src.c_str(), size);
			_pOut->assign(cnv);
			delete[] cnv;
			return num;
		}

		/*!-----------------------------------------------------------
		//	@brief		������p�X����t�@�C�������擾
		//	@param[out]	_pOut		�o�͂��镶����o�b�t�@�|�C���^
		//	@param[in]	_bufSize	�o�͂��镶����o�b�t�@�T�C�Y
		//	@param[in]	_pSrc		���o���镶����o�b�t�@�|�C���^
		//	@param[in]	_isAddExt	�g���q��t�����邩�ǂ���[�ȗ���]
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	void GetFileNameFromPath(AOLPSTR _pOut, size_t _bufSize, LPCTSTR _pSrc, BOOL _isAddExt = TRUE)
		{
			int		offset, nowOffset;
			AOCHAR	endCode = (_isAddExt) ? _T('\0') : _T('.');

			//�I�[����->�K�w����
			for (offset = 0; _pSrc[offset] != endCode; ++offset);
			for (; _pSrc[offset] != '\\'&&_pSrc[offset] != '/'&&offset != -1; --offset);
			for (nowOffset = 0; _pSrc[offset + 1] != endCode || _T('\0'); ++offset, ++nowOffset)
			{
				if (nowOffset >= (int)_bufSize)
				{
					return;
				}

				_pOut[nowOffset] = _pSrc[offset + 1];
			}

			_pOut[nowOffset] = _T('\0');
		}

		/*!-----------------------------------------------------------
		//	@brief		������p�X����t�@�C���p�X
		//	@note		_pOut		�o�͂��镶����o�b�t�@�|�C���^
		//	@param[in]	_pSrc		���o���镶����o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/02
		------------------------------------------------------------*/
		inline	void	GetFilePathFromPath(AOLPSTR _pOut, AOLPCSTR _pSrc)
		{
			//�t�@�C���g���q�K�w�̃t�@�C�������폜
			int copySize, offset;
			for (offset = 0; _pSrc[offset] != _T('\0'); ++offset);
			for (; _pSrc[offset] != _T('\\')&&_pSrc[offset] != _T('/'); --offset);

			copySize = offset * sizeof(AOCHAR) + sizeof(AOCHAR);
			::memcpy(_pOut, _pSrc, copySize);
			_pOut[offset+1] = _T('\0');
		}

		/*!-----------------------------------------------------------
		//	@brief		������p�X����t�@�C���p�X
		//	@note		�}���`�o�C�g��p
		//	@note		_pOut		�o�͂��镶����o�b�t�@�|�C���^
		//	@param[in]	_pSrc		���o���镶����o�b�t�@
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/03
		------------------------------------------------------------*/
		inline	void	GetFilePathFromPath_MultiByte(LPSTR _pOut, LPCTSTR _pSrc)
		{
			//�t�@�C���g���q�K�w�̃t�@�C�������폜
			int copySize, offset;
			for (offset = 0; _pSrc[offset] != '\0'; ++offset);
			for (; _pSrc[offset] != '\\'&&_pSrc[offset] != '/'; --offset);

			copySize = sizeof(AOCHAR)*offset + 1;
			::memcpy(_pOut, _pSrc, copySize);
			_pOut[copySize] = '\0';
		}

		/*!-----------------------------------------------------------
		//	@brief		������p�X����g���q���o
		//	@param[out]	_pOut		�o�͂��镶����o�b�t�@�|�C���^
		//	@param[in]	_bufSize	�o�͂��镶����o�b�t�@�T�C�Y
		//	@param[in]	_pSrc		���o���镶����o�b�t�@�|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/20
		------------------------------------------------------------*/
		inline	void	GetFileExtension(AOLPSTR _pOut, size_t _bufSize, AOLPCSTR _pSrc)
		{
			int		offset;
			int		numChar;

			//�I�[����->�K�w����
			for (offset = 0; _pSrc[offset] != '\0'; ++offset);
			for (numChar = 0; _pSrc[offset] != '.'; --offset, ++numChar);

			//�t�@�C�������o
			ao_strcpy_s(_pOut, _bufSize, &_pSrc[offset + 1]);
		}

		/*!-----------------------------------------------------------
		//	@brief		float�z��p�̐F�擾�}�N��
		//	@param[in]	_r		�ԐF�l
		//	@param[in]	_g		�ΐF�l
		//	@param[in]	_b		�F�l
		//	@param[in]	_a		���ߒl
		//	@param[out]	_pOut	�o�͂���float�z��|�C���^
		//	@return		float�̔z��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	const float* GetColorF(float* _pOut, unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a)
		{
			_pOut[0] = (_r & 0xFF) / 255.0f;
			_pOut[1] = (_g & 0xFF) / 255.0f;
			_pOut[2] = (_b & 0xFF) / 255.0f;
			_pOut[3] = (_a & 0xFF) / 255.0f;
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		float�z��p�̐F�擾�}�N��
		//	@param[out]	_pOut		�o�͂���float�z��|�C���^
		//	@param[in]	_dwColor	32bit�J���[
		//	@return		float�̔z��|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/07/25
		------------------------------------------------------------*/
		inline	const float* GetColorF(float* _pOut, unsigned long _dwColor)
		{
			_pOut[0] = ((_dwColor >> 16) & 0xFF) / 255.0f;
			_pOut[1] = ((_dwColor >> 8) & 0xFF) / 255.0f;
			_pOut[2] = ((_dwColor)& 0xFF) / 255.0f;
			_pOut[3] = ((_dwColor >> 24) & 0xFF) / 255.0f;

			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@brief		�w�肵���^�ɕϊ����Ēl��߂�
		//	@param[in]	_p		�ΏۂƂȂ郁�����擪�|�C���^
		//	@param[in]	_size	���o���郁�����T�C�Y
		//	@return		�����������璊�o�������l�i���0�͕Ԃ��j
		//	@author		Tatsunori Aoyama
		//	@date		2013/11/27
		------------------------------------------------------------*/
		template<class T>
		inline	T	GetMemoryInValue(void* _p, int _size)
		{
			switch (_size)
			{
			case 1:	return (T)(*(__int8*)_p);
			case 2: return (T)(*(__int16*)_p);
			case 4: return (T)(*(__int32*)_p);
			case 8: return (T)(*(__int64*)_p);
			}

			return 0;
		}

		/*!-----------------------------------------------------------
		//	@brief		�|�C���^�̃L���X�g
		//	@param[in]	_p	�L���X�g����|�C���^
		//	@return		�L���X�g����^�̃|�C���^
		//	@author		Tatsunori Aoyama
		//	@date		2013/09/01
		------------------------------------------------------------*/
		template<class T>
		inline	T	pointer_cast(void* _p)
		{
			return static_cast<T>(_p);
		}

		/*!-----------------------------------------------------------
		//	@brief		COM�|�C���^�̈��S���
		//	@param[in]	_p	�������COM�|�C���^
		//	@return		S_OK�F��������@S_FALSE�F������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_RELEASE(T& _p)
		{
			if (_p)
			{
				_p->Release();
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}

		/*!-----------------------------------------------------------
		//	@brief		�������̈��S���
		//	@param[in]	_p	�������COM�|�C���^
		//	@return		S_OK�F��������@S_FALSE�F������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_DELETE(T& _p)
		{
			if (_p)
			{
				delete _p;
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}


		/*!-----------------------------------------------------------
		//	@brief		�z�񃁃����̈��S���
		//	@param[in]	_p	�������COM�|�C���^
		//	@return		S_OK�F��������@S_FALSE�F������s
		//	@author		Tatsunori Aoyama
		//	@date		2014/06/26
		------------------------------------------------------------*/
		template<class T>
		inline HRESULT SAFE_DELETE_ARRAY(T& _p)
		{
			if (_p)
			{
				delete[] _p;
				_p = nullptr;
				return S_OK;
			}

			return S_FALSE;
		}
	}
}

#endif
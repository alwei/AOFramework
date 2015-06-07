/*************************************************************
*	@file   	BaseShader.hpp
*	@brief  	BaseShader�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_BaseShader_hpp_	// �C���N���[�h�K�[�h
#define _Include_BaseShader_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"
#include"../Api/DirectX11BaseApi.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseShader
	//	@brief  �V�F�[�_�[���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class BaseShader : public DirectX11BaseApi
	{
	public:
		BaseShader(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext) :
			DirectX11BaseApi(_pd3dDevice,_pd3dDeviceContext)
		{
			this->shaderName.assign(_T("no name shader"));
		};

		virtual	~BaseShader()override{};

		/*!-----------------------------------------------------------
		//	@brief		�V�F�[�_�[���𖽖�
		//	@param[in]	_name	�ݒ肷�閽��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/14
		------------------------------------------------------------*/
		void Naming(ao::string _name){
			this->shaderName.assign(_name);
		}

		/*!-----------------------------------------------------------
		//	@brief		�V�F�[�_�[���擾
		//	@return		�V�F�[�_�[��
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/14
		------------------------------------------------------------*/
		AOLPSTR GetName() const{
			this->shaderName.c_str();
		}

	protected:
		ao::string shaderName;
	};
}

#endif // _Include_BaseShader_hpp_
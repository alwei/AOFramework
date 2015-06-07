/*************************************************************
*	@file   	BaseTexture.hpp
*	@brief  	BaseTexture�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_BaseTexture_hpp_	// �C���N���[�h�K�[�h
#define _Include_BaseTexture_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<d3d11.h>
#include"IResource.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseTexture
	//	@brief  �e�N�X�`�����N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class BaseTexture : public IResource
	{
	public:
		BaseTexture(){};
		virtual	~BaseTexture(){};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override {
			return ResourceType::TEXTURE;
		}

	public:
		string filePath;
		string fileName;
		SmartComPtr<ID3D11ShaderResourceView>	pSrView;
		SmartComPtr<ID3D11SamplerState>			pSampState;
	};
}

#endif // _Include_BaseTexture_hpp_
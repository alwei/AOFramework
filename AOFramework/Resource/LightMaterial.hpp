/*************************************************************
*	@file   	LightMaterial.hpp
*	@brief  	LightMaterial�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2015/04/29
*************************************************************/

#ifndef _Include_LightMaterial_hpp_	// �C���N���[�h�K�[�h
#define _Include_LightMaterial_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"IResource.hpp"
#include"../Math/MathDefine.hpp"
#include"../Math/GameArithmetic.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectinalLightMaterial
	//	@brief  ���C�g�}�e���A��
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class BaseLightMaterial : public IResource
	{
	public:
		/// �R���X�g���N�^
		BaseLightMaterial():lightColor(color::white),range(10000),isEnable(false)
		{
			float attn = 1.0f / 3.0f;
			this->attenuation[0] = attn;
			this->attenuation[1] = attn;
			this->attenuation[2] = attn;
		};

		/// �f�X�g���N�^
		virtual ~BaseLightMaterial()override{};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual	const ResourceType GetType() const override
		{
			return ResourceType::INVALID;
		}

		inline void operator=(const bool _isEnable)
		{
			this->isEnable = _isEnable;
		}

	public:
		Transform transform;
		color lightColor;
		std::array<float, 3> attenuation;
		float range;
		bool isEnable;
	};


	/*!-----------------------------------------------------------
	//	@class  DirectinalLightMaterial
	//	@brief  �f�B���N�V���i�����C�g�}�e���A��
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class DirectinalLightMaterial : public BaseLightMaterial
	{
	public:
		/// �R���X�g���N�^
		DirectinalLightMaterial():lightDir(float3::down){};

		/// �f�X�g���N�^
		~DirectinalLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::DIRECTIONAL_LIGHT;
		}

	public:
		float3 lightDir;
	};


	/*!-----------------------------------------------------------
	//	@class  PointLightMaterial
	//	@brief  �|�C���g���C�g�}�e���A��
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class PointLightMaterial : public BaseLightMaterial
	{
	public:
		/// �R���X�g���N�^
		PointLightMaterial(){};

		/// �f�X�g���N�^
		~PointLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::POINT_LIGHT;
		}
	};


	/*!-----------------------------------------------------------
	//	@class  PointLightMaterial
	//	@brief  �|�C���g���C�g�}�e���A��
	//	@author	Tatsunori Aoyama
	//	@date	2014/04/29
	------------------------------------------------------------*/
	class SpotLightMaterial : public BaseLightMaterial
	{
	public:
		/// �R���X�g���N�^
		SpotLightMaterial(){};

		/// �f�X�g���N�^
		~SpotLightMaterial()override final{};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const override final
		{
			ResourceType::SPOT_LIGHT;
		}
	};
}

#endif // _Include_LightMaterial_hpp_
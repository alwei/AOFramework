/*************************************************************
*	@file   	Material.hpp
*	@brief  	Material�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/19
*************************************************************/

#ifndef _Include_Material_hpp_	// �C���N���[�h�K�[�h
#define _Include_Material_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include"IResource.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  Material
	//	@brief  �}�e���A��
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/19
	------------------------------------------------------------*/
	class Material : public IResource
	{
	public:
		Material()
		{
			for (auto& it : this->diffuse)
			{
				it = 1.0f;
			}
			for (auto& it : this->ambient)
			{
				it = 0.5f;
			}
			for (auto& it : this->specular)
			{
				it = 1.0f;
			}
			for (auto& it : this->emissive)
			{
				it = 0;
			}
		};
		virtual	~Material(){};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/19
		------------------------------------------------------------*/
		virtual	const ResourceType GetType() const 
		{
			return ResourceType::MATERIAL;
		}

	public:
		std::array<float, 4> diffuse;
		std::array<float, 3> ambient;
		std::array<float, 3> specular;
		std::array<float, 3> emissive;
	};
}

#endif // _Include_Material_hpp_
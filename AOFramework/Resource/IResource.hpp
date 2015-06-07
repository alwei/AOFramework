/*************************************************************
*	@file   	IResource.hpp
*	@brief  	IResource�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/01
*************************************************************/

#ifndef _Include_IResource_hpp_	// �C���N���[�h�K�[�h
#define _Include_IResource_hpp_

namespace ao
{
	//------------------------------------------------------------
	// �C���N���[�h
	//------------------------------------------------------------
	enum class ResourceType : int
	{
		INVALID = -1,
		TEXTURE,
		MATERIAL,
		SPRITE,
		POINTSPRITE,
		GEOMETRY,
		ST_GEOMETRY,
		SK_GEOMETRY,
		BONE_RELATION,
		BONE,
		BONE_EX,
		IK_BONE,
		MOTION,
		MOTION_CTRL,
		BASE_MESH,
		ST_MESH,
		STA_MESH,
		SK_MESH,
		SUBSET,
		INSTANCE,
		RBURST_BUFFER,
		VERTEX_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER,
		COMPUTE_SHADER,
		UAV_BUFFER,
		RENDER_DESC,
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOT_LIGHT,
		LIGHTING_PKG,
		TYPE_MAX
	};

	/*!-----------------------------------------------------------
	//	@class  IResource
	//	@brief  ���\�[�X�̊���N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/01
	------------------------------------------------------------*/
	class IResource
	{
	public:
		IResource(){};
		virtual	~IResource(){};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		virtual	const ResourceType GetType() const = 0;
	};
}

#endif // _Include_IResource_hpp_
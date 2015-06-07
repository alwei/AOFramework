/*************************************************************
*	@file   	RenderResource.hpp
*	@brief  	RenderResource�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/08/10
*************************************************************/

#ifndef _Include_RenderResource_hpp_	// �C���N���[�h�K�[�h
#define _Include_RenderResource_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include<array>
#include<d3d11.h>
#include"IResource.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	struct VertexDesc{
		INT startSlot;
		INT numBuffers;
		std::array<ID3D11Buffer*, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT> pBuffers;
		std::array<UINT, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT> strides;
		std::array<UINT, D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT> offsets;
	};

	struct IndexDesc{
		INT offset;
		DXGI_FORMAT format;
		SmartComPtr<ID3D11Buffer> buffer;
	};

	struct DrawDesc{
		enum class Type : int{
			NO_INDEX,
			INDEX,
			INSTANCE
		};

		INT indexCount;
		INT instanceCount;
		INT startIndexLocation;
		INT baseVertexCount;
		INT baseIndexLocation;
		INT startInstanceLocation;
		Type drawType;
		D3D_PRIMITIVE_TOPOLOGY primitiveType;
	};

	/*!-----------------------------------------------------------
	//	@class  RenderResource
	//	@brief  �����_�����O���\�[�X
	//	@author	Tatsunori Aoyama
	//	@date	2014/08/10
	------------------------------------------------------------*/
	class RenderResource : IResource
	{
	public:
		RenderResource(){};
		virtual	~RenderResource(){};

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�̎擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/10
		------------------------------------------------------------*/
		ResourceType	GetType(){
			return ResourceType::RENDER_DESC;
		}

	public:
		VertexDesc	vertexDesc;
		IndexDesc	indexDesc;
		DrawDesc	drawDesc;
	};
}

#endif // _Include_RenderResource_hpp_
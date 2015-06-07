#ifndef __SHADER_RESOURCE_HPP__
#define __SHADER_RESOURCE_HPP__

#include<tchar.h>
#include<d3d11.h>
#include"IResource.hpp"
#include"../Define/SystemDefine.hpp"
#include"../Utility/SmartComPtr.hpp"

namespace ao
{
	/// �V�F�[�_�[���\�[�X���N���X
	class BaseShaderResource : public IResource
	{
	public:
		BaseShaderResource(){
			this->fileName.assign(_T("BaseShaderResourceName"));
			this->filePath.assign(_T("BaseShaderResourcePath"));
		}
		virtual ~BaseShaderResource(){}

		virtual const ResourceType GetType() const{
			return ResourceType::INVALID;
		}

	public:
		string	filePath;
		string	fileName;
	};

	/// ���_�V�F�[�_�[
	class VertexShader : public BaseShaderResource
	{
	public:
		VertexShader(){}
		~VertexShader(){}

		virtual const ResourceType GetType() const final{
			return ResourceType::VERTEX_SHADER;
		}
	public:
		SmartComPtr<ID3D11InputLayout> layout;
		SmartComPtr<ID3D11VertexShader> shader;
	};

	/// �W�I���g���V�F�[�_�[
	class GeometryShader : public BaseShaderResource
	{
	public:
		GeometryShader(){}
		~GeometryShader(){}

		virtual const ResourceType GetType() const final{
			return ResourceType::GEOMETRY_SHADER;
		}
	public:
		SmartComPtr<ID3D11GeometryShader> shader;
	};

	/// �s�N�Z���V�F�[�_�[
	class PixelShader : public BaseShaderResource
	{
	public:
		PixelShader(){}
		~PixelShader(){}

		virtual const ResourceType GetType() const final{
			return ResourceType::PIXEL_SHADER;
		}
	public:
		SmartComPtr<ID3D11PixelShader> shader;
	};

	/// �R���s���[�g�V�F�[�_�[
	class ComputeShader : public BaseShaderResource
	{
	public:
		ComputeShader(){}
		~ComputeShader(){}

		virtual const ResourceType GetType() const final{
			return ResourceType::COMPUTE_SHADER;
		}
	public:
		SmartComPtr<ID3D11ComputeShader> shader;
	};

	/// UAV�o�b�t�@
	class UavBuffer : public BaseShaderResource
	{
	public:
		UavBuffer(){}
		~UavBuffer(){}

		virtual const ResourceType GetType() const final{
			return ResourceType::UAV_BUFFER;
		}
	public:
		SmartComPtr<ID3D11Buffer> buffer;
		SmartComPtr<ID3D11UnorderedAccessView> view;
	};
}

#endif
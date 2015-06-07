/*************************************************************
*	@file   	InstanceBuffer.hpp
*	@brief  	InstanceBufferクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/17
*************************************************************/

#ifndef _Include_InstanceBuffer_hpp_	// インクルードガード
#define _Include_InstanceBuffer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<d3d11.h>
#include<DirectXMath.h>
#include"IResource.hpp"
#include"../Resource/Mesh.hpp"
#include"../Utility/SmartComPtr.hpp"
#include"../Shader/Function/ShaderFunction.hpp"

using namespace DirectX;

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  BaseInstanceBuffer
	//	@brief  インスタンスバッファ
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/17
	------------------------------------------------------------*/
	class BaseInstanceBuffer : public IResource
	{
	public:
		BaseInstanceBuffer() :
			numInstance(0),
			numPushRender(0),
			pMesh(nullptr),
			cMatrix(nullptr),
			lMatrix(nullptr){}
		virtual ~BaseInstanceBuffer(){};

		/*!-----------------------------------------------------------
		//	@brief		行列バッファ取得
		//  @return     行列バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		ID3D11Buffer* GetCameraMatrixBuffer(){
			return this->cMatrixBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		ライト行列取得
		//	@return		ライト行列バッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/19
		------------------------------------------------------------*/
		ID3D11Buffer* GetLightMatrixBuffer(){
			return this->lMatrixBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		インスタンス数取得
		//  @return     インスタンス数
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		int GetNumInstance(){
			return this->numInstance;
		}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		virtual const ResourceType GetType() const override{
			return ResourceType::INSTANCE;
		}

	public:
		int numInstance;
		int numPushRender;
		BaseMesh* pMesh;
		XMFLOAT4X4* cMatrix;
		XMFLOAT4X4* lMatrix;
		SmartComPtr<ID3D11Buffer> cMatrixBuffer;
		SmartComPtr<ID3D11Buffer> lMatrixBuffer;
	};

	/*!-----------------------------------------------------------
	//	@class  MeshInstanceBuffer
	//	@brief  メッシュインスタンスバッファ
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/17
	------------------------------------------------------------*/
	template<class T>
	class MeshInstanceBuffer final : public BaseInstanceBuffer
	{
	public:
		/// コンストラクタ
		MeshInstanceBuffer() :
			pInstance(nullptr),
			dataSize(0){};

		/// デストラクタ
		~MeshInstanceBuffer(){
			if (this->pInstance)
				delete[] this->pInstance;
			if (this->cMatrix)
				delete[] this->cMatrix;
			if (this->lMatrix)
				delete[] this->lMatrix;
		};

		/*!-----------------------------------------------------------
		//	@brief		作成
		//  @param[in]  _pd3dDevice		デバイスポインタ
		//	@param[in]	_numInstance	インスタンス数
		//	@return		S_OK：作成成功　それ以外：作成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		HRESULT	Create(
			ID3D11Device* _pd3dDevice,
			u32 _numInstance)
		{
			this->numInstance = _numInstance;
			this->dataSize = sizeof(T) * this->numInstance;

			HRESULT hr(E_FAIL);
			hr = ao::CreateVertexBuffer(
				_pd3dDevice,
				this->cMatrixBuffer.ToCreator(),
				sizeof(XMFLOAT4X4)*this->numInstance,
				nullptr,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE);
#ifdef __AO_DEBUG__
			if (FAILED(hr))
			{
				ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[InstanceBuffer::Create]Failed create camera matrix data.\n"));
				return E_FAIL;
			}
#endif

			hr = ao::CreateVertexBuffer(
				_pd3dDevice,
				this->lMatrixBuffer.ToCreator(),
				sizeof(XMFLOAT4X4)*this->numInstance,
				nullptr,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE);
#ifdef __AO_DEBUG__
			if (FAILED(hr))
			{
				ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[InstanceBuffer::Create]Failed create light matrix data.\n"));
				return E_FAIL;
			}
#endif

			hr = ao::CreateVertexBuffer(
				_pd3dDevice,
				this->pGpuInstanceBuffer.ToCreator(),
				this->dataSize,
				nullptr,
				D3D11_USAGE_DYNAMIC,
				D3D11_CPU_ACCESS_WRITE);
#ifdef __AO_DEBUG__
			if (FAILED(hr))
			{
				ao::debug::PrintfColor(ConsoleColor::H_RED, _T("[InstanceBuffer::Create]Failed create instance data.\n"));
				return E_FAIL;
			}
#endif

			if (this->cMatrix)
				delete[] this->cMatrix;
			this->cMatrix = new XMFLOAT4X4[this->numInstance];
			if (this->lMatrix)
				delete[] this->lMatrix;
			this->lMatrix = new XMFLOAT4X4[this->numInstance];
			if (this->pInstance)
				delete[] this->pInstance;
			this->pInstance = new T[this->numInstance];

			return S_OK;
		}

		/*!-----------------------------------------------------------
		//	@brief		インスタンスバッファ取得
		//  @return     インスタンスバッファ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		ID3D11Buffer* GetInstanceBuffer(){
			return this->pGpuInstanceBuffer.GetComPtr();
		}

		/*!-----------------------------------------------------------
		//	@brief		インスタンスのサイズ取得
		//  @return     インスタンスデータサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		u32 GetDataSize(){
			return this->dataSize;
		}

		/*!-----------------------------------------------------------
		//	@brief		インスタンス数取得
		//  @return     インスタンス数
		//	@author		Tatsunori Aoyama
		//	@date		2014/09/17
		------------------------------------------------------------*/
		u32 GetNumInstance(){
			return this->numInstance;
		}

	public:
		T* pInstance;
		u32 dataSize;
		SmartComPtr<ID3D11Buffer> pGpuInstanceBuffer;
	};
}

#endif // _Include_InstanceBuffer_hpp_
/*************************************************************
*	@file   	RenderBurstBuffer.hpp
*	@brief  	RenderBurstBufferクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/13
*************************************************************/

#ifndef _Include_RenderBurstBuffer_hpp_	// インクルードガード
#define _Include_RenderBurstBuffer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include"../Define/SystemDefine.hpp"
#include"../Math/MathDefine.hpp"
#include"../Resource/IResource.hpp"
#include"Mesh.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@struct  RenderBurstBuffer
	//	@brief  レンダリングバーストバッファ
	//	@author	Tatsunori Aoyama
	//	@date	2015/05/13
	------------------------------------------------------------*/
	class RenderBurstBuffer : public IResource
	{
	public:
		RenderBurstBuffer(u32 _num,BaseMesh* _pMesh) : numInstance(_num),pMesh(_pMesh){}
		virtual ~RenderBurstBuffer(){}

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/01
		------------------------------------------------------------*/
		const ResourceType GetType() const final override
		{
			return ResourceType::RBURST_BUFFER;
		}

		template <class T>
		class GpuBuffer
		{
		public:
			GpuBuffer() : pCpuBuffer(nullptr), dataSize(0){}

			~GpuBuffer()
			{
				delete[] this->pCpuBuffer;
			}

			void Create(u32 _num)
			{
				if (this->pCpuBuffer)
				{
					delete[] this->pCpuBuffer;
				}
				this->pCpuBuffer = new T[_num];
				this->dataSize = sizeof(T)*_num;
			}

			T& operator[](const u32 _index)
			{
				return this->pCpuBuffer[_index];
			}

			ID3D11Buffer** ToBufferCreator()
			{
				return this->pBuffer.ToCreator();
			}

			ID3D11ShaderResourceView** ToSrViewCreator()
			{
				return this->pSrView.ToCreator();
			}

			ID3D11Buffer* GetGpuBuffer()
			{
				return this->pBuffer.GetComPtr();
			}

			ID3D11ShaderResourceView* GetSrView()
			{
				return this->pSrView.GetComPtr();
			}

			T* GetCpuBuffer() const
			{
				return this->pCpuBuffer;
			}

			const u32 GetDataSize() const 
			{
				return this->dataSize;
			}

			const u32 GetNumBuffer() const
			{
				return this->numBuffer;
			}

		private:
			u32 dataSize;
			u32 numBuffer;
			T* pCpuBuffer;
			SmartComPtr<ID3D11Buffer> pBuffer;
			SmartComPtr<ID3D11ShaderResourceView> pSrView;
		};

	public:
		struct Mapping0
		{
			float4		localLightDir;
			float4		localEyePos;
			Color		sufColor;
		};

		struct Mapping1
		{
			Matrix matWVP;
		};

		struct Mapping2
		{
			Matrix matLWVP;
		};

		u32			numPushRender;
		u32			numInstance;
		BaseMesh*	pMesh;
		GpuBuffer<Mapping0>	gpuBuffer0;
		GpuBuffer<Mapping1>	gpuBuffer1;
		GpuBuffer<Mapping2>	gpuBuffer2;
	};
}

#endif // _Include_RenderBurstBuffer_hpp_

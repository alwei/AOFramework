/*************************************************************
*	@file   	ConstVectorBuffer.hpp
*	@brief  	ConstVectorBufferクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/12/21
*************************************************************/

#ifndef _Include_ConstVectorBuffer_hpp_	// インクルードガード
#define _Include_ConstVectorBuffer_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<D3D11.h>
#include"Buffer.hpp"
#include"../Utility/SmartComPtr.hpp"


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  ConstVectorBuffer
	//	@brief  テンプレート定数バッファクラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/12/21
	------------------------------------------------------------*/
	template< class T1>
	class ConstVectorBuffer final
	{
	public:
		/// コンストラクタ
		ConstVectorBuffer() : mapData(nullptr){}

		/// デストラクタ
		~ConstVectorBuffer(){
			if (this->mapData)
			{
				delete[] this->mapData;
			}
		}

		/*!-----------------------------------------------------------
		//	@brief		ポインタ登録
		//	@param[in]	_pd3dDevice			デバイスポインタ
		//	@param[in]	_pd3dDeviceContext	デバイスコンテキストポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		void	Register(
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext){
			this->pd3dDevice = _pd3dDevice;
			this->pd3dDeviceContext = _pd3dDeviceContext;
		}

		/*!-----------------------------------------------------------
		//	@brief		バッファ生成
		//	@param[in]	_num			生成数
		//	@param[in]	_usage			使用用途[省略可](D3D11_USAGE_DEFAULT:D3D11_USAGE_IMMUTABLE:D3D11_USAGE_DYNAMIC:D3D11_USAGE_STAGIN)
		//	@param[in]	_cpuAccessFlag	CPUアクセスフラグ[省略可](D3D11_CPU_ACCESS_READ:D3D11_CPU_ACCESS_WRITE)
		//	@return		S_OK：生成成功　それ以外：生成失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT		Create(
			const UINT _num,
			const D3D11_USAGE _usage = D3D11_USAGE_DYNAMIC,
			const UINT _cpuAccessFlag = D3D11_CPU_ACCESS_WRITE)
		{
			this->numBuffer = _num;
			this->mapData = new T1[this->numBuffer];
			::SecureZeroMemory(this->mapData, sizeof(T1)*_num);
			return this->gpuBuffer.Create(this->mapData, sizeof(T1)*_num, _usage, _cpuAccessFlag, this->pd3dDevice);
		}

		/*!-----------------------------------------------------------
		//	@brief		VertexShaderの定数にマッピング
		//	@param[in]	_slotId			スロット番号[省略可]
		//	@param[in]	_numBuffer		バッファ数[省略可]
		//	@param[in]	_mapFlag		マップフラグ[省略可]
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	MapVertex(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(this->mapData, sizeof(T1)*this->numBuffer, _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->VSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		GeometryShaderの定数にマッピング
		//	@param[in]	_slotId			スロット番号[省略可]
		//	@param[in]	_numBuffer		バッファ数[省略可]
		//	@param[in]	_mapFlag		マップフラグ[省略可]
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	MapGeometry(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(this->mapData, sizeof(T1)*this->numBuffer, _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->GSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		PixelShaderの定数にマッピング
		//	@param[in]	_slotId				スロット番号[省略可]
		//	@param[in]	_numBuffer			バッファ数[省略可]
		//	@param[in]	_mapFlag			マップフラグ[省略可]
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	MapPixel(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(this->mapData, sizeof(T1)*this->numBuffer, _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->PSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		ComputeShaderの定数にマッピング
		//	@param[in]	_slotId				スロット番号[省略可]
		//	@param[in]	_numBuffer			バッファ数[省略可]
		//	@param[in]	_mapFlag			マップフラグ[省略可]
		//	@return		S_OK：マッピング成功　それ以外：マッピング失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		HRESULT	MapCompute(
			const UINT _slotId = 0,
			const UINT _numBuffer = 1,
			const D3D11_MAP _mapFlag = D3D11_MAP_WRITE_DISCARD)
		{
			HRESULT hr = this->gpuBuffer.Map(this->mapData, sizeof(T1)*this->numBuffer, _mapFlag, this->pd3dDeviceContext);
			this->pd3dDeviceContext->CSSetConstantBuffers(_slotId, _numBuffer, this->gpuBuffer.buffer.GetComPtrPtr());
			return hr;
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子のオーバーロード
		//	@return		true：一致　false：不一致
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		bool operator==(int addr)
		{
			return ((int)this->mapData == addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子のオーバーロード
		//	@return		true：不一致　false：一致
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		bool operator!=(int addr)
		{
			return ((int)this->mapData != addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子のオーバーロード
		//	@return		true：一致　false：不一致
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		bool operator==(void* addr)
		{
			return ((int)this->mapData == (int)addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		比較演算子のオーバーロード
		//	@return		true：不一致　false：一致
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		bool operator!=(void* addr)
		{
			return ((int)this->mapData != (int)addr);
		}

		/*!-----------------------------------------------------------
		//	@brief		アロー演算子のオーバーロード
		//	@return		マップデータのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		T1* operator->()
		{
			return this->mapData;
		}

		/*!-----------------------------------------------------------
		//	@brief		間接参照演算子のオーバーロード
		//	@return		マップデータのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		T1& operator*()
		{
			return this->mapData[0];
		}

		/*!-----------------------------------------------------------
		//	@brief		配列参照演算子のオーバーロード
		//	@return		マップデータのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		T1& operator[](int n) const
		{
			return this->mapData[n];
		}

		/*!-----------------------------------------------------------
		//	@brief		&演算子のオーバーロード
		//	@return		マップデータのポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		T1* operator&(const T1& _inst)
		{
			return &_inst;
		}

		/*!-----------------------------------------------------------
		//	@brief		バッファの先頭ポインタ
		//	@return		バッファの先頭ポインタ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/22
		------------------------------------------------------------*/
		T1* data() const
		{
			return this->mapData;
		}

		/*!-----------------------------------------------------------
		//	@brief		バッファ数取得
		//	@return		バッファ数
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		const UINT GetBufferNum() const
		{
			return this->numBuffer;
		}

		/*!-----------------------------------------------------------
		//	@brief		バッファサイズ取得
		//	@return		バッファサイズ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/21
		------------------------------------------------------------*/
		const UINT GetBufferSize() const
		{
			return sizeof(T1)*this->numBuffer;
		}

	private:
		T1*	mapData;
		Buffer gpuBuffer;
		UINT numBuffer;
		SmartComPtr<ID3D11Device> pd3dDevice;
		SmartComPtr<ID3D11DeviceContext> pd3dDeviceContext;

	private:
		ConstVectorBuffer(const ConstVectorBuffer& inst) = delete;		///< コピーコンストラクタ禁止
		explicit ConstVectorBuffer(T1 type) = delete;					///< 暗黙の型変換によるコンストラクタ禁止
		void operator=(T1 pInst) = delete;								///< 明示的な代入演算子禁止
		void operator=(const ConstVectorBuffer& inst) = delete;			///< 明示的な代入演算子禁止
	};
}

#endif // _Include_ConstVectorBuffer_hpp_
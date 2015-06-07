/*************************************************************
*	@file   	DirectX11Profiler.hpp
*	@brief  	DirectX11Profilerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2015/05/17
*************************************************************/

#ifndef _Include_DirectX11Profiler_hpp_	// インクルードガード
#define _Include_DirectX11Profiler_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<array>
#include"DirectX11BaseApi.hpp"
#include"../Math/MathDefine.hpp"
#include"../Resource/ConstBuffer.hpp"
#include"../Resource/ConstVectorBuffer.hpp"
#include"../System/CountTime.hpp"


enum class ProcessLoadType : u8
{
	INIT = 0,
	LOAD,
	UPDATE,
	RENDER_COMMAND,
	RENDER_BURST,
	RENDER_PRESENT,
	RENDER_CLEAR,
	MAX,
	ALL
};

static const u8 PROC_LOAD_MAX_TYPE = static_cast<u8>(ProcessLoadType::MAX);


namespace ao
{
	class CountTime;
	class DirectX11RenderTarget;
}


typedef std::array<ao::CountTime*, static_cast<u8>(ProcessLoadType::MAX)> ProcLoadTimeArray;
typedef std::array<ao::DirectX11RenderTarget*, static_cast<u8>(ProcessLoadType::MAX)> ProcRenderArray;


namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  DirectX11Profiler
	//	@brief  プロファイラシステム
	//	@author	Tatsunori Aoyama
	//	@date	2014/05/17
	------------------------------------------------------------*/
	class DirectX11Profiler : public DirectX11BaseApi
	{
	public:
		/// コンストラクタ
		DirectX11Profiler(
			const u32 _width,
			const u32 _height,
			SmartComPtr<ID3D11Device> _pd3dDevice,
			SmartComPtr<ID3D11DeviceContext> _pd3dDeviceContext);

		/// デストラクタ
		~DirectX11Profiler();

		/*!-----------------------------------------------------------
		//	@brief		プロファイルタイプを指定
		//	@param[in]	_setProfileIndex	プロファイルインデックス
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispProfileIndex(ProcessLoadType _setProfileIndex = ProcessLoadType::RENDER_BURST)
		{
			this->selectedProfileIndex = _setProfileIndex;
		}

		/*!-----------------------------------------------------------
		//	@brief		レンダリングする画面座標
		//	@param[in]	_x	X座標
		//	@param[in]	_y	座標
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispPosition(const f32 _x, const f32 _y)
		{
			this->dispPosition.x = _x;
			this->dispPosition.y = _y;
		}

		/*!-----------------------------------------------------------
		//	@brief		表示するプロファイラ結果のサイズ
		//	@param[in]	_width	横幅
		//	@param[in]	_height	縦幅
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetDispScale(const f32 _width, const f32 _height)
		{
			this->dispScale.x = _width;
			this->dispScale.y = _height;
		}

		/*!-----------------------------------------------------------
		//	@brief		グラフのカラー設定
		//	@param[in]	_setGraphColor	グラフ色
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetGraphColor(color _setGraphColor = color::white)
		{
			this->graphColor = _setGraphColor;
		}

		/*!-----------------------------------------------------------
		//	@brief		背景カラー設定
		//	@param[in]	_setBackColor	背景カラー
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/23
		------------------------------------------------------------*/
		void SetBackColor(color _setBackColor = color::transparent)
		{
			this->backColor = _setBackColor;
		}

		/*!-----------------------------------------------------------
		//	@brief		計測するフレームレートを設定
		//	@param[in]	_setFrameRate	設定するフレームレート[省略可]
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void SetUseRecordFrameRate(const u32 _setFrameRate = 60)
		{
			this->useRecordFrameRate = _setFrameRate;
		}

		/*!-----------------------------------------------------------
		//	@brief		計測区間の開始
		//	@param[in]	_loadType	負荷計測タイプ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void Begin(ProcessLoadType _procLoadType)
		{
			this->pProcLoadTime[static_cast<u8>(_procLoadType)]->Begin();
		}

		/*!-----------------------------------------------------------
		//	@brief		計測区間の終了
		//	@param[in]	_loadType	負荷計測タイプ
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/21
		------------------------------------------------------------*/
		inline void End(ProcessLoadType _procLoadType)
		{
			this->pProcLoadTime[static_cast<u8>(_procLoadType)]->End();
		}

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	_pGraphicsRenderer	レンダラー
		//	@param[in]	_pGraphicsState		グラフィックステート
		//	@param[in]	_pShader			シェーダー管理クラス
		//	@return		S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/19
		------------------------------------------------------------*/
		HRESULT Initialize(
		class DirectX11GraphicsRenderer* const _pGraphicsRenderer,
		class DirectX11GraphicsState* const _pGraphicsState,
		class DirectX11Shader* const pShader);

		/*!-----------------------------------------------------------
		//	@brief		プロファイラ結果のレンダリング
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/18
		------------------------------------------------------------*/
		void RenderResult();

	private:
		struct GeometryConstBuffer
		{
			float4 procLoad;
		};

		typedef ConstVectorBuffer<GeometryConstBuffer> LoadProcBuffer;

		u32 renderWidth;
		u32 renderHeight;
		u32 recordCount;
		u32 useRecordFrameRate;
		float2 dispPosition;
		float2 dispScale;
		color graphColor;
		color backColor;
		ProcessLoadType selectedProfileIndex;
		class DirectX11Shader* pShader;
		class DirectX11GraphicsRenderer* pGraphicsRenderer;
		class DirectX11GraphicsState* pGraphicsState;
		class VertexShader* pVertexShader;
		class GeometryShader* pGeometryShader;
		class PixelShader* pPixelShader;
		ProcLoadTimeArray pProcLoadTime;
		ProcRenderArray pProcLoadRT;
		ConstBuffer<float4> geomConstBuffer0;
		std::array<LoadProcBuffer, PROC_LOAD_MAX_TYPE> geomConstBuffer1;
		SmartComPtr<ID3D11Buffer> pVertexBuffer;

	private:
		/*!-----------------------------------------------------------
		//	@brief		負荷結果をグラフでレンダリング
		//	@param[in]	_pRenderTarget	描画するれんだーターゲット
		//  @param[in]  _pProcTime		計測したプロセス時間
		//	@author		Tatsunori Aoyama
		//	@date		2015/05/18
		------------------------------------------------------------*/
		void RenderProcessLoad(
		class DirectX11RenderTarget* const _pRenderTarget,
		class CountTime* const _pProcTime,
			LoadProcBuffer* const pMapGeomBuffer1);
	};
}

#endif // _Include_DirectX11Profiler_hpp_
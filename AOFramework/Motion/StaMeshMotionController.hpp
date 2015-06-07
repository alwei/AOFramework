/*************************************************************
*	@file   	StaMeshMotionController.hpp
*	@brief  	StaMeshMotionControllerクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/31
*************************************************************/

#ifndef _Include_StaMeshMotionController_hpp_	// インクルードガード
#define _Include_StaMeshMotionController_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<Windows.h>
#include<vector>
#include"../Math/Math.hpp"
#include"IMeshMotionController.hpp"

namespace ao
{
	struct StaFrame{
		std::vector<XMFLOAT4X4> mtx;
	};
	struct StaMotion{
		int beginFrame;				///< 開始フレーム
		int endFrame;				///< 終了フレーム
		int frameCount;				///< フレーム数
		std::vector<StaFrame> frame;
	};

	typedef std::vector<StaMotion> StaMotionList;

	struct StaMotionData{
		int numMesh;				///< メッシュ数
		StaMotionList list;			///< モーションリスト
	};

	/*!-----------------------------------------------------------
	//	@class  StaMeshMotionController
	//	@brief  静的メッシュモーション制御クラス
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/31
	------------------------------------------------------------*/
	class StaMeshMotionController : public IMeshMotionController
	{
	public:
		/// コンストラクタ
		StaMeshMotionController();

		/// デストラクタ
		~StaMeshMotionController();

		/*!-----------------------------------------------------------
		//	@brief		初期化
		//	@param[in]	なし
		//  @return     S_OK：初期化成功　それ以外：初期化失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		HRESULT Initialize();

		/*!-----------------------------------------------------------
		//	@brief		更新
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/31
		------------------------------------------------------------*/
		void Update();

		/*!-----------------------------------------------------------
		//	@brief		モーション変更
		//	@param[in]	_index	モーションインデックス
		//	@param[in]	_type	モーションタイプ
		//	@return		TRUE：変更成功　FALSE：変更失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/01
		------------------------------------------------------------*/
		BOOL ChangeMotion(int _index, MotionType _type);

		/*!-----------------------------------------------------------
		//	@brief		計算された行列データの取得
		//	@param[in]	_pOut	出力する行列ポインタ
		//	@param[in]	_size	出力先のバッファサイズ
		//	@return		TRUE：出力成功　FALSE：出力失敗
		//	@author		Tatsunori Aoyama
		//	@date		2014/11/01
		------------------------------------------------------------*/
		BOOL GetMoitonData(XMFLOAT4X4* _pOut, const size_t _size);

		/*!-----------------------------------------------------------
		//	@brief		モーションが再生中かどうかチェック
		//	@return		TRUE：再生中　FALSE：再生中ではない
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		BOOL IsPlayNow();

		/*!-----------------------------------------------------------
		//	@brief		再生処理
		//	@param[in]	初期化するフレーム
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		void Play(int _setFrame = -1);

		/*!-----------------------------------------------------------
		//	@brief		停止処理
		//	@param[in]	_初期化するフレーム
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		void Stop(int _setFrame=-1);

		/*!-----------------------------------------------------------
		//	@brief		現在のモーションインデックスを取得
		//	@return		モーションインデックス
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMotionIndex() const {
			return this->nowMotionIndex;
		}
		
		/*!-----------------------------------------------------------
		//	@brief		モーションのタイプ取得
		//	@return		モーションタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		MotionType GetNowMotionType(){
			return this->motionType;
		}

		/*!-----------------------------------------------------------
		//	@brief		現在のフレーム取得
		//	@return		現在のモーションフレーム
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMotionFrame(){
			return this->nowFrame;
		}

		/*!-----------------------------------------------------------
		//	@brief		現在のモーション最大フレーム取得
		//	@return		現在のモーション最大フレーム
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNowMaxMotionFrame(){
			return this->motionData.list[this->nowMotionIndex].frameCount;
		}

		/*!-----------------------------------------------------------
		//	@brief		メッシュ数取得
		//	@return		メッシュ数
		//	@author		Tatsunori Aoyama
		//	@date		2014/12/06
		------------------------------------------------------------*/
		int GetNumMesh(){
			return this->numMesh;
		}

	public:
		BOOL enable;
		int numMesh;
		int nowFrame;
		int nowMotionIndex;
		int nextMotionIndex;
		MotionType motionType;
		std::vector<XMFLOAT4X4> mapData;
		StaMotionData motionData;
	};
}

#endif // _Include_StaMeshMotionController_hpp_
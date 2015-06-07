/*************************************************************
*	@file   	Motion.hpp
*	@brief  	Motionクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Motion_hpp_	// インクルードガード
#define _Include_Motion_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<list>
#include<vector>
#include"IResource.hpp"
#include"../Math/MathDefine.hpp"
#include"../Define/SystemDefine.hpp"

namespace ao
{
	using namespace math;

	/*!-----------------------------------------------------------
	//	@class  Motion
	//	@brief  モーション
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class Motion : public IResource
	{
	public:
		Motion();
		~Motion();

		/*!-----------------------------------------------------------
		//	@brief		リソースタイプ取得
		//	@return		リソースタイプ
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::MOTION;
		}

		struct KeyFrame{
			string boneName;				///< 影響するボーン名
			DWORD frameNo;					///< フレーム番号
			float3 position;				///< 位置座標
			float4 localRotation;			///< 回転クォータニオン
			float4 bezierInterpolation[4];	///< ベジェ曲線によるXYZRの補間パラメータ(0:X 1:Y 2:Z 3:回転)

			bool operator < (const KeyFrame& _inst) const {
				return this->frameNo > _inst.frameNo;
			}
		};

		typedef std::list<KeyFrame>::iterator IKey;
		struct BoneMotion{
			DWORD nowKeyIndex;
			string motionName;
			std::list<KeyFrame> keyFrameList;
			float4 nowBoneRotation;
			float3 nowBonePosition;
			std::vector< IKey > iKeyList;
		};

		struct MotionData{
			DWORD maxMotionTime;					///< 最大モーションフレーム
			std::vector< BoneMotion > motionList;	///< 全ボーンのモーションデータ
		};
	};
}

#endif // _Include_Motion_hpp_
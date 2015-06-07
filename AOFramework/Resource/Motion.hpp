/*************************************************************
*	@file   	Motion.hpp
*	@brief  	Motion�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Motion_hpp_	// �C���N���[�h�K�[�h
#define _Include_Motion_hpp_

//------------------------------------------------------------
// �C���N���[�h
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
	//	@brief  ���[�V����
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class Motion : public IResource
	{
	public:
		Motion();
		~Motion();

		/*!-----------------------------------------------------------
		//	@brief		���\�[�X�^�C�v�擾
		//	@return		���\�[�X�^�C�v
		//	@author		Tatsunori Aoyama
		//	@date		2014/10/04
		------------------------------------------------------------*/
		virtual ResourceType GetType() final{
			return ResourceType::MOTION;
		}

		struct KeyFrame{
			string boneName;				///< �e������{�[����
			DWORD frameNo;					///< �t���[���ԍ�
			float3 position;				///< �ʒu���W
			float4 localRotation;			///< ��]�N�H�[�^�j�I��
			float4 bezierInterpolation[4];	///< �x�W�F�Ȑ��ɂ��XYZR�̕�ԃp�����[�^(0:X 1:Y 2:Z 3:��])

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
			DWORD maxMotionTime;					///< �ő僂�[�V�����t���[��
			std::vector< BoneMotion > motionList;	///< �S�{�[���̃��[�V�����f�[�^
		};
	};
}

#endif // _Include_Motion_hpp_
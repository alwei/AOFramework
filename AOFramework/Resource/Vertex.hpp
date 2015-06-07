/*************************************************************
*	@file   	Vertex.hpp
*	@brief  	Vertex�N���X�w�b�_�[
*	@author		Tatsunori Aoyama
*	@date		2014/10/04
*************************************************************/

#ifndef _Include_Vertex_hpp_	// �C���N���[�h�K�[�h
#define _Include_Vertex_hpp_

//------------------------------------------------------------
// �C���N���[�h
//------------------------------------------------------------
#include"IResource.hpp"

namespace ao
{
	/*!-----------------------------------------------------------
	//	@class  StVertex
	//	@brief  �ÓI���_�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class StVertex
	{
	public:
		union{
			struct {
				float x, y, z;
				float nx, ny, nz;
				float u, v;
			};
			struct {
				float data[8];
			};
		};
	public:
		StVertex(){};
		~StVertex(){};
	};

	/*!-----------------------------------------------------------
	//	@class  SkVertex
	//	@brief  ���I���_�N���X
	//	@author	Tatsunori Aoyama
	//	@date	2014/10/04
	------------------------------------------------------------*/
	class SkVertex
	{
	public:
		union{
			struct{
				float	x, y, z;
				float	nx, ny, nz;
				float	u, v;
				float	weight[3];
				int		index[4];
			};
			struct{
				float data[15];
			};
		};

	public:
		SkVertex(){};
		~SkVertex(){};
	};

}

#endif // _Include_Vertex_hpp_
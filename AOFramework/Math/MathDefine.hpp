/*************************************************************
*	@file   	MathDefine.hpp
*	@brief  	MathDefineクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2014/09/03
*************************************************************/

#ifndef _Include_MathDefine_hpp_	// インクルードガード
#define _Include_MathDefine_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<xutility>
#include<Windows.h>
#include"Math.hpp"


namespace ao
{
	using namespace ao;

	/*!-----------------------------------------------------------
	//	@class  float2
	//	@brief  2次元ベクトル
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class float2
	{
	public:
		float2() : x(0), y(0){}
		float2(float _x, float _y) : x(_x), y(_y){}
		float2(float* _array) : x(_array[0]), y(_array[1]){}
		float2(const XMFLOAT2& _xm) : xm(_xm){}
		float2(const XMFLOAT2&& _xm) : xm(std::move(_xm)){}		
		float2(const float2& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
		}
		float2(const float2&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
		}
		~float2(){};

		inline void Normalize()
		{
			Vector2Normalize(&this->xm, this->xm);
		}

		inline void Add(float _s)
		{
			this->x += _s;
			this->y += _s;
		}

		inline void Add(float _x, float _y)
		{
			this->x += _x;
			this->y += _y;
		}

		inline void Add(const XMFLOAT2& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
		}

		inline void Add(const XMFLOAT2&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
		}

		inline void Add(const float2& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
		}

		inline void Add(const float2&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
		}

		inline void Mul(float _s){
			this->x *= _s;
			this->y *= _s;
		}

		inline void Mul(float _x, float _y)
		{
			this->x *= _x;
			this->y *= _y;
		}

		inline void Mul(const XMFLOAT2& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
		}

		inline void Mul(const XMFLOAT2&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
		}

		inline void Mul(const float2& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
		}

		inline void Mul(const float2&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
		}


		inline float Length()
		{
			return Vector2Length(this->xm);
		}

		inline float Dot(float _x, float _y)
		{
			return Vector2Dot(this->xm, XMFLOAT2(_x, _y));
		}

		inline float Dot(const XMFLOAT2& _v)
		{
			return Vector2Dot(this->xm, _v);
		}

		inline float Dot(const XMFLOAT2&& _v)
		{
			return Vector2Dot(this->xm, _v);
		}

		inline float Dot(const float2& _v)
		{
			return Vector2Dot(this->xm, _v.xm);
		}

		inline float Dot(const float2&& _v)
		{
			return Vector2Dot(this->xm, _v.xm);
		}

		inline void Cross(float _x, float _y)
		{
			Vector2Cross(&this->xm, this->xm, XMFLOAT2(_x, _y));
		}

		inline void Cross(const XMFLOAT2& _v)
		{
			Vector2Cross(&this->xm, this->xm, _v);
		}

		inline void Cross(const XMFLOAT2&& _v)
		{
			Vector2Cross(&this->xm, this->xm, _v);
		}

		inline void Cross(const float2& _v)
		{
			Vector2Cross(&this->xm, this->xm, _v.xm);
		}

		inline void Cross(const float2&& _v)
		{
			Vector2Cross(&this->xm, this->xm, _v.xm);
		}

		inline void TransformCoord(const XMFLOAT4X4& _mtx)
		{
			Vector2TransformCoord(&this->xm, this->xm, _mtx);
		}

		inline void TransformCoord(const XMFLOAT4X4&& _mtx)
		{
			Vector2TransformCoord(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(const XMFLOAT4X4& _mtx)
		{
			Vector2TransformNormal(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(const XMFLOAT4X4&& _mtx)
		{
			Vector2TransformNormal(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(float _x, float _y, float _z, float _w)
		{
			Vector2TransformNormal(&this->xm, this->xm, XMFLOAT4(_x, _y, _z, _w));
		}

		inline void TransformNormal(const XMFLOAT4& _qtn)
		{
			Vector2TransformNormal(&this->xm, this->xm, _qtn);
		}

		inline void TransformNormal(const XMFLOAT4&& _qtn)
		{
			Vector2TransformNormal(&this->xm, this->xm, _qtn);
		}

	public:
		void operator=(const float2& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
		}

		void operator=(const float2&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
		}

		float operator[](const UINT n) const
		{
			return this->f[n];
		}

	public:
		static const float2 one;
		static const float2 zero;
		static const float2 up;
		static const float2 down;
		static const float2 right;
		static const float2 left;

	public:
		union
		{
			struct{ float x, y; };
			struct{ float u, v; };
			struct{ float f[2]; };
			struct{ XMFLOAT2 xm; };
		};
	};

	typedef float2 Vector2;
	typedef float2 aoFloat2;
	typedef float2 aoVector2;

	/*!-----------------------------------------------------------
	//	@class  float3
	//	@brief  3次元ベクトル
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class float3
	{
	public:
		float3() :x(0), y(0), z(0){};
		float3(float _x, float _y, float _z) : x(_x),y(_y),z(_z){}
		float3(float* _array) : x(_array[0]),y(_array[1]),z(_array[2]){}
		float3(const XMFLOAT3& _xm) : xm(_xm){}
		float3(const XMFLOAT3&& _xm) : xm(std::move(_xm)){}
		float3(const float3& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
		}
		float3(const float3&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
		}
		~float3(){};

		inline void Normalize()
		{
			Vector3Normalize(&this->xm, this->xm);
		}

		inline void Add(float _s)
		{
			this->x += _s;
			this->y += _s;
			this->z += _s;
		}

		inline void Add(float _x, float _y, float _z)
		{
			this->x += _x;
			this->y += _y;
			this->z += _z;
		}

		inline void Add(const XMFLOAT3& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
		}

		inline void Add(const XMFLOAT3&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
		}

		inline void Add(const float3& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
		}

		inline void Add(const float3&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
		}

		inline void Mul(float _s){
			this->x *= _s;
			this->y *= _s;
			this->z *= _s;
		}

		inline void Mul(float _x, float _y, float _z)
		{
			this->x *= _x;
			this->y *= _y;
			this->z *= _z;
		}

		inline void Mul(const XMFLOAT3& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
		}

		inline void Mul(const XMFLOAT3&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
		}

		inline void Mul(const float3& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
		}

		inline void Mul(const float3&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
		}

		inline float Length()
		{
			return Vector3Length(this->xm);
		}

		inline float Dot(float _x, float _y, float _z)
		{
			return Vector3Dot(this->xm, XMFLOAT3(_x, _y, _z));
		}

		inline float Dot(const XMFLOAT3& _v)
		{
			return Vector3Dot(this->xm, _v);
		}

		inline float Dot(const XMFLOAT3&& _v)
		{
			return Vector3Dot(this->xm, _v);
		}

		inline float Dot(const float3& _v)
		{
			return Vector3Dot(this->xm, _v.xm);
		}

		inline float Dot(const float3&& _v)
		{
			return Vector3Dot(this->xm, _v.xm);
		}

		inline void Cross(float _x, float _y, float _z)
		{
			Vector3Cross(&this->xm, this->xm, XMFLOAT3(_x, _y, _z));
		}

		inline void Cross(const XMFLOAT3& _v)
		{
			Vector3Cross(&this->xm, this->xm, _v);
		}

		inline void Cross(const XMFLOAT3&& _v)
		{
			Vector3Cross(&this->xm, this->xm, _v);
		}

		inline void Cross(const float3& _v)
		{
			Vector3Cross(&this->xm, this->xm, _v.xm);
		}

		inline void Cross(const float3&& _v)
		{
			Vector3Cross(&this->xm, this->xm, _v.xm);
		}

		inline void TransformCoord(const XMFLOAT4X4& _mtx)
		{
			Vector3TransformCoord(&this->xm, this->xm, _mtx);
		}

		inline void TransformCoord(const XMFLOAT4X4&& _mtx)
		{
			Vector3TransformCoord(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(const XMFLOAT4X4& _mtx)
		{
			Vector3TransformNormal(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(const XMFLOAT4X4&& _mtx)
		{
			Vector3TransformNormal(&this->xm, this->xm, _mtx);
		}

		inline void TransformNormal(float _x, float _y, float _z, float _w)
		{
			Vector3TransformNormal(&this->xm, this->xm, XMFLOAT4(_x, _y, _z, _w));
		}

		inline void TransformNormal(const XMFLOAT4& _qtn)
		{
			Vector3TransformNormal(&this->xm, this->xm, _qtn);
		}

		inline void TransformNormal(const XMFLOAT4&& _qtn)
		{
			Vector3TransformNormal(&this->xm, this->xm, _qtn);
		}

		inline void operator=(const float3& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
		}

		inline void operator=(const float3&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
		}

		inline float operator[](const UINT n)
		{
			return this->f[n];
		}

	public:
		static const float3 one;
		static const float3 zero;
		static const float3 up;
		static const float3 down;
		static const float3 right;
		static const float3 left;
		static const float3 foward;
		static const float3 back;

	public:
		union
		{
			struct{ float x, y, z; };
			struct{ float r, g, b; };
			struct{ float f[3]; };
			struct{ XMFLOAT3 xm; };
		};
	};

	typedef float3 Vector3;
	typedef float3 aoFloat3;
	typedef float3 aoVector3;


	/*!-----------------------------------------------------------
	//	@class  float4
	//	@brief  4次元ベクトル
	//	@author	Tatsunori Aoyama
	//	@date	2014/09/03
	------------------------------------------------------------*/
	class float4
	{
	public:
		float4() : x(0), y(0), z(0), w(0){};
		float4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
		float4(float* _array) : x(_array[0]), y(_array[1]), z(_array[2]), w(_array[3]){}
		float4(const XMFLOAT4& _xm) : xm(_xm){}
		float4(const XMFLOAT4&& _xm) : xm(std::move(_xm)){}
		float4(const float4& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		float4(const float4&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		~float4(){};

		inline void Normalize()
		{
			Vector4Normalize(&this->xm, this->xm);
		}

		inline void Add(float _s)
		{
			this->x += _s;
			this->y += _s;
			this->z += _s;
			this->w += _s;
		}

		inline void Add(float _x, float _y, float _z, float _w)
		{
			this->x += _x;
			this->y += _y;
			this->z += _z;
			this->z += _w;
		}

		inline void Add(const XMFLOAT4& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
			this->w += _v.w;
		}

		inline void Add(const XMFLOAT4&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
			this->w += _v.w;
		}

		inline void Add(const float4& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
			this->w += _v.w;
		}

		inline void Add(const float4&& _v)
		{
			this->x += _v.x;
			this->y += _v.y;
			this->z += _v.z;
			this->w += _v.w;
		}

		inline void Mul(float _s)
		{
			this->x *= _s;
			this->y *= _s;
			this->z *= _s;
			this->w *= _s;
		}

		inline void Mul(float _x, float _y, float _z, float _w)
		{
			this->x *= _x;
			this->y *= _y;
			this->z *= _z;
			this->w *= _w;
		}

		inline void Mul(const XMFLOAT4& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
			this->w *= _v.w;
		}

		inline void Mul(const XMFLOAT4&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
			this->w *= _v.w;
		}

		inline void Mul(const float4& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
			this->w *= _v.w;
		}

		inline void Mul(const float4&& _v)
		{
			this->x *= _v.x;
			this->y *= _v.y;
			this->z *= _v.z;
			this->w *= _v.w;
		}

		inline float Length()
		{
			return Vector4Length(this->xm);
		}

		inline float Dot(float _x, float _y, float _z, float _w)
		{
			return Vector4Dot(this->xm, XMFLOAT4(_x, _y, _z, _w));
		}

		inline float Dot(const XMFLOAT4& _v)
		{
			return Vector4Dot(this->xm, _v);
		}

		inline float Dot(const XMFLOAT4&& _v)
		{
			return Vector4Dot(this->xm, _v);
		}

		inline float Dot(const float4& _v)
		{
			return Vector4Dot(this->xm, _v.xm);
		}

		inline float Dot(const float4&& _v)
		{
			return Vector4Dot(this->xm, _v.xm);
		}

		inline void Cross(float _x1, float _y1, float _z1, float _w1, float _x2, float _y2, float _z2, float _w2)
		{
			Vector4Cross(&this->xm, this->xm, XMFLOAT4(_x1, _y1, _z1, _w1), XMFLOAT4(_x2, _y2, _z2, _w2));
		}

		inline void Cross(const XMFLOAT4& _v1, const XMFLOAT4& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1, _v2);
		}

		inline void Cross(const XMFLOAT4&& _v1, const XMFLOAT4& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1, _v2);
		}

		inline void Cross(const XMFLOAT4& _v1, const XMFLOAT4&& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1, _v2);
		}

		inline void Cross(const XMFLOAT4&& _v1, const XMFLOAT4&& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1, _v2);
		}

		inline void Cross(const float4& _v1, const float4& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1.xm, _v2.xm);
		}

		inline void Cross(const float4&& _v1, const float4& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1.xm, _v2.xm);
		}

		inline void Cross(const float4& _v1, const float4&& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1.xm, _v2.xm);
		}

		inline void Cross(const float4&& _v1, const float4&& _v2)
		{
			Vector4Cross(&this->xm, this->xm, _v1.xm, _v2.xm);
		}

		inline void Transform(const XMFLOAT4X4& _mtx)
		{
			Vector4Transform(&this->xm, this->xm, _mtx);
		}

		inline void Transform(const XMFLOAT4X4&& _mtx)
		{
			Vector4Transform(&this->xm, this->xm, _mtx);
		}

		inline void Transform(float _x, float _y, float _z, float _w)
		{
			Vector4Transform(&this->xm, this->xm, XMFLOAT4(_x, _y, _z, _w));
		}

		inline void Transform(const XMFLOAT4& _qtn)
		{
			Vector4Transform(&this->xm, this->xm, _qtn);
		}

		inline void Transform(const XMFLOAT4&& _qtn)
		{
			Vector4Transform(&this->xm, this->xm, _qtn);
		}

		inline void operator=(const float4& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		inline void operator=(const float4&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}

		inline float operator[](const UINT n)
		{
			return this->f[n];
		}

	public:
		static const float4 one;
		static const float4 zero;
		static const float4 up_0;
		static const float4 down_0;
		static const float4 right_0;
		static const float4 left_0;
		static const float4 foward_0;
		static const float4 back_0;
		static const float4 zero_1;
		static const float4 up_1;
		static const float4 down_1;
		static const float4 right_1;
		static const float4 left_1;
		static const float4 foward_1;
		static const float4 back_1;

	public:
		union
		{
			struct{ float x, y, z, w; };
			struct{ float r, g, b, a; };
			struct{ float f[4]; };
			struct{ XMFLOAT4 xm; };
		};
	};

	typedef float4 Vector4;
	typedef float4 aoFloat4;
	typedef float4 aoVector4;


	/*!-----------------------------------------------------------
	//	@class  color
	//	@brief  色
	//	@author	Tatsunori Aoyama
	//	@date	2015/04/29
	------------------------------------------------------------*/
	class color
	{
	public:
		color() : x(1), y(1), z(1), w(1){};
		color(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
		color(float* _array) : x(_array[0]), y(_array[1]), z(_array[2]), w(_array[3]){}
		color(const XMFLOAT4& _xm) : xm(_xm){}
		color(const XMFLOAT4&& _xm) : xm(std::move(_xm)){}
		color(const color& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		color(const color&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		color(const float4& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		color(const float4&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		~color(){};

		inline bool IsInfinite()
		{
			return ColorIsInfinite(this->xm);
		}

		inline bool IsNaN()
		{
			return ColorIsNaN(this->xm);
		}

		void operator=(const color& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		void operator=(const color&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}

		void operator=(const float4& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}
		void operator=(const float4&& _inst)
		{
			this->x = _inst.x;
			this->y = _inst.y;
			this->z = _inst.z;
			this->w = _inst.w;
		}

		bool operator==(const color& _color)
		{
			return ColorEqual(this->xm, _color.xm);
		}

		bool operator==(const float4& _float4)
		{
			return ColorEqual(this->xm, _float4.xm);
		}

		bool operator!=(const color& _color)
		{
			return ColorNotEqual(this->xm, _color.xm);
		}

		bool operator!=(const float4& _float4)
		{
			return ColorNotEqual(this->xm, _float4.xm);
		}

		bool operator>(const color& _color)
		{
			return ColorGreater(this->xm, _color.xm);
		}

		bool operator>(const float4& _float4)
		{
			return ColorGreater(this->xm, _float4.xm);
		}

		bool operator>=(const color& _color)
		{
			return ColorGreaterOrEqual(this->xm, _color.xm);
		}

		bool operator>=(const float4& _float4)
		{
			return ColorGreaterOrEqual(this->xm, _float4.xm);
		}

		bool operator<(const color& _color)
		{
			return ColorLess(this->xm, _color.xm);
		}

		bool operator<(const float4& _float4)
		{
			return ColorLess(this->xm, _float4.xm);
		}

		bool operator<=(const color& _color)
		{
			return ColorLessOrEqual(this->xm, _color.xm);
		}

		bool operator<=(const float4& _float4)
		{
			return ColorLessOrEqual(this->xm, _float4.xm);
		}

	public:
		static const color white;
		static const color black;
		static const color red;
		static const color green;
		static const color blue;
		static const color yellow;
		static const color cyan;
		static const color purple;
		static const color transparent;

	public:
		union
		{
			struct{ float x, y, z, w; };
			struct{ float r, g, b, a; };
			struct{ float f[4]; };
			struct{ XMFLOAT4 xm; };
		};
	};

	typedef color Color;
	typedef color aoColor;

	/*!-----------------------------------------------------------
	//	@class  行列ラップクラス
	//	@brief  行列
	//	@author	Tatsunori Aoyama
	//	@date	2015/05/10
	------------------------------------------------------------*/
	class matrix
	{
	public:
		matrix() :
			line0(float4(1, 0, 0, 0)),
			line1(float4(0, 1, 0, 0)),
			line2(float4(0, 0, 1, 0)),
			line3(float4(0, 0, 0, 1)){}			

		matrix(const XMFLOAT4X4& _mtx) : xm(_mtx){}

		matrix(const matrix& _mtx) : xm(_mtx.xm){}

		matrix(
			const float4& _line0,
			const float4& _line1,
			const float4& _line2,
			const float4& _line3) : line0(_line0), line1(_line2), line3(_line3){}

		matrix(const float* const _f)
		{
			for (int i = 0; i < 16; i++)
			{
				this->f[i] = _f[i];
			}
		}

		void operator=(const matrix& _mtx)
		{
			::CopyMemory(&this->xm, &_mtx, sizeof(XMFLOAT4X4));
		}

		void operator=(const XMFLOAT4X4& _mtx)
		{
			::CopyMemory(&this->xm, &_mtx, sizeof(XMFLOAT4X4));
		}

	public:
		union
		{
			struct
			{
				XMFLOAT4X4 xm;
			};

			struct
			{
				float4 line0;
				float4 line1;
				float4 line2;
				float4 line3;
			};

			struct
			{
				float f[16];
			};
		};

	public:
		static const matrix identity;
	};

	typedef matrix Matrix;
	typedef matrix aoMatrix;
}

#endif // _Include_MathDefine_hpp_
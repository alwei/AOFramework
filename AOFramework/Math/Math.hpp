/*************************************************************
*	@file   	Math.hpp
*	@brief  	Mathクラスヘッダー
*	@author		Tatsunori Aoyama
*	@date		2013/05/20
*************************************************************/

#ifndef _Include_Math_hpp_	// インクルードガード
#define _Include_Math_hpp_

//------------------------------------------------------------
// インクルード
//------------------------------------------------------------
#include<math.h>
#include<DirectXMath.h>

using namespace DirectX;

namespace ao
{
		const float PI = 3.141592653589793f;
		const float RADIAN = PI / 180.0f;
		const float DEGREE = 180.0f / PI;

		static const XMFLOAT3 zero(0, 0, 0);
		static const XMFLOAT3 one(1, 1, 1);
		static const XMFLOAT3 right(1, 0, 0);
		static const XMFLOAT3 left(-1, 0, 0);
		static const XMFLOAT3 up(0, 1, 0);
		static const XMFLOAT3 down(0, -1, 0);
		static const XMFLOAT3 foward(0, 0, 1);
		static const XMFLOAT3 back(0, 0, -1);
		static const XMFLOAT4 QuaternionIdentity(0, 0, 0, 1);

		/*!-----------------------------------------------------------
		//	@brief		ラジアン変換
		//	@param[in]	_degree	デグリー値
		//	@return		変換されたラジアン値
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		inline	float ToRadian(float _degree){
			return _degree * RADIAN;
		}

		/*!-----------------------------------------------------------
		//	@brief		デグリー変換
		//	@param[in]	_radian	ラジアン値
		//	@return		変換されたデグリー値
		//	@author		Tatsunori Aoyama
		//	@date		2014/08/15
		------------------------------------------------------------*/
		inline	float ToDegree(float _radian)
		{
			return _radian * DEGREE;
		}

		inline bool Vector2Equal(const XMFLOAT2& _v1, const XMFLOAT2& _v2)
		{
			return XMVector2Equal(XMLoadFloat2(&_v1), XMLoadFloat2(&_v2));
		}

		inline XMFLOAT2* Vector2Normalize(XMFLOAT2* _pOut, XMFLOAT2& _v)
		{
			XMStoreFloat2(_pOut, XMVector2Normalize(XMLoadFloat2(&_v)));
			return _pOut;
		}

		inline XMFLOAT2* Vector2Lerp(XMFLOAT2* _pOut, const XMFLOAT2& _v0, const XMFLOAT2& _v1, float _t)
		{
			XMStoreFloat2(_pOut, XMVectorLerp(XMLoadFloat2(&_v0), XMLoadFloat2(&_v1), _t));
			return _pOut;
		}

		inline XMFLOAT2* Vector2Add(XMFLOAT2* _pOut, const XMFLOAT2& _v0, const XMFLOAT2& _v1)
		{
			_pOut->x = _v0.x + _v1.x;
			_pOut->y = _v0.y + _v1.y;
			return _pOut;
		}

		inline float Vector2Length(const XMFLOAT2& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector2Length(XMLoadFloat2(&_v)));
			return temp;
		}

		inline float Vector2LengthEst(const XMFLOAT2& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector2LengthEst(XMLoadFloat2(&_v)));
			return temp;
		}

		inline float Vector2LengthSq(const XMFLOAT2& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector2Length(XMLoadFloat2(&_v)));
			return temp;
		}

		inline float Vector2Dot(const XMFLOAT2& _v1, const XMFLOAT2& _v2)
		{
			float temp;
			XMStoreFloat(&temp, XMVector2Dot(XMLoadFloat2(&_v1), XMLoadFloat2(&_v2)));
			return temp;
		}

		inline XMFLOAT2* Vector2Cross(XMFLOAT2* _pOut, const XMFLOAT2& _v1, const XMFLOAT2& _v2)
		{
			XMStoreFloat2(_pOut, XMVector2Cross(XMLoadFloat2(&_v1), XMLoadFloat2(&_v2)));
			return _pOut;
		}

		inline XMFLOAT2* Vector2TransformCoord(XMFLOAT2* _pOut, const XMFLOAT2& _pIn, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat2(_pOut, XMVector2TransformCoord(XMLoadFloat2(&_pIn), XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT2* Vector2TransformNormal(XMFLOAT2* _pOut, const XMFLOAT2& _pIn, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat2(_pOut, XMVector2TransformNormal(XMLoadFloat2(&_pIn), XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT2* Vector2TransformNormal(XMFLOAT2* _pOut, const XMFLOAT2& _in, const XMFLOAT4& _qtn)
		{
			XMStoreFloat2(_pOut, XMVector2TransformNormal(XMLoadFloat2(&_in), XMMatrixRotationQuaternion(XMLoadFloat4(&_qtn))));
		}

		inline bool Vector3Equal(const XMFLOAT3& _v1, const XMFLOAT3& _v2)
		{
			return XMVector3Equal(XMLoadFloat3(&_v1), XMLoadFloat3(&_v2));
		}

		inline XMFLOAT3* Vector3Normalize(XMFLOAT3* _pOut, const XMFLOAT3& _v)
		{
			XMStoreFloat3(_pOut, XMVector3Normalize(XMLoadFloat3(&_v)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3Lerp(XMFLOAT3* _pOut, const XMFLOAT3& _v0, const XMFLOAT3& _v1, float _t)
		{
			XMStoreFloat3(_pOut, XMVectorLerp(XMLoadFloat3(&_v0), XMLoadFloat3(&_v1), _t));
			return _pOut;
		}

		inline XMFLOAT3* Vector3Add(XMFLOAT3* _pOut, const XMFLOAT3 _v0, const XMFLOAT3& _v1)
		{
			_pOut->x = _v0.x + _v1.x;
			_pOut->y = _v0.y + _v1.y;
			_pOut->z = _v0.z + _v1.z;
			return _pOut;
		}

		inline float Vector3Length(const XMFLOAT3& _v)
		{
			float temp;
			XMStoreFloat(&temp,XMVector3Length(XMLoadFloat3(&_v)));
			return temp;
		}

		inline float Vector3LengthEst(const XMFLOAT3& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector3LengthEst(XMLoadFloat3(&_v)));
			return temp;
		}

		inline float Vector3LengthSq(const XMFLOAT3& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector3Length(XMLoadFloat3(&_v)));
			return temp;
		}

		inline float Vector3Dot(const XMFLOAT3& _v1, const XMFLOAT3& _v2)
		{
			float temp;
			XMStoreFloat(&temp, XMVector3Dot(XMLoadFloat3(&_v1), XMLoadFloat3(&_v2)));
			return temp;
		}

		inline XMFLOAT3* Vector3Cross(XMFLOAT3* _pOut, const XMFLOAT3& _v1, const XMFLOAT3& _v2)
		{
			XMStoreFloat3(_pOut, XMVector3Cross(XMLoadFloat3(&_v1), XMLoadFloat3(&_v2)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3TransformCoord(XMFLOAT3* _pOut, const XMFLOAT3& _in, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat3(_pOut, XMVector3TransformCoord(XMLoadFloat3(&_in), XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3TransformCoord(XMFLOAT3* _pOut, const XMFLOAT3& _in, const XMFLOAT4& _qtn)
		{
			XMStoreFloat3(_pOut, XMVector3TransformCoord(XMLoadFloat3(&_in), XMMatrixRotationQuaternion(XMLoadFloat4(&_qtn))));
			return _pOut;
		}

		inline XMFLOAT3* Vector3TransformNormal(XMFLOAT3* _pOut, const XMFLOAT3& _in, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat3(_pOut, XMVector3TransformNormal(XMLoadFloat3(&_in), XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3TransformNormal(XMFLOAT3* _pOut, const XMFLOAT3& _in, const XMFLOAT4& _qtn)
		{
			XMStoreFloat3(_pOut, XMVector3TransformNormal(XMLoadFloat3(&_in), XMMatrixRotationQuaternion(XMLoadFloat4(&_qtn))));
			return _pOut;
		}

		inline XMFLOAT3* Vector3Rotate(XMFLOAT3* _pOut, const XMFLOAT3& _axis, const XMFLOAT4& _rot)
		{
			XMStoreFloat3(_pOut, XMVector3Rotate(XMLoadFloat3(&_axis), XMLoadFloat4(&_rot)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3InverseRotate(XMFLOAT3* _pOut, const XMFLOAT3& _axis, const XMFLOAT4& _rot)
		{
			XMStoreFloat3(_pOut, XMVector3InverseRotate(XMLoadFloat3(&_axis), XMLoadFloat4(&_rot)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3AngleBetweenNormals(XMFLOAT3* _pOut, const XMFLOAT3& _v1, const XMFLOAT3& _v2)
		{
			XMStoreFloat3(_pOut, XMVector3AngleBetweenNormals(XMLoadFloat3(&_v1), XMLoadFloat3(&_v2)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3AngleBetweenVectors(XMFLOAT3* _pOut, const XMFLOAT3& _v1, const XMFLOAT3& _v2)
		{
			XMStoreFloat3(_pOut, XMVector3AngleBetweenVectors(XMLoadFloat3(&_v1), XMLoadFloat3(&_v2)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3Project(
			XMFLOAT3* _pOut,
			const XMFLOAT3& _vPos,
			float _xView, float _yView,
			float _width, float _height,
			float _zMin, float _zMax,
			const XMFLOAT4X4& _proj, const XMFLOAT4X4& _view, const XMFLOAT4X4& _world)
		{
			XMStoreFloat3(
				_pOut, XMVector3Project(XMLoadFloat3(&_vPos),
				_xView, _yView,
				_width, _height,
				_zMin, _zMax,
				XMLoadFloat4x4(&_proj), XMLoadFloat4x4(&_view), XMLoadFloat4x4(&_world)));
			return _pOut;
		}

		inline XMFLOAT3* Vector3Unproject(
			XMFLOAT3* _pOut,
			const XMFLOAT3& _vPos,
			float _xView, float _yView,
			float _width, float _height,
			float _zMin, float _zMax,
			const XMFLOAT4X4& _proj, const XMFLOAT4X4& _view, const XMFLOAT4X4& _world)
		{
			XMStoreFloat3(
				_pOut, XMVector3Unproject(XMLoadFloat3(&_vPos),
				_xView, _yView,
				_width, _height,
				_zMin, _zMax,
				XMLoadFloat4x4(&_proj), XMLoadFloat4x4(&_view), XMLoadFloat4x4(&_world)));
			return _pOut;
		}

		inline bool Vector4Equal(const XMFLOAT4& _v1, const XMFLOAT4& _v2)
		{
			return XMVector4Equal(XMLoadFloat4(&_v1), XMLoadFloat4(&_v2));
		}

		inline XMFLOAT4* Vector4Normalize(XMFLOAT4* _pOut, XMFLOAT4& _v)
		{
			XMStoreFloat4(_pOut, XMVector4Normalize(XMLoadFloat4(&_v)));
			return _pOut;
		}

		inline XMFLOAT4* Vector4Lerp(XMFLOAT4* _pOut, const XMFLOAT4& _v0, const XMFLOAT4& _v1, float _t)
		{
			XMStoreFloat4(_pOut, XMVectorLerp(XMLoadFloat4(&_v0), XMLoadFloat4(&_v1), _t));
			return _pOut;
		}

		inline XMFLOAT4* Vector3Add(XMFLOAT4* _pOut, const XMFLOAT4 _v0, const XMFLOAT4& _v1)
		{
			_pOut->x = _v0.x + _v1.x;
			_pOut->y = _v0.y + _v1.y;
			_pOut->z = _v0.z + _v1.z;
			_pOut->w = _v0.w + _v1.w;
			return _pOut;
		}

		inline float Vector4Length(const XMFLOAT4& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector4Length(XMLoadFloat4(&_v)));
			return temp;
		}

		inline float Vector4LengthEst(const XMFLOAT4& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector4LengthEst(XMLoadFloat4(&_v)));
			return temp;
		}

		inline float Vector4LengthSq(const XMFLOAT4& _v)
		{
			float temp;
			XMStoreFloat(&temp, XMVector4Length(XMLoadFloat4(&_v)));
			return temp;
		}

		inline float Vector4Dot(const XMFLOAT4& _v1, const XMFLOAT4& _v2)
		{
			float temp;
			XMStoreFloat(&temp, XMVector4Dot(XMLoadFloat4(&_v1), XMLoadFloat4(&_v2)));
			return temp;
		}

		inline XMFLOAT4* Vector4Cross(XMFLOAT4* _pOut, const XMFLOAT4& _v1, const XMFLOAT4& _v2, const XMFLOAT4 _v3)
		{
			XMStoreFloat4(_pOut, XMVector4Cross(XMLoadFloat4(&_v1), XMLoadFloat4(&_v2), XMLoadFloat4(&_v3)));
			return _pOut;
		}

		inline XMFLOAT4* Vector4Transform(XMFLOAT4* _pOut, const XMFLOAT4& _in, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat4(_pOut, XMVector4Transform(XMLoadFloat4(&_in), XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT4* Vector4Transform(XMFLOAT4* _pOut, const XMFLOAT4& _in, const XMFLOAT4& _qtn)
		{
			XMStoreFloat4(_pOut, XMVector4Transform(XMLoadFloat4(&_in), XMMatrixRotationQuaternion(XMLoadFloat4(&_qtn))));
			return _pOut;
		}

		inline XMFLOAT4* ColorSRGBToRGB(XMFLOAT4* _pOut, const XMFLOAT4& _sRGB)
		{
			XMStoreFloat4(_pOut, XMColorSRGBToRGB(XMLoadFloat4(&_sRGB)));
			return _pOut;
		}

		inline XMFLOAT4* ColorRGBToSRGB(XMFLOAT4* _pOut, const XMFLOAT4& _RGB)
		{
			XMStoreFloat4(_pOut, XMColorSRGBToRGB(XMLoadFloat4(&_RGB)));
			return _pOut;
		}

		inline XMFLOAT4* ColorAdjustContrast(XMFLOAT4* _pOut, const XMFLOAT4& _color,const float _contrast)
		{
			XMStoreFloat4(_pOut, XMColorAdjustContrast(XMLoadFloat4(&_color), _contrast));
			return _pOut;
		}

		inline XMFLOAT4* ColorAdjustSaturation(XMFLOAT4* _pOut, const XMFLOAT4& _color, const float _saturation)
		{
			XMStoreFloat4(_pOut, XMColorAdjustSaturation(XMLoadFloat4(&_color), _saturation));
			return _pOut;
		}

		inline bool ColorEqual(const XMFLOAT4& _color1, const XMFLOAT4& _color2)
		{
			return XMColorEqual(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline bool ColorGreater(const XMFLOAT4 _color1, const XMFLOAT4& _color2)
		{
			return XMColorGreater(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline bool ColorGreaterOrEqual(const XMFLOAT4 _color1, const XMFLOAT4& _color2)
		{
			return XMColorGreaterOrEqual(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline XMFLOAT4* ColorHSLToRGB(XMFLOAT4* _pOut, const XMFLOAT4& _HSL)
		{
			XMStoreFloat4(_pOut, XMColorHSLToRGB(XMLoadFloat4(&_HSL)));
			return _pOut;
		}

		inline XMFLOAT4* ColorHSVToRGB(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorHSVToRGB(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline bool ColorIsInfinite(const XMFLOAT4& _color)
		{
			return XMColorIsInfinite(XMLoadFloat4(&_color));
		}

		inline bool ColorIsNaN(const XMFLOAT4& _color)
		{
			return XMColorIsNaN(XMLoadFloat4(&_color));
		}

		inline bool ColorLess(const XMFLOAT4 _color1, const XMFLOAT4& _color2)
		{
			return XMColorLess(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline bool ColorLessOrEqual(const XMFLOAT4 _color1, const XMFLOAT4& _color2)
		{
			return XMColorLessOrEqual(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline XMFLOAT4* ColorModulate(XMFLOAT4* _pOut, const XMFLOAT4& _color1, const XMFLOAT4& _color2)
		{
			XMStoreFloat4(_pOut, XMColorModulate(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2)));
			return _pOut;
		}

		inline XMFLOAT4* ColorNegative(XMFLOAT4* _pOut, const XMFLOAT4& _color)
		{
			XMStoreFloat4(_pOut, XMColorNegative(XMLoadFloat4(&_color)));
			return _pOut;
		}

		inline bool ColorNotEqual(const XMFLOAT4& _color1, const XMFLOAT4& _color2)
		{
			return XMColorNotEqual(XMLoadFloat4(&_color1), XMLoadFloat4(&_color2));
		}

		inline XMFLOAT4* ColorRGBToHSL(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorRGBToHSL(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorRGBToHSV(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorRGBToHSV(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorRGBToXYZ(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorRGBToXYZ(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorRGBToYUV(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorRGBToYUV(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorRGBToYUV_HD(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorRGBToYUV_HD(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorSRGBToXYZ(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorSRGBToXYZ(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorXYZToRGB(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorXYZToRGB(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorXYZToSRGB(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorXYZToSRGB(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorYUVToRGB(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorYUVToRGB(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4* ColorYUVToRGB_HD(XMFLOAT4* _pOut, const XMFLOAT4& _HSV)
		{
			XMStoreFloat4(_pOut, XMColorYUVToRGB_HD(XMLoadFloat4(&_HSV)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixIdentity(XMFLOAT4X4* _pOut)
		{
			XMStoreFloat4x4(_pOut, XMMatrixIdentity());
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixSetOffset(XMFLOAT4X4* _pOut, float _x, float _y, float _z)
		{
			_pOut->_41 = _x;
			_pOut->_42 = _y;
			_pOut->_43 = _z;
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixSetOffsetScale(XMFLOAT4X4* _pOut, float _x, float _y, float _z, float _sx, float _sy, float _sz)
		{
			_pOut->_41 = _x;
			_pOut->_42 = _y;
			_pOut->_43 = _z;
			_pOut->_11 = _sx;
			_pOut->_22 = _sy;
			_pOut->_33 = _sz;
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixScale(XMFLOAT4X4* _pOut, float _x, float _y, float _z)
		{
			XMStoreFloat4x4(_pOut, XMMatrixScaling(_x, _y, _z));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixScale(XMFLOAT4X4* _pOut, const XMFLOAT3& _scale)
		{
			XMStoreFloat4x4(_pOut, XMMatrixScalingFromVector(XMLoadFloat3(&_scale)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixTranslate(XMFLOAT4X4* _pOut, float _x, float _y, float _z)
		{
			XMStoreFloat4x4(_pOut, XMMatrixTranslation(_x, _y, _z));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixTranslate(XMFLOAT4X4* _pOut, const XMFLOAT3& _position)
		{
			XMStoreFloat4x4(_pOut, XMMatrixTranslation(_position.x, _position.y, _position.z));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixRotationX(XMFLOAT4X4* _pOut, float _angle)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationX(_angle));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixRotationY(XMFLOAT4X4* _pOut, float _angle)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationY(_angle));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixRotationZ(XMFLOAT4X4* _pOut, float _angle)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationZ(_angle));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixRollPitchYaw(XMFLOAT4X4* _pOut, float _xRot, float _yRot, float _zRot)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationRollPitchYaw(_xRot, _yRot, _zRot));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixRollPitchYawFromVector3(XMFLOAT4X4* _pOut, XMFLOAT3& _in)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&_in)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixMultiply(XMFLOAT4X4* _pOut, const XMFLOAT4X4& _local, const XMFLOAT4X4& _world)
		{
			XMStoreFloat4x4(_pOut, XMMatrixMultiply(XMLoadFloat4x4(&_local), XMLoadFloat4x4(&_world)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixInverse(XMFLOAT4X4* _pOut, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat4x4(_pOut, XMMatrixInverse(nullptr, XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixTranspose(XMFLOAT4X4* _pOut, const XMFLOAT4X4& _mtx)
		{
			XMStoreFloat4x4(_pOut, XMMatrixTranspose(XMLoadFloat4x4(&_mtx)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixLookAtLH(XMFLOAT4X4* _pOut, const XMFLOAT3& _eye, const XMFLOAT3& _at, const XMFLOAT3& _up)
		{
			XMStoreFloat4x4(_pOut, XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_at), XMLoadFloat3(&_up)));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixOrthographicLH(XMFLOAT4X4* _pOut, float _ViewWidth, float _ViewHeight, float _NearZ, float _FarZ)
		{
			XMStoreFloat4x4(_pOut, XMMatrixOrthographicLH(_ViewWidth, _ViewHeight, _NearZ, _FarZ));
			return _pOut;
		}

		inline XMFLOAT4X4* MatrixPerspectiveFovLH(XMFLOAT4X4* _pOut, float _FovAngleY, float _AspectHByW, float _NearZ, float _FarZ)
		{
			XMStoreFloat4x4(_pOut, XMMatrixPerspectiveFovLH(_FovAngleY, _AspectHByW, _NearZ, _FarZ));
			return _pOut;
		}

		inline XMFLOAT3* MatrixToEuler(XMFLOAT3* _pOut, const XMFLOAT4X4& _in)
		{
			if (_in._32 >= 1.0f)
			{
				_pOut->x = PI * 0.5f;
				_pOut->y = 0;
				_pOut->z = ::atanf(_in._21 / _in._11);
			}
			else if (_in._32 <= -1.0f)
			{
				_pOut->x = -PI * 0.5f;
				_pOut->y = 0;
				_pOut->z = ::atanf(_in._21 / _in._11);
			}
			else
			{
				_pOut->x = -::asinf(_in._32);
				_pOut->y = -::atanf(-_in._31 / _in._33);
				_pOut->z = -::atanf(-_in._12 / _in._22);
			}
			return _pOut;
		}

		inline XMFLOAT4* MatrixToQuaternion(XMFLOAT4* _pOut, const XMFLOAT4X4& _in)
		{
			XMStoreFloat4(_pOut, XMQuaternionRotationMatrix(XMLoadFloat4x4(&_in)));
			return _pOut;
		}

		inline XMFLOAT4X4* QuaternionToMatrix(XMFLOAT4X4* _pOut, const XMFLOAT4& _in)
		{
			XMStoreFloat4x4(_pOut, XMMatrixRotationQuaternion(XMLoadFloat4(&_in)));
			return _pOut;
		}

		inline XMFLOAT4* QuaternionRotationAxis(XMFLOAT4* _pOut, float _x, float _y, float _z, float _radian)
		{
			XMStoreFloat4(_pOut, XMQuaternionRotationAxis(XMLoadFloat3(&XMFLOAT3(_x, _y, _z)), _radian));
			return _pOut;
		}

		inline XMFLOAT4* QuaternionRotationAxis(XMFLOAT4* _pOut, const XMFLOAT3& _axis, float _radian)
		{
			XMStoreFloat4(_pOut, XMQuaternionRotationAxis(XMLoadFloat3(&_axis), _radian));
			return _pOut;
		}

		inline float QuaternionToAxisAngle(XMFLOAT3* _pOutAxis, float* _pOutAngle, const XMFLOAT4& _qtn)
		{
			XMVECTOR outAxis;
			XMQuaternionToAxisAngle(&outAxis, _pOutAngle, XMLoadFloat4(&_qtn));
			XMStoreFloat3(_pOutAxis, outAxis);
			return *_pOutAngle;
		}

		inline XMFLOAT4* QuaternionMultiply(XMFLOAT4* _pOut, const XMFLOAT4& _q1, const XMFLOAT4& _q2)
		{
			XMStoreFloat4(_pOut, XMQuaternionMultiply(XMLoadFloat4(&_q1), XMLoadFloat4(&_q2)));
			return _pOut;
		}

		inline XMFLOAT4* QuaternionInverse(XMFLOAT4* _pOut, const XMFLOAT4& _qtn)
		{
			XMStoreFloat4(_pOut, XMQuaternionInverse(XMLoadFloat4(&_qtn)));
			return _pOut;
		}

		inline XMFLOAT4* QuaternionSlerp(XMFLOAT4* _pOut, const XMFLOAT4& _q1, const XMFLOAT4& _q2, float _t)
		{
			XMStoreFloat4(_pOut, XMQuaternionSlerp(XMLoadFloat4(&_q1), XMLoadFloat4(&_q2), _t));
			return _pOut;
		}

		inline XMFLOAT4* LookAtVector3ToQuaternion(XMFLOAT4* _pOut, const XMFLOAT3& _to, const XMFLOAT3& _from)
		{
			XMFLOAT3 eps(0.000001f, 0.000001f, 0.000001f);
			XMFLOAT3 dir(_to.x - _from.x, _to.y - _from.y, _to.z - _from.z);
			Vector3Normalize(&dir, dir);
			if (XMVector3NearEqual(XMLoadFloat3(&dir), XMLoadFloat3(&foward), XMLoadFloat3(&eps)))
			{
				_pOut->x = _pOut->y = _pOut->z = 0;
				return nullptr;
			}

			float radian;
			XMFLOAT3 axis;
			Vector3Cross(&axis, foward, dir);
			radian = XMScalarACos(Vector3Dot(foward, dir));
			QuaternionRotationAxis(_pOut, axis, radian);

			return _pOut;
		}

		inline XMFLOAT4* LookAtVector3ToQuaternion(XMFLOAT4* _pOut, const XMFLOAT3& _dir)
		{
			XMFLOAT3 eps(0.000001f, 0.000001f, 0.000001f);
			XMFLOAT3 dir;
			Vector3Normalize(&dir, _dir);
			if (XMVector3NearEqual(XMLoadFloat3(&dir), XMLoadFloat3(&foward), XMLoadFloat3(&eps)))
			{
				_pOut->x = _pOut->y = _pOut->z = 0;
				return nullptr;
			}

			float radian;
			XMFLOAT3 axis;
			Vector3Cross(&axis, foward, dir);
			radian = XMScalarACos(Vector3Dot(foward, dir));
			QuaternionRotationAxis(_pOut, axis, radian);
			return _pOut;
		}

		/*!-----------------------------------------------------------
		//	@class  SinCosTable
		//	@brief  SinCosテーブルクラス
		//	@author	Tatsunori Aoyama
		//	@date	2013/05/20
		------------------------------------------------------------*/
		class SinCosTable
		{
		public:
			enum DEFAULT_PARAM
			{
				ACCURACY = 10,
				DIVISION = 360 * ACCURACY
			};
		public:
			~SinCosTable(){}

			/*!-----------------------------------------------------------
			//	@brief		Sinテーブルから値を参照
			//	@param[in]	_degreeDecimal 小数指定できるdgree
			//	@return		sin値：正常な角度値	0xFF：不正な角度値
			//	@author		Tatsunori Aoyama
			//	@date		2013/05/20
			------------------------------------------------------------*/
			const float GetSinTable(const float _degreeDecimal)
			{
				if (0 <= _degreeDecimal && _degreeDecimal <= 360)
				{
					int degree = (int)(_degreeDecimal * ACCURACY);
					return this->pSinTable[degree];
				}

				return 0xFF;
			}

			/*!-----------------------------------------------------------
			//	@brief		Cosテーブルから値を参照
			//	@param[in]	degreeDecimal 小数指定できるdgree
			//	@return		cos値：正常なdegree0xFF：不正なdegree値
			//	@author		Tatsunori Aoyama
			//	@date		2013/05/20
			------------------------------------------------------------*/
			const float GetCosTable(const float degreeDecimal)
			{
				if (0 <= degreeDecimal && degreeDecimal <= 360)
				{
					int degree = (int)(degreeDecimal * ACCURACY);
					return this->pCosTable[degree];
				}

				return 0xFF;
			}

			/*!-----------------------------------------------------------
			//	@brief		インスタンス取得メソッド
			//	@return		自身のインスタンス
			//	@author		Tatsunori Aoyama
			//	@date		2013/05/20
			------------------------------------------------------------*/
			static	SinCosTable*	GetInstance()
			{
				static SinCosTable singleton;
				return &singleton;
			}

		private:
			SinCosTable(){
				this->divNum = DEFAULT_PARAM::DIVISION;
				for (int i = 0; i < DEFAULT_PARAM::DIVISION; ++i)
				{
					this->pSinTable[i] = sin(i / (float)ACCURACY*RADIAN);
					this->pCosTable[i] = cos(i / (float)ACCURACY*RADIAN);
				}
			}
		private:
			int		divNum;
			float	pSinTable[DEFAULT_PARAM::DIVISION];
			float	pCosTable[DEFAULT_PARAM::DIVISION];
		};

	//シングルトン定義
	inline	ao::SinCosTable*	sSinCos()
	{
		return ao::SinCosTable::GetInstance();
	}
}





#endif // _Include_Math_hpp_
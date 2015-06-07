static const float4x4 identityMtx =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};


static const float3 TriVertex[3] =
{
	{ -1.f, +1.f, 0.f },
	{ +3.f, +1.f, 0.f },
	{ -1.f, -3.f, 0.f }
};

static const float PI = 3.14159265f;
static const float TO_RAD = PI / 180.0f;
static const float TO_DEG = 180.0f / PI;




inline float ToRadian(float _degree)
{
	return _degree * TO_RAD;
};


inline float ToDegree(float _radian)
{
	return _radian * TO_DEG;
};


inline float4x4 CreateXRotationMatrix(float _degree)
{
	float4x4 mtx = identityMtx;
	float t = ToRadian(_degree);
	float s = 0;
	float c = 0;
	sincos(t, s, c);
	mtx._22 = 0 + c;
	mtx._23 = 0 - s;
	mtx._32 = 0 + s;
	mtx._33 = 0 + c;

	return mtx;
};


inline float4x4 CreateYRotationMatrix(float _degree)
{
	float4x4 mtx = identityMtx;
	float t = ToRadian(_degree);
	float s = 0;
	float c = 0;
	sincos(t, s, c);
	mtx._11 = 0 + c;
	mtx._13 = 0 + s;
	mtx._31 = 0 - s;
	mtx._33 = 0 + c;

	return mtx;
};


inline float4x4 CreateZRotationMatrix(float _degree)
{
	float4x4 mtx = identityMtx;
	float t = ToRadian(_degree);
	float s = 0;
	float c = 0;
	sincos(t, s, c);
	mtx._11 = 0 + c;
	mtx._12 = 0 - s;
	mtx._21 = 0 + s;
	mtx._22 = 0 + c;

	return mtx;
};


inline float4x4 CreateScaleMatrix(float _x = 1, float _y = 1, float _z = 1)
{
	float4x4 mtx = identityMtx;

	mtx._11 = _x;
	mtx._22 = _y;
	mtx._33 = _z;

	return mtx;
};


inline float4x4 CreateOffsetMatrix(float _x = 0, float _y = 0, float _z = 0)
{
	float4x4 mtx = identityMtx;

	mtx._41 = _x;
	mtx._42 = _y;
	mtx._43 = _z;

	return mtx;
};


inline float4x4 CreateScaleOffsetMatrix(
	float _ox = 0, float _oy = 0, float _oz = 0,
	float _sx = 1, float _sy = 1, float _sz = 1)
{
	float4x4 mtx = identityMtx;

	mtx._11 = _sx;
	mtx._22 = _sy;
	mtx._33 = _sz;
	mtx._41 = _ox;
	mtx._42 = _oy;
	mtx._43 = _oz;

	return mtx;
}
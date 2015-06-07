#include"stdafx.hpp"
#include"MathDefine.hpp"

using namespace ao;

const float2 float2::one(1, 1);
const float2 float2::zero(0, 0);
const float2 float2::up(0, 1);
const float2 float2::down(0, -1);
const float2 float2::right(1, 0);
const float2 float2::left(-1, 0);


const float3 float3::one(1, 1, 1);
const float3 float3::zero(0, 0, 0);
const float3 float3::up(0, 1, 0);
const float3 float3::down(0, -1, 0);
const float3 float3::right(1, 0, 0);
const float3 float3::left(-1, 0, 0);
const float3 float3::foward(0, 0, 1);
const float3 float3::back(0, 0, -1);


const float4 float4::one(1, 1, 1, 1);
const float4 float4::zero(0, 0, 0, 0);
const float4 float4::up_0(0, 1, 0, 0);
const float4 float4::down_0(0, -1, 0, 0);
const float4 float4::right_0(1, 0, 0, 0);
const float4 float4::left_0(-1, 0, 0, 0);
const float4 float4::foward_0(0, 0, 1, 0);
const float4 float4::back_0(0, 0, -1, 0);
const float4 float4::zero_1(0, 0, 0, 1);
const float4 float4::up_1(0, 1, 0, 1);
const float4 float4::down_1(0, -1, 0, 1);
const float4 float4::right_1(1, 0, 0, 1);
const float4 float4::left_1(-1, 0, 0, 1);
const float4 float4::foward_1(0, 0, 1, 1);
const float4 float4::back_1(0, 0, -1, 1);


const color color::white(1, 1, 1, 1);
const color color::black(0, 0, 0, 1);
const color color::red(1, 0, 0, 1);
const color color::green(0, 1, 0, 1);
const color color::blue(0, 0, 1, 1);
const color color::yellow(1, 1, 0, 1);
const color color::cyan(0, 1, 1, 1);
const color color::purple(1, 0, 1, 1);
const color color::transparent(0, 0, 0, 0);


const Matrix identity(float4(1, 0, 0, 0), float4(0, 1, 0, 0), float4(0, 0, 1, 0), float4(0, 0, 0, 1));
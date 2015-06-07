#include"../Subroutine/GeneralMathSubroutine.hlsl"


struct GSInstance
{
	float2 pos;
	float2 size;
	float4 color;
};


cbuffer GsConst0 : register(b0)
{
	float4x4 matOrtho;
};


cbuffer GsConst1 : register(b1)
{
	GSInstance gsInst[4096/16];
};

struct GSInput
{
	float4 pos		: SV_POSITION;
	float degree	: TEXCOORD0;
	uint id			: TEXCOORD1;
};


struct GSOutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
	float4 col	: TEXCOORD1;
};


[maxvertexcount(3)]
void main(
	point GSInput input[1], 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput Out = (GSOutput)0;
	Out.col = gsInst[input[0].id].color;
	
	float4x4 osMtx = CreateScaleOffsetMatrix(
	gsInst[input[0].id].pos.x, gsInst[input[0].id].pos.y, 0,
	gsInst[input[0].id].size.x, gsInst[input[0].id].size.y, 0);
	float4x4 rotMtx = CreateZRotationMatrix(input[0].degree);
	
	float4x4 mtx = mul(mul(rotMtx, osMtx), matOrtho);
	
	for (uint i = 0; i < 3; i++)
	{
		Out.pos = mul(float4(TriVertex[i],1), mtx);
		output.Append(Out);
	}
}
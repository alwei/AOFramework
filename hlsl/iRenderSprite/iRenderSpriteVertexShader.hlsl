struct VSInput
{
	float3		pos			: POSITION;
	float3		normal		: NORMAL;
	float2		uv			: TEXEL;
	float4x4	mtx			: MATRIX;
	float4		color		: COLOR;
	uint		instanceID	: SV_InstanceID;
};

struct VSOutput
{
	float4	pos			: SV_POSITION;
	float3	normal		: NORMAL;
	float2	uv			: TEXCOORD0;
	float4	color		: TEXCOORD1;
};

VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = mul(float4(In.pos, 1), In.mtx);
	Out.normal = In.normal;
	Out.uv = In.uv;
	Out.color = In.color;

	return Out;
}
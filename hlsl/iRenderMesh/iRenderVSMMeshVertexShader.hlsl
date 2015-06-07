
struct VSInput
{
	float3		pos			: POSITION;
	float3		normal		: NORMAL;
	float2		uv			: TEXEL;
	uint		instanceID	: SV_InstanceID;
};


struct VSOutput
{
	float4	pos			: SV_POSITION;
	float3	normal		: NORMAL;
	float2	uv			: TEXCOORD0;
	float4	zMap		: TEXCOORD1;
	float4	color		: TEXCOORD2;
	float4	light		: TEXCOORD3;
	float4	eyeDir		: TEXCOORD4;
};


struct GpuBuffer0
{
	float4 localLightDir;
	float4 localEyePos;
	float4 sufColor;
};

struct GpuBuffer1
{
	float4x4 matWVP;
};

struct GpuBuffer2
{
	float4x4 matLWVP;
};



StructuredBuffer<GpuBuffer0> gpuBuffer0 : register(t0);
StructuredBuffer<GpuBuffer1> gpuBuffer1 : register(t1);
StructuredBuffer<GpuBuffer2> gpuBuffer2 : register(t2);


VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = mul(float4(In.pos, 1), gpuBuffer1[In.instanceID].matWVP);
	Out.normal = In.normal;
	Out.uv = In.uv;
	Out.zMap = mul(float4(In.pos, 1), gpuBuffer2[In.instanceID].matLWVP);
	Out.color = gpuBuffer0[In.instanceID].sufColor;
	Out.light = gpuBuffer0[In.instanceID].localLightDir;
	Out.eyeDir.xyz = gpuBuffer0[In.instanceID].localEyePos.xyz - In.pos;
	Out.eyeDir.w = gpuBuffer0[In.instanceID].localEyePos.w;

	return Out;
}
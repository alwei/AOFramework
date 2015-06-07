struct VSInput
{
	float3		pos			: POSITION;
	uint		instanceID	: SV_InstanceID;
};


struct GpuBuffer2
{
	float4x4 matLWVP;
};


StructuredBuffer<GpuBuffer2> gpuBuffer2 : register(t2);


struct VSOutput
{
	float4 pos		: SV_POSITION;
	float4 tex		: TEXCOORD0;
};

VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;
	Out.pos = mul(float4(In.pos, 1), gpuBuffer2[In.instanceID].matLWVP);
	Out.tex = Out.pos;

	return Out;
}
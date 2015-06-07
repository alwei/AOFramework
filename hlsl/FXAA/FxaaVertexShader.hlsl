struct VSInput
{
	float3 pos	: POSITION;
	float2 uv	: UV;
};


struct VSOutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD0;
};

VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = float4(In.pos + float3(ADJUST_X, ADJUST_Y,0), 1);
	Out.uv = In.uv;

	return Out;
}


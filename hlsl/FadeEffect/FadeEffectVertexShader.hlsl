struct VSInput{
	float3 pos	: POSITION;
};

struct VSOutput{
	float4 pos	: SV_POSITION;
};

VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = float4(In.pos, 1);

	return Out;
}
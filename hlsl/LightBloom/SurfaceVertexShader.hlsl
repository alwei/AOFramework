

struct VSInput
{
	float3 pos	: POSITION;
	float2 uv	: TEXCOORD;
};

struct VSOutput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
};


VSOutput main( VSInput In )
{
	VSOutput Out=(VSOutput)0;

	Out.pos = float4(In.pos, 1);
	Out.uv = In.uv;

	return Out;
}
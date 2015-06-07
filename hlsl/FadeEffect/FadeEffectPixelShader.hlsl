cbuffer MapColor : register(b0)
{
	float4 fadeColor;
};


struct PSInput{
	float4 pos	: SV_POSITION;
};


float4 main(PSInput In) : SV_TARGET
{
	return fadeColor;
}
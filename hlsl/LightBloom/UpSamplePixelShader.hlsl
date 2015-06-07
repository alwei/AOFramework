cbuffer PsConst : register(b0)
{
	int numBuffer;
	float mulPower;
	float2 uvOffset;
};

struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
};


Texture2D		reductionBuffer[MAX_BUFFER];
SamplerState	samplerState[MAX_BUFFER];


float4 main(PSInput In) : SV_TARGET
{
	float4 outColor = (float4)0;

	for (int i = 0; i < numBuffer; ++i){
		outColor += reductionBuffer[i].Sample(samplerState[i], In.uv + float2(0 + uvOffset.x, 0 + uvOffset.y));
		outColor += reductionBuffer[i].Sample(samplerState[i], In.uv + float2(0 + uvOffset.x, 0 - uvOffset.y));
		outColor += reductionBuffer[i].Sample(samplerState[i], In.uv + float2(0 - uvOffset.x, 0 + uvOffset.y));
		outColor += reductionBuffer[i].Sample(samplerState[i], In.uv + float2(0 - uvOffset.x, 0 - uvOffset.y));
	}

	return outColor*mulPower;
}
cbuffer MatrixBuffer : register(b0)
{
	float4x4 matWVP;
}


struct InputVS{
	float3 pos	: POSITION;
};


struct OutputVS{
	float4 pos		: SV_POSITION;
	float4 tex		: TEXCOORD0;
};


OutputVS main(InputVS In)
{
	OutputVS Out = (OutputVS)0;
	Out.pos = mul(float4(In.pos, 1), matWVP);
	Out.tex = Out.pos;

	return Out;
}
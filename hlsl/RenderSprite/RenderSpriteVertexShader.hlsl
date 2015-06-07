
cbuffer MatrixBuffer : register(b0)
{
	float4x4 matWVP;
}


struct InputVS{
	float3 pos	: POSITION;
	float3 n	: NORMAL;
	float2 uv	: UV;
};


struct OutputVS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD;
};


OutputVS main(InputVS In)
{
	OutputVS Out = (OutputVS)0;

	Out.pos = mul(float4(In.pos, 1), matWVP);
	Out.n = In.n;
	Out.uv = In.uv;

	return Out;
}

cbuffer MatrixBuffer : register(b0)
{
	float4x4 matWVP;
}

cbuffer AnimationBuffer : register(b1)
{
	float4x4 animBuffer[NUM_MESH];
}


struct InputVS{
	float3 pos	: POSITION;
	float3 n	: NORMAL;
	float2 uv	: UV;
	int	idx		: MESH_INDEX;
};


struct OutputVS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD;
};


OutputVS main( InputVS In)
{
	OutputVS Out = (OutputVS)0;
	Out.pos = float4(In.pos, 1);

	if( In.idx != -1 )
		Out.pos = mul(Out.pos, animBuffer[In.idx]);

	Out.pos = mul(Out.pos, matWVP);
	Out.n = In.n;
	Out.uv = In.uv;

	return Out;
}
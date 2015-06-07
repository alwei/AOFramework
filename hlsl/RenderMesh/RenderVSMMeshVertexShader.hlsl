
cbuffer MatrixBuffer : register(b0)
{
	float4x4 matWVP;
	float4 eyePos;
	float4x4 matLWVP;
}


struct InputVS{
	float3 pos	: POSITION;
	float3 n	: NORMAL;
	float2 uv	: UV;
};


struct OutputVS{
	float4 pos	: SV_POSITION;
	float3 n	: NORMAL;
	float2 uv	: TEXCOORD0;
	float4 eye	: TEXCOORD1;
	float4 zMap	: TEXCOORD2;
};


OutputVS main(InputVS In)
{
	OutputVS Out = (OutputVS)0;

	Out.pos = mul(float4(In.pos, 1), matWVP);
	Out.zMap = mul(float4(In.pos, 1), matLWVP);
	Out.n = In.n;
	Out.uv = In.uv;
	Out.eye.xyz = normalize(eyePos.xyz - In.pos.xyz);
	Out.eye.w = eyePos.w;

	return Out;
}
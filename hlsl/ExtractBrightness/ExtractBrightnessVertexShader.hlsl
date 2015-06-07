
struct VSInput
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};


struct VSOutput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};


VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = float4(In.pos, 1);
	Out.tex = In.tex;

	return Out;
}
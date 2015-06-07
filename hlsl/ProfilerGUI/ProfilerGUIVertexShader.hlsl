

struct OutputVS
{
	float4 pos : SV_Position;
	uint id : TEXCOORD;
};


OutputVS main(uint id : POSITION)
{
	OutputVS Out = (OutputVS)0;

	Out.pos = float4(id, 0, 0, 1);
	Out.id = id;

	return Out;
}
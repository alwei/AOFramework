

cbuffer GpuBuffer0 : register(b1)
{
	float2	renderSize;
	float	loadNormalize;
	float	renderAlpha;
};


cbuffer GpuBuffer1 : register(b2)
{
	float4 procLoad[RENDER_WIDTH];
};


struct GSInput
{
	float4 pos : SV_Position;
	uint id : TEXCOORD;
};


struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 col : TEXCOORD0;
};


[maxvertexcount(2)]
void main(point GSInput input[1], inout LineStream< GSOutput > output)
{
	GSOutput Out = (GSOutput)0;

	uint index = input[0].id;

	//�����_�����O���̃��X�^���C�Y���W
	float x = 1 - 2 * (index / renderSize.x);

	//���K�����ꂽ���א��l
	float n = -1 + min(2,procLoad[index / 4][index % 4] / loadNormalize);

	//�O���t�̉������_
	Out.pos = float4(x, -1, 0, 1);
	Out.col = float4(0, 1, 0, renderAlpha);
	output.Append(Out);

	//�O���t�̏㕔���_
	Out.pos = float4(x, n, 0, 1);
	Out.col = float4(1 + n, -n, 0, renderAlpha);
	output.Append(Out);
}
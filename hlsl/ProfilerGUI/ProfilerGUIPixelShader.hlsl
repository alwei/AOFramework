
struct PSInput
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
};



float4 main(PSInput In) : SV_TARGET
{
	//�O���t�ŕ��א��l��F�ŕ\��
	return In.col;
}

struct PSInput
{
	float4 pos	: SV_POSITION;
	float4 col	: TEXCOORD0;
};



float4 main(PSInput In) : SV_TARGET
{
	//グラフで負荷数値を色で表現
	return In.col;
}
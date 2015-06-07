struct VSInput
{
	float3 pos			: POSITION;
	float4 color		: COLOR;
	float3 speed		: SPEED;
	float nowLifeTime	: LIFE_TIME;
	float size			: SIZE;
};


struct VSOutput
{
	float4 pos			: SV_POSITION;
	float4 color		: TEXCOORD0;
	float nowLifeTime	: TEXCOORD1;
	float size			: TEXCOORD2;
};


VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = float4(In.pos, 1);
	Out.color = In.color;
	Out.nowLifeTime = In.nowLifeTime;
	Out.size = In.size;

	return Out;
}
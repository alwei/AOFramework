cbuffer pixBuffer : register(b0)
{
	float	g_weight0	: packoffset(c0.x);
	float	g_weight1	: packoffset(c0.y);
	float	g_weight2	: packoffset(c0.z);
	float	g_weight3	: packoffset(c0.w);
	float	g_weight4	: packoffset(c1.x);
	float	g_weight5	: packoffset(c1.y);
	float	g_weight6	: packoffset(c1.z);
	float	g_weight7	: packoffset(c1.w);
	float2	g_offset	: packoffset(c2.x);
	float2	g_power		: packoffset(c2.z);
}

Texture2D		g_tex		: register(t0);
SamplerState	g_samp		: register(s0);

struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv0	: TEXCOORD0;
	float2 uv1	: TEXCOORD1;
	float2 uv2	: TEXCOORD2;
	float2 uv3	: TEXCOORD3;
	float2 uv4	: TEXCOORD4;
	float2 uv5	: TEXCOORD5;
	float2 uv6	: TEXCOORD6;
	float2 uv7	: TEXCOORD7;
};


float4 main(PSInput In) : SV_TARGET
{
	float4 Out = 0;

	Out += (g_tex.Sample(g_samp, In.uv0) + g_tex.Sample(g_samp, float2(In.uv7.x, g_offset.y + In.uv7.y))) * g_weight0;
	Out += (g_tex.Sample(g_samp, In.uv1) + g_tex.Sample(g_samp, float2(In.uv6.x, g_offset.y + In.uv6.y))) * g_weight1;
	Out += (g_tex.Sample(g_samp, In.uv2) + g_tex.Sample(g_samp, float2(In.uv5.x, g_offset.y + In.uv5.y))) * g_weight2;
	Out += (g_tex.Sample(g_samp, In.uv3) + g_tex.Sample(g_samp, float2(In.uv4.x, g_offset.y + In.uv4.y))) * g_weight3;
	Out += (g_tex.Sample(g_samp, In.uv4) + g_tex.Sample(g_samp, float2(In.uv3.x, g_offset.y + In.uv3.y))) * g_weight4;
	Out += (g_tex.Sample(g_samp, In.uv5) + g_tex.Sample(g_samp, float2(In.uv2.x, g_offset.y + In.uv2.y))) * g_weight5;
	Out += (g_tex.Sample(g_samp, In.uv6) + g_tex.Sample(g_samp, float2(In.uv1.x, g_offset.y + In.uv1.y))) * g_weight6;
	Out += (g_tex.Sample(g_samp, In.uv7) + g_tex.Sample(g_samp, float2(In.uv0.x, g_offset.y + In.uv0.y))) * g_weight7;

	Out *= g_power.x;

	return Out;
}
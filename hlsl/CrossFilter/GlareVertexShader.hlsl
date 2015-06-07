
//定数バッファ												  
cbuffer vsBuffer : register(b0)
{
	float2	g_uvPos0	: packoffset(c0.x);
	float2	g_uvPos1	: packoffset(c0.z);
	float2	g_uvPos2	: packoffset(c1.x);
	float2	g_uvPos3	: packoffset(c1.z);
	float2	g_uvPos4	: packoffset(c2.x);
	float2	g_uvPos5	: packoffset(c2.z);
	float2	g_uvPos6	: packoffset(c3.x);
	float2	g_uvPos7	: packoffset(c3.z);
};


struct VSInput
{
	float3 pos	: POSITION;
	float2 uv	: TEXCOORD;
};


struct VSOutput
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


VSOutput main(VSInput In)
{
	VSOutput Out = (VSOutput)0;

	Out.pos = float4(In.pos, 1);
	Out.uv0 = In.uv + g_uvPos0;
	Out.uv1 = In.uv + g_uvPos1;
	Out.uv2 = In.uv + g_uvPos2;
	Out.uv3 = In.uv + g_uvPos3;
	Out.uv4 = In.uv + g_uvPos4;
	Out.uv5 = In.uv + g_uvPos5;
	Out.uv6 = In.uv + g_uvPos6;
	Out.uv7 = In.uv + g_uvPos7;

	return Out;
}
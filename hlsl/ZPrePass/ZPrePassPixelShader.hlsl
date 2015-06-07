struct InputPS{
	float4 pos	: SV_POSITION;
	float4 tex	: TEXCOORD0;
};


float2 main(InputPS In) : SV_TARGET
{
	float z = In.tex.z / In.tex.w;
	return float2(z, z*z);
}
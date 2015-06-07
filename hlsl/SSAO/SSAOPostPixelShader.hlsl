#define SPHERE_COUNT 16
static float3 SphereArray16[SPHERE_COUNT] =
{
	float3( 0.53812504, 0.18565957, -0.43192 ),
	float3( 0.13790712, 0.24864247, 0.44301823 ),
	float3( 0.33715037, 0.56794053, -0.005789503 ),
	float3( -0.6999805, -0.04511441, -0.0019965635 ),
	float3( 0.06896307, -0.15983082, -0.85477847 ),
	float3( 0.056099437, 0.006954967, -0.1843352 ),
	float3( -0.014653638, 0.14027752, 0.0762037 ),
	float3( 0.010019933, -0.1924225, -0.034443386),
	float3( -0.35775623, -0.5301969, -0.43581226),
	float3( -0.3169221, 0.106360726, 0.015860917),
	float3(0.010350345, -0.58698344, 0.0046293875),
	float3(-0.08972908, -0.49408212, 0.3287904),
	float3(0.7119986, -0.0154690035, -0.09183723),
	float3(-0.053382345, 0.059675813, -0.5411899),
	float3(0.035267662, -0.063188605, 0.54602677),
	float3(-0.47761092, 0.2847911, -0.0271716)
};

cbuffer ConstBuffer : register(b0)
{
	float4x4	g_matInvProj;
	float4x4	g_matProj;
	float		g_hemRadius;
	float		g_zFar;
	float		g_aoPower;
};



Texture2D		g_depthNormalMap : register(t0);
SamplerState	g_samplerState : register(s0);


struct PSInput
{
	float4 pos	: SV_POSITION;
	float2 uv	: TEXCOORD;
};


float3 CreateNormal( float2 _xy )
{
	float3 normal = float3(0,0,0);

	normal.xy = _xy;
	normal.z = -sqrt( max(1-normal.x*normal.x-normal.y*normal.y,0) );
	normal = normalize( normal );

	return normal;
};


float4 main(PSInput In) : SV_TARGET
{
	float4 Out;

	float4 depthNormalMap = g_depthNormalMap.Sample(g_samplerState,In.uv);

	// create normal vector
	float3 N = CreateNormal( depthNormalMap.xy );

	float4 clipPos=float4(0,0,0,0);
	clipPos.xy = (In.uv * float2(2.0f, -2.0f) + float2( -1.0f,1.0f ) ) * depthNormalMap.w;
	clipPos.zw = depthNormalMap.zw;

	float4 viewPos = mul( clipPos, g_matInvProj );

	float normalAO = 0;
	float depthAO = 0;

	for( uint i=0; i<SPHERE_COUNT; ++i )
	{
		// ray dir
		float3 ray = SphereArray16[i].xyz * g_hemRadius;

		// converged within the range of normal ray 
		ray = sign( dot( N, ray ) ) * ray;

		float4 envPos;
		// move to ray direction
		envPos.xyz = viewPos.xyz + ray;
		// conversion clip space 
		envPos = mul( float4( envPos.xyz, 1),g_matProj );
		// conversion screen space
		envPos.xy = envPos.xy / envPos.w * float2( 0.5f, -0.5f ) + 0.5f;

		// sampling
		float4 envNormalMap = g_depthNormalMap.Sample( g_samplerState,envPos.xy );
		float3 envN = CreateNormal( envNormalMap.xy );

		// check dot
		float n = dot( N,envN ) * 0.5f + 0.5f;
		n += step( depthNormalMap.z, envNormalMap.z );
		normalAO += min( n , 1 );

		depthAO += abs( depthNormalMap.z - envNormalMap.z ) / g_zFar;
	}

	Out = normalAO / (float)SPHERE_COUNT + depthAO;

	Out = pow( abs(Out) , g_aoPower );

	return Out;
}
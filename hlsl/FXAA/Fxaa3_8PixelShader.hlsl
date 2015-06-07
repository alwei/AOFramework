/*============================================================================

                              FXAA3 QUALITY - PC

============================================================================*/
#if (FXAA_PC == 1)
/*--------------------------------------------------------------------------*/
float4 FxaaPixelShader(
    // {xy} = center of pixel
    float2 pos,
    // {xyzw} = not used on FXAA3 Quality
    float4 posPos,       
    // {rgb_} = color in linear or perceptual color space
    // {___a} = luma in perceptual color space (not linear) 
    FxaaTex tex,
    // This must be from a constant/uniform.
    // {x_} = 1.0/screenWidthInPixels
    // {_y} = 1.0/screenHeightInPixels
    float2 rcpFrame,
    // {xyzw} = not used on FXAA3 Quality
    float4 rcpFrameOpt 
) {   
/*--------------------------------------------------------------------------*/
    #if (FXAA_GATHER4_ALPHA == 1)
        float4 luma4A = FxaaTexOffAlpha4(tex, pos.xy, FxaaInt2(-1, -1), rcpFrame.xy);
        #if (FXAA_DISCARD == 0)
            float4 rgbyM = FxaaTexTop(tex, pos.xy);
        #endif
        float4 luma4B = FxaaTexAlpha4(tex, pos.xy, rcpFrame.xy);
        float lumaNE = FxaaTexOff(tex, pos.xy, FxaaInt2(1, -1), rcpFrame.xy).w;
        float lumaSW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 1), rcpFrame.xy).w;
        float lumaNW = luma4A.w;
        float lumaN  = luma4A.z;
        float lumaW  = luma4A.x;
        float lumaM  = luma4A.y;
        float lumaE  = luma4B.z;
        float lumaS  = luma4B.x;
        float lumaSE = luma4B.y;
    #else
        float lumaN = FxaaTexOff(tex, pos.xy, FxaaInt2(0, -1), rcpFrame.xy).w;
        float lumaW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 0), rcpFrame.xy).w;
        float4 rgbyM = FxaaTexTop(tex, pos.xy);
        float lumaE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 0), rcpFrame.xy).w;
        float lumaS = FxaaTexOff(tex, pos.xy, FxaaInt2( 0, 1), rcpFrame.xy).w;
        float lumaM = rgbyM.w;
    #endif
/*--------------------------------------------------------------------------*/
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
/*--------------------------------------------------------------------------*/
    if(range < max(FXAA_QUALITY__EDGE_THRESHOLD_MIN, rangeMax * FXAA_QUALITY__EDGE_THRESHOLD))
        #if (FXAA_DISCARD == 1)
            FxaaDiscard;
        #else
            return rgbyM;
        #endif
/*--------------------------------------------------------------------------*/
    #if (FXAA_GATHER4_ALPHA == 0) 
        float lumaNW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1,-1), rcpFrame.xy).w;
        float lumaNE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1,-1), rcpFrame.xy).w;
        float lumaSW = FxaaTexOff(tex, pos.xy, FxaaInt2(-1, 1), rcpFrame.xy).w;
        float lumaSE = FxaaTexOff(tex, pos.xy, FxaaInt2( 1, 1), rcpFrame.xy).w;
    #endif
/*--------------------------------------------------------------------------*/
    #define FXAA_QUALITY__SUBPIX_TRIM_SCALE  (1.0/(1.0 - FXAA_QUALITY__SUBPIX_TRIM))
/*--------------------------------------------------------------------------*/
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float rangeL = abs(lumaL - lumaM);
    float blendL = FxaaSat((rangeL / range) - FXAA_QUALITY__SUBPIX_TRIM) * FXAA_QUALITY__SUBPIX_TRIM_SCALE; 
    blendL = min(FXAA_QUALITY__SUBPIX_CAP, blendL);
/*--------------------------------------------------------------------------*/
    float edgeVert = 
              abs(lumaNW + (-2.0 * lumaN) + lumaNE) +
        2.0 * abs(lumaW  + (-2.0 * lumaM) + lumaE ) +
              abs(lumaSW + (-2.0 * lumaS) + lumaSE);
    float edgeHorz = 
              abs(lumaNW + (-2.0 * lumaW) + lumaSW) +
        2.0 * abs(lumaN  + (-2.0 * lumaM) + lumaS ) +
              abs(lumaNE + (-2.0 * lumaE) + lumaSE);
    bool horzSpan = edgeHorz >= edgeVert;
/*--------------------------------------------------------------------------*/
    float lengthSign = horzSpan ? -rcpFrame.y : -rcpFrame.x;
    if(!horzSpan) lumaN = lumaW;
    if(!horzSpan) lumaS = lumaE;
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;
/*--------------------------------------------------------------------------*/
    bool pairN = gradientN >= gradientS;
    if(!pairN) lumaN = lumaS;
    if(!pairN) gradientN = gradientS;
    if(!pairN) lengthSign *= -1.0;
    float2 posN;
    posN.x = pos.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = pos.y + (horzSpan ? lengthSign * 0.5 : 0.0);
/*--------------------------------------------------------------------------*/
    #define FXAA_SEARCH_STEPS     6
    #define FXAA_SEARCH_THRESHOLD (1.0/4.0)
/*--------------------------------------------------------------------------*/
    gradientN *= FXAA_SEARCH_THRESHOLD;
/*--------------------------------------------------------------------------*/
    float2 posP = posN;
    float2 offNP = horzSpan ? 
        FxaaFloat2(rcpFrame.x, 0.0) :
        FxaaFloat2(0.0f, rcpFrame.y); 
    float lumaEndN;
    float lumaEndP;
    bool doneN = false;
    bool doneP = false;
    posN += offNP * (-1.5);
    pos
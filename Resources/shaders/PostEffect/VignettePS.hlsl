#include "FullScreen.hlsli"
#include "../Utility/PostCalc.hlsli"
#include "../Utility/Gaussian.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<VignetteData> gVignette : register(b1);
ConstantBuffer<BloomData> gBloom : register(b6);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

///
/// ビネット
///
PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height; // uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
        
    int32_t kernelSize = 9;

    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    float32_t3 sceneColor = gTexture.Sample(gSampler, input.texcoord).rgb;
    
    float32_t weight = 0.0f;
    float32_t kernel5x5[9][9];
  
    for (int32_t x = 0; x < kernelSize; ++x)
    {
        for (int32_t y = 0; y < kernelSize; ++y)
        {
            kernel5x5[x][y] = Gauss(kIndex9x9[x][y].x, kIndex9x9[x][y].y, gBloom.gaussSigma);
            weight += kernel5x5[x][y];
        }
    }
        
    for (int32_t x2 = 0; x2 < kernelSize; ++x2)
    {
        for (int32_t y2 = 0; y2 < kernelSize; ++y2)
        {
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex9x9[x2][y2] * uvStepSize;
            
            // 4. 色に1/9掛けて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            
            // 5. 輝度に基づくスケーリング処理
            float32_t3 scaledColor = ScaleByLuminance(fetchColor, gBloom.threshold);
            
            // 6. スケールされた色にガウスカーネルを適用
            output.color.rgb += scaledColor * kernel5x5[x2][y2];
           //// 計算
            //float32_t3 brightFetchColor = ExtractBrightParts(fetchColor, gBloom.threshold);
            //output.color.rgb += brightFetchColor * kernel5x5[x2][y2];
            
        }
    }
    
    output.color.rgb *= rcp(weight);
    output.color.rgb += sceneColor;

    // 
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    
    float vignette = correct.x * correct.y * gVignette.scale;
    
    vignette = saturate(pow(vignette, gVignette.powValue));
    
    // ビネットの色
    float32_t3 vignetteColor = gVignette.color;
    
    float32_t3 vignetteEffect = (1.0 - vignette) * vignetteColor + vignette * output.color.rgb;

    output.color.rgb = vignetteEffect;
    
    return output;
}

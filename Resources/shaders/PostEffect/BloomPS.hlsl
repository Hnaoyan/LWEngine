#include "FullScreen.hlsli"
#include "../Utility/PostCalc.hlsli"
#include "../Utility/Gaussian.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

ConstantBuffer<BloomData> gBloom : register(b6);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float32_t3 ExtractBrightParts(float32_t3 color,float32_t threshold)
{  
    float luminance = dot(color.rgb, float32_t3(0.2126, 0.7152, 0.0722));
    if (luminance > threshold)
    {
        return color; // 輝度が閾値より高い場合は色をそのまま返す
    }
    return float32_t3(0, 0, 0); // 輝度が閾値以下の場合は透明にする
}

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
            
            // 計算
            float32_t3 brightFetchColor = ExtractBrightParts(fetchColor, gBloom.threshold);
            output.color.rgb += brightFetchColor * kernel5x5[x2][y2];
        }
    }
    
    output.color.rgb *= rcp(weight);
    output.color.rgb += sceneColor;
    //float32_t3 brightColor = ExtractBrightParts(sceneColor);
    //output.color.rgb = brightColor;
    return output;
}

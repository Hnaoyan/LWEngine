#include "FullScreen.hlsli"
#include "../Utility/HSV_RGB.hlsli"
#include "../Utility/Gaussian.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float32_t Luminance(float32_t3 color)
{
    return dot(color, float32_t3(0.2126, 0.7152, 0.0722));
}

PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height; // uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
        
    int32_t kernelSize = 5;

    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    float32_t3 sceneColor = gTexture.Sample(gSampler, input.texcoord).rgb;
    float32_t3 brightColor = float32_t3(0.0f, 0.0f, 0.0f);
    float32_t brightnessThreshold = 0.05f; // 明るさの閾値を設定
    //float32_t3 
    if (Luminance(sceneColor) > brightnessThreshold)
    {
        brightColor = sceneColor; // 明るい部分を抽出
    }

    
    float32_t weight = 0.0f;
    float32_t kernel5x5[5][5];
  
    for (int32_t x = 0; x < kernelSize; ++x)
    {
        for (int32_t y = 0; y < kernelSize; ++y)
        {
            kernel5x5[x][y] = Gauss(kIndex5x5[x][y].x, kIndex5x5[x][y].y, 2.0f);
            weight += kernel5x5[x][y];
        }
    }
        
    for (int32_t x2 = 0; x2 < kernelSize; ++x2)
    {
        for (int32_t y2 = 0; y2 < kernelSize; ++y2)
        {
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex5x5[x2][y2] * uvStepSize;
            // 4. 色に1/9掛けて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            
            // 計算
            if (Luminance(fetchColor) > brightnessThreshold)
            {
                output.color.rgb += fetchColor * kernel5x5[x2][y2];
            }
        }
    }
    
    output.color.rgb *= rcp(weight);
    output.color.rgb += brightColor;

    return output;
}

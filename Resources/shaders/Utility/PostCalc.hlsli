///
/// Dissolve
///
float32_t3 Dissolve(float32_t3 color, float32_t threshold, float32_t mask)
{
    float32_t edge = 1.0f - smoothstep(threshold, threshold + 0.03f, mask);
    float32_t3 output = edge * color;
    return output;
}

void ProcessMask(float32_t mask, float32_t threshold)
{
    if (mask < threshold)
    {
        discard;
    }
}

///
/// Luminance
///
float32_t Luminance(float32_t3 color)
{
    return dot(color, float32_t3(0.2126f, 0.7152f, 0.0722f));
}

///
/// Wrap
///
float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}
///
/// RGB→HSV
///
float32_t3 RGBToHSV(float32_t3 rgb)
{
    // RGB
    float32_t r = rgb.r;
    float32_t g = rgb.g;
    float32_t b = rgb.b;
    
    float32_t max = r > g ? r : g;
    max = max > b ? max : b;
    float32_t min = r < g ? r : g;
    min = min < b ? min : b;
    float32_t h = max - min;
    if (h > 0.0f)
    {
        if (max == r)
        {
            h = (g - b) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == g)
        {
            h = 2.0f + (b - r) / h;
        }
        else
        {
            h = 4.0f + (r - g) / h;

        }
    }
    
    h /= 6.0f;
    float32_t s = (max - min);
    if (max != 0.0f)
    {
        s /= max;
    }
    float32_t v = max;
    return float32_t3(h, s, v);
}

///
/// HSV→RGB
///
float32_t3 HSVToRGB(float32_t3 hsv)
{
    // HSV
    float32_t h = hsv.r;
    float32_t s = hsv.g;
    float32_t v = hsv.b;
    // RGB
    float32_t r = v;
    float32_t g = v;
    float32_t b = v;
    if (s > 0.0f)
    {
        h *= 6.0f;
        const int32_t i = (int32_t) h;
        const float32_t f = h - (float32_t) i;
        switch (i)
        {
            case 0:
                g *= 1.0f - s * (1.0f - f);
                b *= 1.0f - s;
                break;
            case 1:
                r *= 1.0f - s * f;
                b *= 1.0f - s;
                break;
            case 2:
                r *= 1.0f - s;
                b *= 1.0f - s * (1.0f - f);
                break;
            case 3:
                r *= 1.0f - s;
                g *= 1.0f - s * f;
                break;
            case 4:
                r *= 1.0f - s * (1.0f - f);
                g *= 1.0f - s;
                break;
            case 5:
                g *= 1.0f - s;
                b *= 1.0f - s * f;
                break;
        }
    }
    return float32_t3(r, g, b);
}


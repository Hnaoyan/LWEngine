///
/// ScaleCheck
///
float32_t3 ScaleCheck(float32_t3 scale)
{
    float32_t3 result = scale;
    if (scale.x < 0)
    {
        result.x = 0.0f;
    }
    if (scale.y < 0)
    {
        result.y = 0.0f;
    }
    if (scale.z < 0)
    {
        result.z = 0.0f;
    }
    return result;
}

float32_t Easing(float32_t start, float32_t end, float32_t t)
{    
    return float32_t((start * (1.0f - t)) + (end * t));
}

float32_t2 Easing(float32_t2 start, float32_t2 end, float32_t t)
{
    float32_t2 result;
    
    result.x = Easing(start.x, end.x, t);
    result.y = Easing(start.y, end.y, t);
   
    return result;
}

float32_t3 Easing(float32_t3 start, float32_t3 end, float32_t t)
{
    float32_t3 result;
    
    result.x = Easing(start.x, end.x, t);
    result.y = Easing(start.y, end.y, t);
    result.z = Easing(start.z, end.z, t);
   
    return result;
}

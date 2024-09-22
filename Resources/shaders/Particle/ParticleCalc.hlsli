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

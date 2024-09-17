static const float32_t PI = 3.14159265f;

float Gauss(float x, float y, float sigma)
{
    float exponent = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponent) * rcp(denominator);
}

static const float32_t2 kIndex3x3[3][3] =
{
    {
        { -1.0f, -1.0f },
        { 0.0f, -1.0f },
        { 1.0f, -1.0f }
    },
    {
        { -1.0f, 0.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f }
    },
    {
        { -1.0f, 1.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f }
    },
};

static const float32_t2 kIndex5x5[5][5] =
{
    { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
    { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
    { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
    { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
    { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } },
};

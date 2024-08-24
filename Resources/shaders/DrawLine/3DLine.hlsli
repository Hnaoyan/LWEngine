struct VSOutput
{
    float32_t4 position : SV_POSITION;
    float32_t4 color : COLOR0;
    
    float32_t3 worldPosition : POSITION0;
};

static const uint32_t vertexNum = 2;

struct Vertex
{
    float32_t4 position[vertexNum];
    float32_t4 color[vertexNum];
};

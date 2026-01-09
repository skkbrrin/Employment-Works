#include "Particle.hlsli"

static const int vnum = 4;

static const float2 uv_array[vnum] =
{
    float2(0, 0),
    float2(1, 0),
    float2(0, 1),
    float2(1, 1),
};

static const float2 offset_array[vnum] =
{
    float2(-1, 1),
    float2(1, 1),
    float2(-1, -1),
    float2(1, -1),
};


[maxvertexcount(4)]
void main(
    point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output
)
{
    float size = 0.6f;

    float3 camRight = float3(matView._11, matView._21, matView._31);
    float3 camUp    = float3(matView._12, matView._22, matView._32);

    float3 center = input[0].Pos.xyz;

    for (int i = 0; i < vnum; i++)
    {
        PS_INPUT o;

        float3 pos =
            center
            + camRight * offset_array[i].x * size
            + camUp    * offset_array[i].y * size;

        float4 wp = float4(pos, 1.0f);
        wp = mul(wp, matView);
        wp = mul(wp, matProj);

        o.Pos   = wp;
        o.Color = input[0].Color;
        o.Tex   = uv_array[i];
        o.Tex2  = uv_array[i];

        output.Append(o);
    }

    output.RestartStrip();
}
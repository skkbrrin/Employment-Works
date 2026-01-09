Texture2D tex0 : register(t0);
SamplerState sam0 : register(s0);

cbuffer TimerCB : register(b1)
{
    float ratio;
    float3 padding;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
    float2 TexCoord : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    float2 uv = input.TexCoord - 0.5;
    float angle = atan2(uv.y, uv.x);
    if (angle < 0)
        angle += 6.2831853;

    float t = angle / 6.2831853;

    if (t > ratio)
        discard;

    return tex0.Sample(sam0, input.TexCoord) * input.Color;
}

#include "Particle.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

// ブラー
float4 blur(float2 uv)
{
    float power = 0.05f; //< 大きくすると版画みたいになる
    float center = 1.0f - power * 10.0f; //< 大きくすると光る
    float4 output = 0;
    for (int i = 0; i < 10; i++)
    {
        output += tex.Sample(samLinear, uv + float2(((i % 3) -  1) * 0.01f, ((i / 3) - 1) * 0.01f)) * power; //< ぶれ方が大きく変わる
    }
    output += tex.Sample(samLinear, uv) * center;
    return output;
}


// メイン
float4 main(PS_INPUT input) : SV_TARGET
{
	//	画像表示
    float4 output = tex.Sample(samLinear, input.Tex);
   
    // ブラー
    output = blur(input.Tex);
    
    return output;
}
#include "UI.hlsli"


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //	float型のratioに現在のゲージの位置が入っているようにしてください。
    //	0がゲージ0、1がゲージ最大の状態となります
    float2 center = float2(0.5f, 0.5f);
	
	//	指定された画像からUV座標に対応した色を取得
    float4 output = tex.Sample(samLinear, input.tex);

	//  描画しようとしているUV座標から中心までのベクトルを計算
    float2 dir = input.tex - center;
    
    //  ベクトルの長さを計算
    float len = length(dir);
    
    //  ベクトルの角度を計算（ラジアン）
    float angle = atan2(dir.y, dir.x);
    
    //  角度を0～1の範囲に正規化（真上が0、時計回りとするための微調整も含む）
    float normalizedAngle = frac((1.6f + angle) / (2.0f * 3.14159265359f));
    normalizedAngle = 1.0f - normalizedAngle;
    //  指定されているゲージの表示割合と比較し、表示するかどうかを決定
    float alpha = step(len, 0.5f) * step(normalizedAngle, alphaDate);
    
    return float4(output.rgb, output.a * alpha);
}
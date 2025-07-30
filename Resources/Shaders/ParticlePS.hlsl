//	ピクセルシェーダ

//	レジスターは、ゲームプログラム（C++・CPU）からシェーダプログラム（HLSL・GPU）へ情報データを渡すために使用する。
//	「register」句の後に「(xy)」を指定する。
//	xに当てはまるのは以下の通り。（DirectX11バージョン）
//		t - シェーダー リソース ビュー (SRV) 用
//		s - サンプラー用
//		u - 順序指定されていないアクセス ビュー (UAV) 用
//		b - 定数バッファー ビュー (CBV) 用
//	yに当てはまるのはスロット番号。
//	上記にある各用途別の最大スロット数は以下の通り（DirectX11バージョン）
//		t → D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNTより、最大スロット数128
//		s → D3D11_COMMONSHADER_SAMPLER_SLOT_COUNTより、最大スロット数16
//		u → 恐らく、GPUハードウェア上のメモリ制限により変動
//		b → D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNTより、最大スロット数15
//
//	また、レジスターは基本的に数値型かポインタ型しか入ることはない。
//	シェーダ側では、ゲームプログラム側から受け取ったデータへは「レジスタ」+「スロット番号」の形で格納場所を管理しているが、
//	各「レジスタ」+「スロット番号」へは、どのような形のデータを入れているかを教えてあげないと、HLSL上で取り扱うことは不可能。
//	そのため、ファイル冒頭ではそれらの定義を行っておくことが必要となる

//	b0にはマトリックス3つとディフューズ情報を構造体として入れることにする
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;
};

//	t0にはテクスチャ情報を入れていることにする
Texture2D tex : register(t0);
//	s0にはサンプラー情報を入れていることにする
SamplerState samLinear : register(s0);

//	引数として受け取りたいデータの構造体を定義
struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	//変数texで指定された画像の、該当する座標にある色を取得する
	float4 output = tex.Sample(samLinear, input.Tex);

	//R,G,B,A全てが1の真っ白ピクセルを返す場合は、以下をreturnする
	float4 outputw = float4(1, 1, 1, 1);
	
	return output;
}
//-------------------------------------------------------------------------------------
// File: FloorPrimitive.h
//
// render floor by primitive
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "FloorPrimitive.h"
#include <PrimitiveBatch.h> 
#include <VertexTypes.h> 
#include <WICTextureLoader.h> 

using namespace DirectX;

//	今回は、3D空間の頂点座標と、各頂点に設定するUV座標だけを指定する
//	TODO:01)頂点は4つのみとし、画像を10回ループさせるにはUV座標としてどんな値を設定すれば良いか？
//			※初期状態は1枚がベタ張りされる設定
const VertexPositionTexture kHorikawa::FloorPrimitive::VERTICES[4] =
{
	VertexPositionTexture(DirectX::SimpleMath::Vector3( 30.0f, 0.0f, -30.0f), DirectX::SimpleMath::Vector2( 30.0f,  0.0f)),  // 0:左上
	VertexPositionTexture(DirectX::SimpleMath::Vector3( 30.0f, 0.0f,  30.0f), DirectX::SimpleMath::Vector2( 30.0f,  30.0f)),  // 1:右上
	VertexPositionTexture(DirectX::SimpleMath::Vector3(-30.0f, 0.0f,  30.0f), DirectX::SimpleMath::Vector2( 0.0f,  30.0f)),  // 3:右下
	VertexPositionTexture(DirectX::SimpleMath::Vector3(-30.0f, 0.0f, -30.0f), DirectX::SimpleMath::Vector2( 0.0f,  0.0f)),  // 2:左下
};

/// <summary>
/// Constractor
/// </summary>
kHorikawa::FloorPrimitive::FloorPrimitive(ID3D11Device1* device)
{   
	//	エフェクトの作成 
	m_BatchEffect = std::make_unique<AlphaTestEffect>(device);
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_BatchEffect->SetReferenceAlpha(255);

	//	入力レイアウト生成 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_BatchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_InputLayout.GetAddressOf()
	);

	//	共通ステートの作成
	m_States = std::make_unique<CommonStates>(device);

	//	テクスチャのロード
	//	TODO:02)「Resources/Textures/floor.png」ファイルを読み込む、CreateWICTextureFromFile関数の
	//			呼び出しを記述する。
	//			ヒント：m_Texture変数はID3D11ShaderResourceView*型を持つunique_ptrなので、
	//					第三引数がnullptr、第四引数がnullptr以外、となる
	DirectX::CreateWICTextureFromFile(
		device, //< ID3D11Device*の変数　引数で受け取り
		L"Resources/Textures/Floor.png", //< 画像のファイル名
		nullptr,
		m_Texture.GetAddressOf()//< テクスチャのアドレス
		);

	//	TODO:03)テクスチャデータをBatchEffectに設定する
	m_BatchEffect->SetTexture(m_Texture.Get());

	//	初期化（原点）
	m_position = DirectX::SimpleMath::Vector3::Zero;
}

/// <summary>
/// Destractor
/// </summary>
kHorikawa::FloorPrimitive::~FloorPrimitive()
{

}

/// <summary>
/// Render 
/// </summary>
void kHorikawa::FloorPrimitive::Render(ID3D11DeviceContext1* context,SimpleMath::Matrix view, SimpleMath::Matrix proj )
{
	//	プリミティブバッチの作成 
	m_Batch =
		std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	頂点情報（板ポリゴンの頂点） 
	VertexPositionTexture vertex[4];
	//	TODO:04)vertex変数を、DrawQuad関数に渡して描画したい。
	//			VERTICESの内容をコピーする処理を記述しよう
	for(int i = 0; i < 4; i++)
	{
		vertex[i] = VERTICES[i];
	}


	//	テクスチャサンプラーの設定
	//	TODO:05)サンプラーの設定を、PointWrapにしよう
	ID3D11SamplerState* samplers[1] = { m_States->PointWrap() };
	context->PSSetSamplers(0, 1, samplers);

	//	深度バッファに書き込み参照する 
	context->OMSetDepthStencilState(m_States->DepthDefault(), 0);

	//	裏面は描画しない
	context->RSSetState(m_States->CullNone());

	//	不透明のみ描画する設定 
	m_BatchEffect->SetAlphaFunction(D3D11_COMPARISON_NOT_EQUAL);
	m_BatchEffect->SetReferenceAlpha(0);
	m_BatchEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	m_BatchEffect->SetView(view);
	m_BatchEffect->SetProjection(proj);
	m_BatchEffect->Apply(context);
	context->IASetInputLayout(m_InputLayout.Get());

	//	半透明部分が含まれることを想定した描画 
	m_Batch->Begin();
	//	TODO:06)DrawQuad関数を使って頂点の描画が出来るように、引数の記述をしよう
	//			ヒント：頂点データはvertexの内容を使用する
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
/// <summary>
/// Finalize 
/// </summary>
void kHorikawa::FloorPrimitive::Finalize()
{

}

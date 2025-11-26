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

const VertexPositionTexture kHorikawa::FloorPrimitive::VERTICES[4] =
{
	VertexPositionTexture(DirectX::SimpleMath::Vector3( 100.0f, 0.0f, -100.0f), DirectX::SimpleMath::Vector2( 100.0f,  0.0f)),  // 0:左上
	VertexPositionTexture(DirectX::SimpleMath::Vector3( 100.0f, 0.0f,  100.0f), DirectX::SimpleMath::Vector2( 100.0f, 100.0f)),  // 1:右上
	VertexPositionTexture(DirectX::SimpleMath::Vector3(-100.0f, 0.0f,  100.0f), DirectX::SimpleMath::Vector2( 0.0f,  100.0f)),  // 3:右下
	VertexPositionTexture(DirectX::SimpleMath::Vector3(-100.0f, 0.0f, -100.0f), DirectX::SimpleMath::Vector2( 0.0f,  0.0f)),  // 2:左下
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
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/Floor.png",
		nullptr,
		m_Texture.GetAddressOf()
		);

	m_BatchEffect->SetTexture(m_Texture.Get());

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
	for(int i = 0; i < 4; i++)
	{
		vertex[i] = VERTICES[i];
	}


	//	テクスチャサンプラーの設定
	ID3D11SamplerState* samplers[1] = { m_States->LinearWrap()};
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
	m_Batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_Batch->End();
}
/// <summary>
/// Finalize 
/// </summary>
void kHorikawa::FloorPrimitive::Finalize()
{

}

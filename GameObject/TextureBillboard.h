#pragma once
#include <DeviceResources.h>
#include "StepTimer.h"
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

class TextureBillboard
{
public:
    struct ConstBuffer
    {
        DirectX::SimpleMath::Matrix matWorld; //< ワールド
        DirectX::SimpleMath::Matrix matView; //< ビュー
        DirectX::SimpleMath::Matrix matProj; //< 射影行列
        DirectX::SimpleMath::Vector4 Diffuse; //< 拡散
    };
private:
	DX::DeviceResources* m_pDR;						//< m_deviceResources.get()
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer; //< バッファ
	DX::StepTimer m_timer;                          //< タイマー

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_Batch;
	std::unique_ptr<DirectX::CommonStates> m_states;

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader; //< 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader; //< ピクセルシェーダ

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj; 
	DirectX::SimpleMath::Matrix m_billboard;

	DirectX::SimpleMath::Vector3 m_position;

public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

    TextureBillboard();
    ~TextureBillboard();

	void LoadTexture(const wchar_t* path);
	void Create(DX::DeviceResources* pDR, ID3D11Device* device);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void CreateBillboard(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up);

private:
	void CreateShader();
};


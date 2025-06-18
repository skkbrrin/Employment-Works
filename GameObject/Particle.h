//--------------------------------------------------------------------------------------
// File: Particle.h
//
// パーティクルクラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
namespace kHorikawa
{
	class Particle
	{
	public:
		//	データ受け渡し用コンスタントバッファ(送信側)
		struct ConstBuffer
		{
			DirectX::SimpleMath::Matrix		matWorld;
			DirectX::SimpleMath::Matrix		matView;
			DirectX::SimpleMath::Matrix		matProj;
			DirectX::SimpleMath::Vector4	Diffuse;
		};

		struct GSConstBuffer
		{
			float value1;
			float value2;
			float value3;
			float value4;
		};

	private:
		//	変数
		DX::DeviceResources* m_pDR;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	m_GSBuffer;

		float m_sizeDate;

		DX::StepTimer                           m_timer;
		//	入力レイアウト
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

		//	プリミティブバッチ
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
		//	コモンステート
		std::unique_ptr<DirectX::CommonStates> m_states;
		//	テクスチャハンドル
		std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;
		//	テクスチャハンドル
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
		//	頂点シェーダ
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		//	ピクセルシェーダ
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		//	ジオメトリシェーダ
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

		DirectX::SimpleMath::Matrix m_world;
		DirectX::SimpleMath::Matrix m_view;
		DirectX::SimpleMath::Matrix m_proj;

	public:
		//	関数
		static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

		Particle();
		~Particle();

		void LoadTexture(const wchar_t* path);
		
		void Create(DX::DeviceResources* pDR);

		void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	private:

		void CreateShader();
	};
}
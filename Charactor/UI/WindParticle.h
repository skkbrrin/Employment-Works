//--------------------------------------------------------------------------------------
//	File: WindParticle.h
//
//	パーティクルクラス
//	未拡張の場合はPosition情報のみ
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

class WindParticle
{
public:
	struct WindUnit
	{
		DirectX::SimpleMath::Vector3 pos;
		float angle;   // 円周上の角度
		float time;
	};


	//	データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	time;
	};

private:
	//	変数
	DX::DeviceResources* m_pDR;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	DX::StepTimer                           m_timer;
	//	入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
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

	float m_time;

	bool m_active = false;

	std::vector<WindUnit> m_units;

	DirectX::SimpleMath::Vector3 m_centerPos;

	float m_radius = 1.2f;      // プレイヤー周りの半径
	float m_lifeTime = 0.8f;
	int   m_unitCount = 12;     // 風の枚数

public:
	//	関数
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	WindParticle();
	~WindParticle();

	void LoadTexture(const wchar_t* path);
	
	void Create(DX::DeviceResources* pDR);

	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Start() { m_active = true; }
	void Stop() { m_active = false; }
	bool IsActive() const { return m_active; }

	void Burst(const DirectX::SimpleMath::Vector3& center);
private:

	void CreateShader();
};

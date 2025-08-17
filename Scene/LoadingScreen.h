/*
タイトルシーン
*/
#pragma once

#include <vector>
#include "ItoLib/SceneManager.h"
#include "ItoLib/UserResources.h"


class LoadingScreen : public Ito::LoadingScreen<UserResources>
{
private:


public:

	// コンストラクタ
	LoadingScreen();

	// デストラクタ
	~LoadingScreen();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 終了処理
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backSRV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_moziSRV;

	float timer;
	float m_rotation = 0.0f;
};
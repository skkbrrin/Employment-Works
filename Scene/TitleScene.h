#pragma once

#include "../ItoLib/SceneManager.h"
#include "../ItoLib/DebugCamera.h"
#include "../ItoLib/UserResources.h"
#include "../GameObject/Menu.h"
#include "DeviceResources.h"

class TitleScene : public Ito::Scene<UserResources>
{
public:
	TitleScene() = default;
	~TitleScene() = default;

	// 初期化
	void Initialize() override;

	// 更新
	void Update(float elapsedTime) override; 

	// 描画
	void Render() override; 

	// 終了
	void Finalize() override; 

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:
	// カメラのポインタ
	std::unique_ptr<Ito::DebugCamera> m_debugCamera;

	// メニューポインタ
	std::unique_ptr<kHorikawa::Menu> m_menu;

	// デバイスリソース
	std::unique_ptr<DX::DeviceResources> m_deviceresources;
public:

};


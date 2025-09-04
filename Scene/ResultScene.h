#pragma once

#include "../ItoLib/SceneManager.h"
#include "../ItoLib/DebugCamera.h"
#include "../ItoLib/UserResources.h"

#include "Charactor/ScoreManager.h"
#include "ItoLib/TaskManager.h"
#include "Charactor/Task/Number.h"

class ResultScene : public Ito::Scene<UserResources>
{
public:
	// コンストラクタ
	ResultScene() = default; 
	~ResultScene() = default; 
	
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
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// カメラのポインタ
	std::unique_ptr<Ito::DebugCamera> m_debugCamera;

	std::unique_ptr<ScoreManager> m_scoreManager;
	Ito::TaskManager m_taskManager;
	Number* m_number;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_numberSRV; // 数字

};


#pragma once

#include "ItoLib/SceneManager.h"
#include "ItoLib/DebugCamera.h"
#include "ItoLib/UserResources.h"
#include "ItoLib/GridFloor.h"
#include "GameObject/FloorPrimitive.h"
#include "ItoLib/GameCamera.h"

#include "Charactor/Player/Player.h"

class TutorialScene : public Ito::Scene<UserResources>
{
public:
	// コンストラクタ
	TutorialScene() = default; 
	~TutorialScene() = default; 
	
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

	// グリッド床
	std::unique_ptr<Ito::GridFloor> m_gridFloor;

	// 空
	std::unique_ptr<DirectX::Model> m_skyModel;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// ベーシックエフェクトへのポインタ
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// プリミティブバッチへのポインタ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 入力レイアウトへのポインタ
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<kHorikawa::FloorPrimitive> m_floorPrimitive;

	// プレイヤー
	std::unique_ptr<DirectX::Model> m_enemy;
	DirectX::SimpleMath::Vector3 m_posE;

	std::unique_ptr<Player> m_player;
	bool m_cameraDebug = false;
	bool m_cameraGame = false;

	bool act = false;
	bool gameC = false;

	GameCamera m_camera;

private:
	float m_skyRotate = 0.0f;

public :
	float GetSkyRotation() { return m_skyRotate; }

};


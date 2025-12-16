#pragma once

#include "ItoLib/SceneManager.h"
#include "ItoLib/DebugCamera.h"
#include "ItoLib/UserResources.h"
#include "ItoLib/GridFloor.h"
#include "GameObject/FloorPrimitive.h"
#include "ItoLib/GameCamera.h"

#include "Charactor/Field.h"

#include "ItoLib/TaskManager.h"
#include "Charactor/Task/Number.h"

#include <Audio.h>

class PlayScene : public Ito::Scene<UserResources>
{
public:
	// コンストラクタ
	PlayScene() = default;
	~PlayScene() = default;

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

	// 床
	std::unique_ptr<kHorikawa::FloorPrimitive> m_floorPrimitive;

	// カメラ
	GameCamera m_camera;

	// 音楽
	std::unique_ptr<DirectX::AudioEngine> m_audioE;
	std::unique_ptr<DirectX::SoundEffect> m_bgm;
	std::unique_ptr<DirectX::SoundEffectInstance> m_bgmInstance;

	// フィールド
	std::unique_ptr<Field> m_field;
};


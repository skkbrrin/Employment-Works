#pragma once

#include "ItoLib/SceneManager.h"
#include "ItoLib/UserResources.h"

#include <Audio.h> 

#include "GameObject/TextureAlpha.h"
#include "GameObject/TextureSprite.h"

class TitleScene : public Ito::Scene<UserResources>
{
public:

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
	std::unique_ptr<DirectX::AudioEngine> audioEngine;
	std::unique_ptr<DirectX::SoundEffect> bgm;
	std::unique_ptr<DirectX::SoundEffectInstance> bgmInstance;

	std::unique_ptr<TextureSprite> m_textureSprite;
	std::unique_ptr<TextureAlpha> m_tectureAlpha;

	float timer;
	DirectX::XMVECTORF32 color;

	std::unique_ptr<DirectX::CommonStates> m_states;
};


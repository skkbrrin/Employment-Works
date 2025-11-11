#pragma once

#include "ItoLib/SceneManager.h"
#include "ItoLib/UserResources.h"

#include <Audio.h> 

#include "GameObject/TextureAlpha.h"
#include "GameObject/TextureSprite.h"

#include "Charactor/KeyConfig.h"
#include <vector>
#include <string>

class OptionScene : public Ito::Scene<UserResources>
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

private:
	std::unique_ptr<DirectX::AudioEngine> audioEngine;
	std::unique_ptr<DirectX::SoundEffect> bgm;
	std::unique_ptr<DirectX::SoundEffectInstance> bgmInstance;

	std::unique_ptr<TextureSprite> m_textureSprite;
	std::unique_ptr<TextureAlpha> m_tectureAlpha;

	KeyConfig* m_keyConfig; //< ゲームから渡されるキー設定
	int m_selectedIndex = 0; //< キー選択項目
	bool m_waitingForKey = false; //<　新しいキーを待っているかどうか

	enum class OptionMode
	{
		MainMenu,
		KeyCnofingtion
	};

	OptionMode m_mode = OptionMode::MainMenu;

	int m_keyConfigIndex = 0;

	struct MenuItem
	{
		std::string name;
	};
	std::vector<MenuItem> m_menuItems;

	struct KeyItem
	{
		std::string name;
		ActionID action;
	};
	std::vector<KeyItem> m_keyItems;

	int m_tempKey = 0; //< Spaceで確定する前の仮設定キー
};


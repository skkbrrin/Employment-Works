#include "pch.h"
#include "OptionScene.h"

#include <CommonStates.h>
#include <iostream>

using namespace DirectX;

void OptionScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	if (!m_keyConfig)
		m_keyConfig = GetUserResources()->GetKeyConfig();

	// メニュー名
	m_menuItems = {
		{"Key Config"},
		{"Volume"},
		{"Fullscreen"},
		{"Back"},
	};

	// キー設定名
	m_keyItems = {
		{"Mode Forward", ActionID::Forward},
		{"Mode Avoid", ActionID::Avoid},
		{"Mode LeftRotation", ActionID::LeftRotation},
		{"Mode RigthRotation", ActionID::RigthRotation},
		{"Mode NormalAttack", ActionID::NormalAttack},
		{"Mode ComboAttack", ActionID::ComboAttack},
		{"Mode Pause", ActionID::Pause},
	};

	// BGM
	AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
	audioEngine = std::make_unique<AudioEngine>(flags);

	try {
		bgm = std::make_unique<SoundEffect>(audioEngine.get(), L"Resources/Sounds/Momiziyado.wav");
		bgmInstance = bgm->CreateInstance();
		bgmInstance->Play(true);
	}
	catch (const std::exception& e) {
		OutputDebugStringA(e.what());
	}

}

void OptionScene::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	auto kb = GetUserResources()->GetKeyboardStateTracker();

	//m_tectureAlpha->Update(elapsedTime);

	// 通常メニュー
	if (m_mode == OptionMode::MainMenu)
	{
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			m_selectedIndex = (m_selectedIndex - 1 + m_menuItems.size()) % m_menuItems.size();
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			m_selectedIndex = (m_selectedIndex + 1 + m_menuItems.size()) % m_menuItems.size();

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			switch (m_selectedIndex)
			{
			case 0:
				m_mode = OptionMode::KeyCnofingtion;
				m_keyConfigIndex = 0;
				break;

			case 1:
				std::cout << "音量設定\n";
				break;
			case 2:
				std::cout << "フルスクリーン設定\n";
				break;
			case 3:
				std::cout << "タイトルへ戻る\n";
				break;
			}
		}
	}
	// キー設定メニュー
	else if (m_mode == OptionMode::KeyCnofingtion)
	{
		if (!m_waitingForKey)
		{
			// 上下で項目選択
			if (GetAsyncKeyState(VK_UP) & 0x8000)
				m_keyConfigIndex = (m_keyConfigIndex - 1 + m_keyItems.size()) % m_keyItems.size();
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
				m_keyConfigIndex = (m_keyConfigIndex + 1 ) % m_keyItems.size();

			// Space押下で決定
			if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			{
				const auto& item = m_keyItems[m_keyConfigIndex];
				m_keyConfig->SetKey(item.action, m_tempKey);
				m_keyConfig->Save("Resources/File/keycon.json");
				std::cout << "Saved new key for " << item.name << "!\n";
				Sleep(200); // 連続入力防止
			}

			// ESCで戻る
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				m_mode = OptionMode::MainMenu;
				Sleep(200);
			}

			// 任意のキーが押されたら、仮設定として保持
			for (int vk = 0x30; vk <= 0x5A; ++vk)
			{
				if (GetAsyncKeyState(vk) & 0x8000)
				{
					m_tempKey = vk;
					break;
				}
			}
		}
	}
}

void OptionScene::Render()
{
	system("cls");

	if (m_mode == OptionMode::MainMenu)
	{
		std::cout << "=== OPTION MENU ===\n";
		for (size_t i = 0; i < m_menuItems.size(); ++i)
		{
			std::cout << (i == m_selectedIndex ? "> " : "  ")
				<< m_menuItems[i].name << "\n";
		}
	}
	else if (m_mode == OptionMode::KeyCnofingtion)
	{
		std::cout << "=== KEY CONFIG ===\n";
		for (size_t i = 0; i < m_keyItems.size(); ++i)
		{
			int key = m_keyConfig->GetKey(m_keyItems[i].action);
			std::cout << (i == m_keyConfigIndex ? "> " : "  ")
				<< m_keyItems[i].name << " : " << (char)key << "\n";
		}

		std::cout << "\n↑↓で選択, 任意キーで仮設定, SPACEで確定, ESCで戻る\n";
		if (m_tempKey)
			std::cout << "現在入力中: " << (char)m_tempKey << "\n";
	}
}

void OptionScene::Finalize()
{
	// ファイルのセーブ
	m_keyConfig->Save("Resources\File\keycon.json");

	// 終了処理
	bgmInstance.reset();
	bgm.reset();
	audioEngine.reset();
}

void OptionScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();

	m_textureSprite = std::make_unique<TextureSprite>(device, context);
	m_tectureAlpha = std::make_unique<TextureAlpha>(device, context);

	//m_textureSprite->Load(L"Resources/Textures/Option.dds");
	//m_tectureAlpha->Load(L"Resources/Textures/Enter.dds");
}

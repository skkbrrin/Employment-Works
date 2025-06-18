//--------------------------------------------------------------------------------------
// File: Menu.h
//
// メニュークラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "StepTimer.h"
#include "UserInterface.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
namespace kHorikawa
{
	
	class Menu
	{
	public:

//	変数
	private:

		unsigned int m_menuIndex;
		DX::DeviceResources* m_pDR;

		std::vector<std::unique_ptr<kHorikawa::UserInterface>> m_userInterface;
		std::vector<std::unique_ptr<kHorikawa::UserInterface>> m_base;

		const wchar_t* m_baseTexturePath;

		std::unique_ptr<kHorikawa::UserInterface> m_baseWindow;

		int m_windowWidth, m_windowHeight;

		DirectX::Keyboard::KeyboardStateTracker m_tracker;
//	関数
	public:
		Menu();
		~Menu();

		void Initialize(DX::DeviceResources* pDR, int width, int height);
		void Update();
		void Render();

		void Add(const wchar_t* path
			, DirectX::SimpleMath::Vector2 position
			, DirectX::SimpleMath::Vector2 scale
			, kHorikawa::ANCHOR anchor);

	};
}
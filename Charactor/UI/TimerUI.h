#pragma once

#include "StepTimer.h"
#include "GameObject/UserInterface.h"
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include "Keyboard.h"


class TimerUI
{
public:

	//ïœêî
private:

	unsigned int m_menuIndex;
	DX::DeviceResources* m_pDR;

	std::unique_ptr<kHorikawa::UserInterface> m_gauge;
	std::unique_ptr<kHorikawa::UserInterface> m_frame;
	std::unique_ptr<kHorikawa::UserInterface> m_base;


	const wchar_t* m_baseTexturePath;

	std::unique_ptr<kHorikawa::UserInterface> m_baseWindow;

	int m_windowWidth, m_windowHeight;

	bool m_isRunning = true;


	//ä÷êî
public:
	TimerUI();
	~TimerUI();

	void Initialize(DX::DeviceResources* pDR, int width, int height);
	void Update(float dt);
	void Render();

	void Add(const wchar_t* path
		, DirectX::SimpleMath::Vector2 position
		, DirectX::SimpleMath::Vector2 scale
		, kHorikawa::ANCHOR anchor);

	float GetRemainingTime() const;

	bool IsTimeUp() const
	{
		return m_gauge->GetRenderRatio() <= 1.0f;
	}
};


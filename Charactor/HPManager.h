//--------------------------------------------------------------------------------------
// File: HPManager.h
//-------------------------------------------------------------------------------------

#pragma once
#include "StepTimer.h"
#include "GameObject/UserInterface.h"
#include <DeviceResources.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>


class HPManager {
public:

private:
	DX::DeviceResources* m_pDR; // デバイスリソース

	std::unique_ptr<kHorikawa::UserInterface> m_gauge; // ゲージ
	std::unique_ptr<kHorikawa::UserInterface> m_frame; // 枠
	std::unique_ptr<kHorikawa::UserInterface> m_base; // 背景ゲージ

	const wchar_t* m_baseTexturePath; // 背景ゲージテクスチャパス
	const wchar_t* m_frameTexturePath; // 枠テクスチャパス
	const wchar_t* m_gaugeTexturePath; // ゲージテクスチャパス

public:
	HPManager();
	~HPManager();

	void Initialize(DX::DeviceResources* pDR);
	void Update(int HP, int maxHP);
	void Render();

	void Creates( DirectX::SimpleMath::Vector2 pos
		, DirectX::SimpleMath::Vector2 scale);
	
// 読み込み
public:
	void LoadBase(const wchar_t* base) { m_baseTexturePath = base; }
	void LoadFrame(const wchar_t* frame) { m_frameTexturePath = frame; }
	void LoadGauge(const wchar_t* gauge) { m_gaugeTexturePath = gauge; }
};

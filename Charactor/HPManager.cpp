//-------------------------------------------------------------------------------------
// File: HPManager.h
//
// render floor by primitive
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "HPManager.h"
#include "GameObject/UserInterface.h"

#include "GameObject/BinaryFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
using namespace DirectX;

static float Clamp(float v, float lo, float hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}


HPManager::HPManager()
	: m_pDR(nullptr)
	, m_baseTexturePath(nullptr)
	, m_frameTexturePath(nullptr)
	, m_gaugeTexturePath(nullptr)
	, m_base(nullptr)
	, m_frame(nullptr)
	, m_gauge(nullptr)
{
}

HPManager::~HPManager()
{
}

void HPManager::Initialize(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	Creates(SimpleMath::Vector2{0.0f, 0.0f}, SimpleMath::Vector2{0.5f, 0.5f});
	
	RECT rect = m_pDR->GetOutputSize();
	m_base->SetWindowSize(rect.right, rect.bottom);
	m_gauge->SetWindowSize(rect.right, rect.bottom);
	m_frame->SetWindowSize(rect.right, rect.bottom);

}

void HPManager::Update(int HP, int maxHP)
{
	float ratio = m_gauge->GetRenderRatio();

	if (maxHP > 0)
	{
		ratio = static_cast<float>(HP) / static_cast<float>(maxHP);
		ratio = Clamp(ratio, 0.0f, 1.0f); // •‰‚â1’´‚¦‚ð–h‚®
	}

	m_gauge->SetRenderRatio(ratio);
}

void HPManager::Render()
{
	m_base->Render();
	m_gauge->Render();
	m_frame->Render();
}

void HPManager::Creates(DirectX::SimpleMath::Vector2 pos, DirectX::SimpleMath::Vector2 scale)
{
	SimpleMath::Vector2 gaugePos = SimpleMath::Vector2{ pos.x + 62.0f, pos.y - 0.2f };

	m_base = std::make_unique<kHorikawa::UserInterface>();
	m_base->Create(m_pDR, m_baseTexturePath, gaugePos, scale, kHorikawa::ANCHOR::TOP_LEFT);

	m_gauge = std::make_unique<kHorikawa::UserInterface>();
	m_gauge->Create(m_pDR, m_gaugeTexturePath, gaugePos, scale, kHorikawa::ANCHOR::TOP_LEFT);
	m_gauge->SetRenderRatioOffset(0.1f);

	m_frame = std::make_unique<kHorikawa::UserInterface>();
	m_frame->Create(m_pDR, m_frameTexturePath, pos, scale, kHorikawa::ANCHOR::TOP_LEFT);
}


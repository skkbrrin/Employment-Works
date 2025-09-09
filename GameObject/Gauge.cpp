//--------------------------------------------------------------------------------------
// File: Gauge.cpp
//
// ÉQÅ[ÉW
// 
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Gauge.h"
#include "UserInterface.h"

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

kHorikawa::Gauge::Gauge()
    : m_menuIndex(0)
    ,m_windowHeight(0)
    ,m_windowWidth(0)
    ,m_pDR(nullptr)
    ,m_baseTexturePath(nullptr)
    ,m_gauge(nullptr)
    ,m_frame(nullptr)
{
}

kHorikawa::Gauge::~Gauge()
{
}

void kHorikawa::Gauge::Initialize(DX::DeviceResources* pDR,int width,int height)
{
	m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    m_baseTexturePath = L"Resources/Textures/frame_back.png";


    Add(L"Resources/Textures/frame.png"
        , SimpleMath::Vector2(0, 500)
        , SimpleMath::Vector2(1.0f,1.0f)
        , kHorikawa::ANCHOR::MIDDLE_LEFT);

}

void kHorikawa::Gauge::Update()
{

    auto keystate = Keyboard::Get().GetState();
    m_tracker.Update(keystate);

    float ratio = m_gauge->GetRenderRatio();

    if (m_tracker.pressed.Right)
    {
        ratio += 0.1f;
        ratio = std::min(1.0f, ratio);
    }
    if (m_tracker.pressed.Left)
    {
        ratio -= 0.1f;
        ratio = std::max(0.0f, ratio);
    }

    m_gauge->SetRenderRatio(ratio);

}

void kHorikawa::Gauge::Render()
{
    m_base->Render();
    m_gauge->Render();
    m_frame->Render();
}

void kHorikawa::Gauge::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kHorikawa::ANCHOR anchor)
{
    m_base = std::make_unique<kHorikawa::UserInterface>();
    m_base->Create(m_pDR
        , L"Resources/Textures/frame_base.png"
        , position
        , scale
        , anchor);
    m_base->SetWindowSize(m_windowWidth, m_windowHeight);


    m_gauge = std::make_unique<kHorikawa::UserInterface>();
    m_gauge->Create(m_pDR
        , m_baseTexturePath
        , position
        , scale
        , anchor);
    m_gauge->SetWindowSize(m_windowWidth, m_windowHeight);
    m_gauge->SetRenderRatioOffset(0.3f);

    m_frame = std::make_unique<kHorikawa::UserInterface>();
    m_frame->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    m_frame->SetWindowSize(m_windowWidth, m_windowHeight);


}



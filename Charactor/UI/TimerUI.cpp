#include "pch.h"
#include "Charactor/UI/TimerUI.h"


using namespace DirectX;

TimerUI::TimerUI()
    : m_menuIndex(0)
    , m_windowHeight(0)
    , m_windowWidth(0)
    , m_pDR(nullptr)
    , m_baseTexturePath(nullptr)
    , m_gauge(nullptr)
    , m_frame(nullptr)
{
}

TimerUI::~TimerUI()
{
}

void TimerUI::Initialize(DX::DeviceResources* pDR, int width, int height)
{
    m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    m_baseTexturePath = L"Resources/Textures/full.png";

    Add(L"Resources/Textures/waku.png"
        , SimpleMath::Vector2(0, 75)
        , SimpleMath::Vector2(0.7f, 0.7f)
        , kHorikawa::ANCHOR::MIDDLE_LEFT);

    m_gauge->SetRenderRatio(2.0f);
}

void TimerUI::Update(float dt)
{
    if (!m_isRunning) return;

    float ratio = m_gauge->GetRenderRatio();
    ratio -= (1.0f / 45.0f) * dt;
    if (ratio < 1.0f) ratio = 1.0f;
    if (ratio > 2.0f) ratio = 2.0f;
    m_gauge->SetRenderRatio(ratio);
}

void TimerUI::Render()
{
    m_base->Render();
    m_gauge->Render();
    m_frame->Render();
}

void TimerUI::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kHorikawa::ANCHOR anchor)
{
    m_base = std::make_unique<kHorikawa::UserInterface>();
    m_base->Create(m_pDR
        , L"Resources/Textures/lost.png"
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
    m_gauge->SetRenderRatioOffset(1.0f);

    m_frame = std::make_unique<kHorikawa::UserInterface>();
    m_frame->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    m_frame->SetWindowSize(m_windowWidth, m_windowHeight);
}

float TimerUI::GetRemainingTime() const
{
    float ratio = m_gauge->GetRenderRatio();

    float normalized = ratio - 1.0f;
    return normalized * 45.0f;
}
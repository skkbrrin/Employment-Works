#pragma once
#include "StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
#include <DeviceResources.h>

struct TimerCB
{
    float ratio;
    DirectX::SimpleMath::Vector3 padding; // 16byte境界
};


class TimerUI
{
public:
    void Initialize(ID3D11Device* device);
    void Update(float dt);
    void Render(DirectX::SpriteBatch* spriteBatch);

    void Start(float time);
    bool IsTimeUp() const { return m_timer <= 0.0f; }

private:
    float m_timer = 0.0f;
    float m_maxTime = 1.0f;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

    // シェーダ関連
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
};

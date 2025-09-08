//--------------------------------------------------------------------------------------
// File: HPManager.h
//-------------------------------------------------------------------------------------

#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>


class HPManager {
private:
    wchar_t m_texture; // 張り付けるテクスチャファイル
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backSRV; // 枠
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fillSRV; // 中身
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    ID3D11DeviceContext* m_context;
    ID3D11Device* m_device;

    int m_HP;
    DirectX::XMVECTORF32 m_color;

    float m_fillW = 256;
    float m_fillH = 64;

public:
    HPManager(ID3D11Device1* device, ID3D11DeviceContext* context);
    ~HPManager();

    bool Load(const wchar_t* back, const wchar_t* fill);

    void Update(int HP, int maxHP);

    void Render(DirectX::SimpleMath::Vector2 pos, 
                float scale = 1.0f);

    void Finalize();
};

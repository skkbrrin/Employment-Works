//--------------------------------------------------------------------------------------
// File: TextureAlpha.h
//
// プリミティブによる地面の描画
//
//-------------------------------------------------------------------------------------

#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>


class TextureAlpha {
private:
    wchar_t m_texture; // 張り付けるテクスチャファイル
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    ID3D11DeviceContext* m_context;
    ID3D11Device* m_device;


    float m_timer;
    DirectX::XMVECTORF32 m_color;

public:
    TextureAlpha(ID3D11Device1* device, ID3D11DeviceContext* context);
    ~TextureAlpha();

    bool Load(const wchar_t* texture);

    void Update(float elapsedTime);

    void Render(DirectX::SimpleMath::Vector2 pos, 
                float scale = 1.0f);

    void Finalize();
};

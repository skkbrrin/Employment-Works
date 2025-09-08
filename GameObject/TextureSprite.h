//--------------------------------------------------------------------------------------
// File: TextureSprite.h
//
// プリミティブによる地面の描画
//
//-------------------------------------------------------------------------------------

#pragma once
#include <PrimitiveBatch.h>
#include <VertexTypes.h>


class TextureSprite {
private:
    wchar_t m_texture; // 張り付けるテクスチャファイル
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    ID3D11DeviceContext* m_context;
    ID3D11Device* m_device;

public:
    TextureSprite(ID3D11Device1* device, ID3D11DeviceContext* context);
    ~TextureSprite();

    bool Load(const wchar_t* texture);

    void Render(DirectX::SimpleMath::Vector2 pos, 
                float scale = 1.0f,
                DirectX::FXMVECTOR color = DirectX::Colors::White);

    void Finalize();
};

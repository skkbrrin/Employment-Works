#pragma once
#include <unordered_map>
#include <string>
#include <wrl/client.h>
#include <d3d11.h>

class TextureManager
{
public:
    static TextureManager& Instance()
    {
        static TextureManager instance;
        return instance;
    }

    void Initialize(ID3D11Device* device)
    {
        m_device = device;
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
        Load(const std::wstring& path);

private:
    TextureManager() = default;

    ID3D11Device* m_device = nullptr;

    std::unordered_map<
        std::wstring,
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
    > m_textures;
};

/* Žg—p•û–@
auto tex = TextureManager::Instance().Load(
    L"Resources/Textures/Dog.png"
);
*/
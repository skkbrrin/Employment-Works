#include "pch.h"
#include "TextureManager.h"
#include <WICTextureLoader.h>

using Microsoft::WRL::ComPtr;

ComPtr<ID3D11ShaderResourceView>
TextureManager::Load(const std::wstring& path)
{
    auto it = m_textures.find(path);
    if (it != m_textures.end())
    {
        return it->second;
    }

    ComPtr<ID3D11ShaderResourceView> srv;
    DirectX::CreateWICTextureFromFile(
        m_device,
        path.c_str(),
        nullptr,
        srv.GetAddressOf()
    );

    m_textures[path] = srv;
    return srv;
}

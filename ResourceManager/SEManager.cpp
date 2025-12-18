#include "pch.h"
#include "SEManager.h"

std::shared_ptr<DirectX::SoundEffect>
SEManager::Load(const std::wstring& path)
{
    assert(m_audioEngine && "SEManager::Initialize が呼ばれていません");

    auto it = m_sounds.find(path);
    if (it != m_sounds.end())
    {
        return it->second;
    }

    auto sound = std::make_shared<DirectX::SoundEffect>(
        m_audioEngine,
        path.c_str()
    );

    m_sounds[path] = sound;
    return sound;
}

#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <Audio.h>

class SEManager
{
public:
    static SEManager& Instance()
    {
        static SEManager instance;
        return instance;
    }

    void Initialize(DirectX::AudioEngine* engine)
    {
        m_audioEngine = engine;
    }

    std::shared_ptr<DirectX::SoundEffect>
        Load(const std::wstring& path);

private:
    SEManager() = default;

    DirectX::AudioEngine* m_audioEngine = nullptr;

    std::unordered_map<
        std::wstring,
        std::shared_ptr<DirectX::SoundEffect>
    > m_sounds;
};


/* 使用方法
 auto se = SEManager::Instance().Load(
    L"Resources/SE/Attack.wav"
);

// 毎回新しいインスタンスで再生（重要）
auto instance = se->CreateInstance();
instance->Play();
*/
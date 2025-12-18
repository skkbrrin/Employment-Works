#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <Model.h>
#include <Effects.h>

class ModelManager
{
public:
    static ModelManager& Instance()
    {
        static ModelManager instance;
        return instance;
    }

    void Initialize(ID3D11Device* device)
    {
        m_device = device;
        m_fx = std::make_unique<DirectX::EffectFactory>(device);
        m_fx->SetDirectory(L"Resources/Models");
    }

    std::shared_ptr<DirectX::Model> Load(const std::wstring& path)
    {
        auto it = m_models.find(path);
        if (it != m_models.end())
        {
            return it->second;
        }

        auto uniqueModel = DirectX::Model::CreateFromSDKMESH(
            m_device,
            path.c_str(),
            *m_fx
        );

        std::shared_ptr<DirectX::Model> model = std::move(uniqueModel);

        m_models[path] = model;
        return model;
    }

private:
    ModelManager() = default;

    ID3D11Device* m_device = nullptr;
    std::unique_ptr<DirectX::EffectFactory> m_fx;
    std::unordered_map<std::wstring, std::shared_ptr<DirectX::Model>> m_models;
};

/* Žg—p•û–@
auto model = ModelManager::Instance().Load(
    L"Resources/Models/DogBody.sdkmesh"
);
*/
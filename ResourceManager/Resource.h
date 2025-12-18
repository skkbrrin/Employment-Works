#include "ModelManager.h"
#include "SEManager.h"
#include "TextureManager.h"

class Resource
{
public:
    static void Initialize(ID3D11Device* device)
    {
        Model().Initialize(device);
        Texture().Initialize(device);
    }

    static ModelManager& Model()
    {
        return ModelManager::Instance();
    }
    static TextureManager& Texture()
    {
        return TextureManager::Instance();
    }
    static SEManager& Sound()
    {
        return SEManager::Instance();
    }
};

/* 使用方法
Resource::Model().Load(L"Resources/Models/・・・.sdkmesh");
Resource::Texture().Load(L"Resources/Textures/・・・.png");
Resource::Sound().Load(L"Resources/Sounds/・・・.wav"); 
....
*/
#include "pch.h"
#include "Item.h"
#include <Model.h>
#include <Effects.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::unique_ptr<Model> Item::m_woodModel = nullptr;

void Item::LoadModels(ID3D11Device* device)
{
    std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
    fx->SetDirectory(L"Resources/Models");

    m_woodModel = Model::CreateFromSDKMESH(device, L"Resources/Models/Wood.sdkmesh", *fx);
}

Item::Item(Type type, DirectX::SimpleMath::Matrix world)
    : m_type(type)
{
    SetWorld(world);
}

void Item::Update(float dt)
{
}

void Item::Render(ID3D11DeviceContext* context, CommonStates* states, Matrix view, Matrix proj)
{
    if (m_type == Type::Wood)
    {
        if (m_woodModel)
            m_woodModel->Draw(context, *states, m_world, view, proj);
        return;
    }
}

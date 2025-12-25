#include "pch.h"
#include "Item.h"
#include <Model.h>
#include <Effects.h>
#include "ResourceManager/Resource.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

std::shared_ptr<DirectX::DX11::Model> Item::m_woodModel = nullptr;

void Item::LoadModels(ID3D11Device* device)
{
    m_woodModel = Resource::Model().Load(L"Resources/Models/Wood.sdkmesh");
}

Item::Item(Type type, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& velocity)
{
    m_type = type;     // ← これがないと Wood と認識されない
    m_position = position;
    m_velocity = velocity;
    pickedUp = false;

    m_world = Matrix::CreateTranslation(position);
}

void Item::Update(float dt)
{
    if (!pickedUp)
    {
        // 重力
        const float gravity = -9.8f;
        m_velocity.y += gravity * dt;

        // 速度で移動
        m_position += m_velocity * dt;

        // 地面に落ちたら停止（簡易処理）
        if (m_position.y < 0.0f)
        {
            m_position.y = 0.0f;
            m_velocity = Vector3::Zero;
        }
    }

    m_world = Matrix::CreateTranslation(m_position);
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

#pragma once
#include <SimpleMath.h>
#include <CommonStates.h>
#include "Model.h"
#include "Charactor/UI/ShadowSprite.h"

class Item
{
public:
    enum class Type {
        Wood,
    };

    Item(Type type,
        const DirectX::SimpleMath::Vector3& position,
        const DirectX::SimpleMath::Vector3& velocity
        );
    
    static void LoadModels(ID3D11Device* device);
    
    void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    void Update(float dt);
    void Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj);

    bool IsPickedUp() const { return pickedUp; }
    void SetPickedUp(bool p) { pickedUp = p; }

    Type GetType() const { return m_type; }
    const DirectX::SimpleMath::Matrix& GetWorldMatrix() const { return m_world; }
    void SetWorld(DirectX::SimpleMath::Matrix& world) { m_world = world; }
    void SetPosition(const DirectX::SimpleMath::Vector3& pos) { m_position = pos; }
    void SetVelocity(const DirectX::SimpleMath::Vector3& vel) { m_velocity = vel; }

private:
    Type m_type;
    static std::shared_ptr<DirectX::Model> m_woodModel;
    DirectX::SimpleMath::Matrix m_world;
    DirectX::SimpleMath::Vector3 m_position;
    DirectX::SimpleMath::Vector3 m_velocity;
    float   m_time = 0.0f;
    bool pickedUp = false;

    std::unique_ptr<ShadowSprite> m_shadow;
};
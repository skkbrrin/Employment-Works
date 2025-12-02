#pragma once
#include <SimpleMath.h>
#include <CommonStates.h>
#include "Model.h"

class Item
{
public:
    enum class Type {
        Wood,
    };

    Item(Type type, const DirectX::SimpleMath::Matrix& world);
    
    static void LoadModels(ID3D11Device* device);
    
    void Update(float dt);
    void Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj);

    bool IsPickedUp() const { return pickedUp; }
    void SetPickedUp(bool p) { pickedUp = p; }

    Type GetType() const { return m_type; }
    const DirectX::SimpleMath::Matrix& GetWorldMatrix() const { return m_world; }

private:
    Type m_type;
    static std::unique_ptr<DirectX::Model> m_woodModel;
    DirectX::SimpleMath::Matrix m_world;
    bool pickedUp = false;
};
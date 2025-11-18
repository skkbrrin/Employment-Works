#pragma once
#include <SimpleMath.h>
#include <Model.h>
#include <vector>
#include <memory>
#include <string>

class TransformNode
{
public:
    TransformNode(const std::wstring& name) : m_name(name) {}

    DirectX::SimpleMath::Matrix GetLocalMatrix() const
    {
        return DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
            DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation) *
            DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
    }

    void AddChild(std::unique_ptr<TransformNode> child)
    {
        child->m_parent = this;
        m_children.push_back(std::move(child));
    }

    void SetModel(std::unique_ptr<DirectX::Model> model)
    {
        m_model = std::move(model);
    }


    DirectX::SimpleMath::Matrix GetWorldMatrix() const { return m_world; }

    void UpdateWorldMatrix(const DirectX::SimpleMath::Matrix& parentMat = DirectX::SimpleMath::Matrix::Identity)
    {
        using namespace DirectX::SimpleMath;

        Matrix local =
            Matrix::CreateScale(m_scale) *
            Matrix::CreateFromQuaternion(m_rotation) *
            Matrix::CreateTranslation(m_position);

        m_world = local * parentMat;

        for (auto& c : m_children)
            c->UpdateWorldMatrix(m_world);
    }

    void Render(ID3D11DeviceContext* context,
        DirectX::CommonStates* states,
        const DirectX::SimpleMath::Matrix& view,
        const DirectX::SimpleMath::Matrix& proj)
    {
        using namespace DirectX::SimpleMath;
        Matrix world = GetWorldMatrix();
        if (m_model)
            m_model->Draw(context, *states, world, view, proj);

        for (auto& c : m_children)
            c->Render(context, states, view, proj);
    }

    DirectX::SimpleMath::Vector3 m_position = { 0, 0, 0 };
    DirectX::SimpleMath::Quaternion m_rotation = DirectX::SimpleMath::Quaternion::Identity;
    DirectX::SimpleMath::Vector3 m_scale = { 1, 1, 1 };

    std::wstring m_name;
    TransformNode* m_parent = nullptr;
    std::vector<std::unique_ptr<TransformNode>> m_children;

private:
    std::unique_ptr<DirectX::Model> m_model;
    DirectX::SimpleMath::Matrix m_world = DirectX::SimpleMath::Matrix::Identity;

};

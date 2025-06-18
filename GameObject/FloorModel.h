//--------------------------------------------------------------------------------------
// File: FloorModel.h
//
// モデルによる地面の描画
//
//-------------------------------------------------------------------------------------

#pragma once
#include <Model.h>

namespace kHorikawa
{
    class FloorModel
    {
    private :

        // 共通ステートオブジェクトへのポインタ
        std::unique_ptr<DirectX::CommonStates> m_states;

        // エフェクトファクトリー
        std::unique_ptr<DirectX::EffectFactory> m_EffectFactory;

        // 床のモデル
        std::unique_ptr<DirectX::Model> m_floorModel;
    public:
        FloorModel(ID3D11Device1* device, wchar_t const* fileName = nullptr);
        ~FloorModel();

        void Render(ID3D11DeviceContext1* context, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
        void Finalize();
    };

}

//-------------------------------------------------------------------------------------
// File: FloorModel.cpp
//
// render floor by model
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "FloorModel.h"


using namespace DirectX;

/// <summary>
/// Constractor
/// </summary>
kHorikawa::FloorModel::FloorModel(ID3D11Device1* device, wchar_t const* fileName)
{   
    // 共通ステートオブジェクトの作成
    m_states = std::make_unique<CommonStates>(device);

    // 床モデルの読み込み
    m_EffectFactory = std::make_unique<EffectFactory>(device);
    m_EffectFactory->SetDirectory(L"Resources/Models");
    m_floorModel = Model::CreateFromSDKMESH(device, fileName, *m_EffectFactory.get());
}

/// <summary>
/// Destractor
/// </summary>
kHorikawa::FloorModel::~FloorModel()
{

}

/// <summary>
/// Render 
/// </summary>
void kHorikawa::FloorModel::Render(ID3D11DeviceContext1* context,SimpleMath::Matrix view, SimpleMath::Matrix proj )
{
    SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

    world = DirectX::SimpleMath::Matrix::CreateRotationX(90.0f / 180.0f * 3.14f) * DirectX::SimpleMath::Matrix::CreateScale(10.0f) * DirectX::SimpleMath::Matrix::CreateTranslation(0.0f ,-10.0f, 0.0f);

    // 床の描画
    m_floorModel->Draw(context, *m_states.get(), world, view, proj, false, [&]()
        {
            ID3D11SamplerState* samplers[] = { m_states->PointWrap() };
            context->PSSetSamplers(0, 1, samplers);
        }
    );
}
/// <summary>
/// Finalize 
/// </summary>
void kHorikawa::FloorModel::Finalize()
{
    // シーンのクリーンアップ
    m_EffectFactory.reset();
    m_floorModel.reset();
}

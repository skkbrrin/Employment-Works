//--------------------------------------------------------------------------------------
// File: Menu.cpp
//
// メニュー
//
//-------------------------------------------------------------------------------------

#include "pch.h"
#include "Menu.h"
#include "UserInterface.h"

#include "GameObject/MyBinalyFile.h"
#include "DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>

kHorikawa::Menu::Menu() 
    : m_menuIndex(0)
    , m_windowHeight(0)
    , m_windowWidth(0)
    , m_pDR(nullptr)
    , m_baseTexturePath(nullptr)
{
    m_userInterface.clear();
}

kHorikawa::Menu::~Menu()
{
}

void kHorikawa::Menu::Initialize(DX::DeviceResources* pDR,int width,int height)
{
	m_pDR = pDR;
    m_windowWidth = width;
    m_windowHeight = height;

    //  背景となるウィンドウ画像を読み込む
    m_baseTexturePath = L"Resources/Textures/window.png";

    ////  草画像を読み込む
    //Add(L"Resources/Textures/grass.png"
    //    , DirectX::SimpleMath::Vector2(100, 100)
    //    , DirectX::SimpleMath::Vector2(0.5f,0.5f)
    //    , kHorikawa::ANCHOR::MIDDLE_CENTER);

}

void kHorikawa::Menu::Update()
{
    DirectX::Keyboard::State keystate = DirectX::Keyboard::Get().GetState();
    m_tracker.Update(keystate);
    if (m_tracker.pressed.Right)
    {
        //  →キーを押したら、選択先を1つ進める
        m_menuIndex += 1;
        //  メニューアイテム数の最大値を超えないように制御
        m_menuIndex %= m_userInterface.size();
    }
    if (m_tracker.pressed.Left)
    {
        //  ←キーを押したら、選択先を1つ戻す。
        //  ただし、選択先のオーバーを割り算の余りで補正するため、（アイテムの最大個数 - 1）を足して必ず余りで計算する形にしておく。
        //  以下の式は、例えばメニューが4つあったら、現在値に3を足すという事。
        //  例）選択中のメニューが0～3まである内の2番目だった場合、
        //  (2 + (4 - 1)) % 4 = 1 ← 選択中の番号が1つ減った
        //  ということ
        m_menuIndex += static_cast<unsigned int>(m_userInterface.size()) - 1;
        m_menuIndex %= m_userInterface.size();
    }

    //  各アイテムに表示する画像の初期サイズを設定する
    for (int i = 0; i < m_userInterface.size(); i++)
    {
        m_base[i]->SetScale(m_base[i]->GetBaseScale());
        m_userInterface[i]->SetScale(m_userInterface[i]->GetBaseScale());
    }

    //  選択中の初期サイズを取得する
    DirectX::SimpleMath::Vector2 select = m_userInterface[m_menuIndex]->GetBaseScale();
    //  選択状態とするための変化用サイズを算出する
    DirectX::SimpleMath::Vector2 selectScale = DirectX::SimpleMath::Vector2::Lerp(m_userInterface[m_menuIndex]->GetBaseScale(), DirectX::SimpleMath::Vector2::One, 1);
    //  選択状態は初期状態＋30％の大きさとする
    select += selectScale * 0.3f;
    //  算出後のサイズを現在のサイズとして設定する
    m_userInterface[m_menuIndex]->SetScale(select);
    //  背景用のウィンドウ画像にも同じ割合の値を設定する
    m_base[m_menuIndex]->SetScale(select);
}

void kHorikawa::Menu::Render()
{
    for (int i = 0;i < m_userInterface.size();i++)
    {
        //  アイテム用ウィンドウ背景を表示
        m_base[i]->Render();
        //  実際に表示したいアイテム画像を表示
        m_userInterface[i]->Render();
    }
}

void kHorikawa::Menu::Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, kHorikawa::ANCHOR anchor)
{
    //  メニューとしてアイテムを追加する
    std::unique_ptr<kHorikawa::UserInterface> userInterface = std::make_unique<kHorikawa::UserInterface>();
    //  指定された画像を表示するためのアイテムを作成する
    userInterface->Create(m_pDR
        , path
        , position
        , scale
        , anchor);
    userInterface->SetWindowSize(m_windowWidth, m_windowHeight);

    //  アイテムを新しく追加
    m_userInterface.push_back(std::move(userInterface));

    //  背景用のウィンドウ画像も追加する
    std::unique_ptr<kHorikawa::UserInterface> base = std::make_unique<kHorikawa::UserInterface>();
    base->Create(m_pDR
        , m_baseTexturePath
        , position
        , scale
        , anchor);
    base->SetWindowSize(m_windowWidth, m_windowHeight);

    //  背景用のアイテムも新しく追加する
    m_base.push_back(std::move(base));

}



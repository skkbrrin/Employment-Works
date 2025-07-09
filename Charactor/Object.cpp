#include "pch.h"
#include "Object.h"
#include "TanakaLib/Message.h"

// オブジェクト番号
int Object::s_objectNumber = -1;

// オブジェクトをカウントアップする
int Object::CountUpNumber() { return s_objectNumber++; }

// コンストラクタ
Object::Object(const float& maxTurnRate, const float& maxSpeed, const float& maxForce, const float& mass, const float& scale)
	:
	m_currentState{},											// 現在の状態
	m_currentMessage{},									// 現在のメッセージ
	m_maxTurnRate(maxTurnRate),			// 回転するときの最大速度
	m_maxSpeed(maxSpeed),						// 最大移動速度
	m_maxForce(maxForce),							// 最大力
	m_mass(mass),												// 質量
	m_scale(scale)												// スケール
{
}

// デストラクタ
Object::~Object()
{
}

// 初期化する
void Object::Initialize()
{ 
}

// 更新する
void Object::Update(const float& elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);
}

// 描画する
void Object::Render()
{
}

// 後処理を行う
void Object::Finalize()
{
}
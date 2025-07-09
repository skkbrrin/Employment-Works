#pragma once
#ifndef OBJECT_DEFINED
#define OBJECT_DEFINED
#include "TanakaLib/IObject.h"
#include "TanakaLib/IState.h"

// Objectクラス
class Object : public IObject
{
public:
	// オブジェクトをカウントアップする
	static int CountUpNumber();
	// 状態を取得する
	IState* GetState() { return m_currentState; }
	// 状態を設定する
	void SetState(IState* state) { m_currentState = state; }
	// 現在の状態を変更する
	void ChangeState(IState* state)
	{
		// 新規の状態遷移前に事後更新を行う
		m_currentState->PostUpdate();
		// 新規の状態を現在の状態に設定する
		m_currentState = state;
		// 新規の状態遷移後に事前更新を行う
		m_currentState->PreUpdate();
	}
	// メッセージを取得する
	Message GetCurrentMessage() const { return m_currentMessage; }
	// メッセージを設定する
	void SetCurrentMessage(Message currentMessage) { m_currentMessage = currentMessage; }

public:
	// 回転するときの最大速度
	float GetMaxTurnRate() const { return m_maxTurnRate; }
	// 最大移動速度
	float GetMaxSpeed() const { return m_maxSpeed; }
	// 最大力
	float GetMaxForce() const { return m_maxForce; }
	// 質量
	float GetMass() const { 	return m_mass; }
	// スケール
	float GetScale() const { return m_scale; }

public:
	// コンストラクタ
	Object(const float& maxTurnRate, const float& maxSpeed, const float& maxForce, const float& mass, const float& scale);
	// デストラクタ
	virtual ~Object();
	// 初期化する
	virtual void Initialize();
	// 更新する
	virtual void Update(const float& elapsedTime);
	// 描画する
	virtual void Render();
	// 後処理を行う
	virtual void Finalize();

private:
	// オブジェクト番号
	static int s_objectNumber;
	// 現在の状態
	IState* m_currentState;
	// 現在のメッセージ
	Message m_currentMessage;
	// 回転するときの最大速度
	float m_maxTurnRate;
	// 最大移動速度
	float m_maxSpeed;
	// 最大力
	float m_maxForce;
	// プレイヤーの質量
	float m_mass;
	// スケール
	float m_scale;
};

#endif		// OBJECT_DEFINED

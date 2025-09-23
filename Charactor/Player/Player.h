#pragma once
#include "ItoLib/GameCamera.h"

class Enemy;

class Player
{
public:
	// 通常関数
	Player();
	~Player();
	void Initialize(ID3D11Device* device);
	void Update(float elapsedTime, Enemy* enemy);
	void Render(
		ID3D11DeviceContext* context, DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	// 攻撃用
	void NormalAttack(float elapsedTime, const std::vector<std::unique_ptr<Enemy>>& enemies); //< 単体攻撃
	void ComboAttack(float elapsedTime, const std::vector<std::unique_ptr<Enemy>>& enemies); //< コンボ攻撃

private:
	std::unique_ptr<DirectX::Model> m_player; // モデル
	DirectX::SimpleMath::Vector3 m_position; // 座標
	DirectX::SimpleMath::Quaternion m_rotate; // 回転

	DirectX::Keyboard::KeyboardStateTracker m_tracker; // キーボードトラッカー

	int m_HP; // HP
	int m_fullHP; // 最大HP
	int m_attck; // 攻撃力

	bool m_isAttacking = false;// 攻撃しているか
	float m_attackTime = 0.0f; // 経過時間
	float m_attackDuration = 0.5f;

	DirectX::SimpleMath::Vector3 m_attackStartPos; // 攻撃開始時座標
	DirectX::SimpleMath::Vector3 m_attackTargetPos; // 攻撃終了時座標
	Enemy* m_attackTarget = nullptr; // ターゲット

// 取得　設定
public:
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_position; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::SimpleMath::Quaternion& GetPlayerRotate() { return m_rotate; }
	void SetRotate(DirectX::SimpleMath::Quaternion rotate) { m_rotate = rotate; }
	int GetHP() { return m_HP; }
	int GetFullHP() { return m_fullHP; }
	void SetHP(int hp) { m_HP = hp; }
	void SubHP(int damage) { m_HP -= damage; }

	bool GetAttacking() { return m_isAttacking; }
	void SetAttacking(bool isAttack) { m_isAttacking = isAttack; }

	Enemy* GetAttackTarget() { return m_attackTarget; }

	void Damage(int damage) 
	{
		m_HP -= damage; 
		if (m_HP <= 0)
		{
			m_HP = 0;
		}
	}
};

#pragma once
#include "ItoLib/GameCamera.h"

enum class State
{
	Chase,// 追跡ステート
	Roll, // 回転ステート（攻撃実装したら攻撃に変える）
};

class Enemy
{
public:
	Enemy();
	~Enemy();
	void Inisialize(ID3D11Device* device);
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3 playerPos);
	void Render( ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

public:
	void Attack();
	void Roll(float elapsedTime);
	void Chase(float elapsedTime, const DirectX::SimpleMath::Vector3 playerPos);

private:
	std::unique_ptr<DirectX::Model> m_enemyModel;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotate;
	bool rigth = false;

	float m_speed;

	State m_state = State::Chase; // ステート
	float m_detectionRange = 3.0f; // 攻撃可能距離
	float m_distance;

	int m_HP;
	int m_attack;

public:
	bool Getrigth() { return rigth; }
	DirectX::SimpleMath::Vector3 GetPos() { return m_position; }
	float GetDistance(){ return m_distance; }
	int GetHP() { return m_HP; }
	void SetHP(int hp) { m_HP = hp; }
	void SubHP(int damage) { m_HP -= damage; }
};


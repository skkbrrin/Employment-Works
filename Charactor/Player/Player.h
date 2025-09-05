#pragma once
#include "ItoLib/GameCamera.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(ID3D11Device* device);
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3 enemyPos);
	void Render(
		ID3D11DeviceContext* context, DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	std::unique_ptr<DirectX::Model> m_player;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotate;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	int m_HP;
	int m_attck;


public:
	DirectX::SimpleMath::Vector3 GetPlayerPosition() { return m_position; }
	void SetPosition(DirectX::SimpleMath::Vector3 pos) { m_position = pos; }
	DirectX::SimpleMath::Quaternion GetPlayerRotate() { return m_rotate; }
	void SetRotate(DirectX::SimpleMath::Quaternion rotate) { m_rotate = rotate; }
	int GetHP() { return m_HP; }
	void SetHP(int hp) { m_HP = hp; }
	void SubHP(int damage) { m_HP -= damage; }
};

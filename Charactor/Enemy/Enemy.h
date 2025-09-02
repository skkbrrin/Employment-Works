#pragma once
#include "ItoLib/GameCamera.h"

class Enemy
{
public:
	Enemy();
	~Enemy();
	void Inisialize(ID3D11Device* device);
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3 playerPos);
	void Render( ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	std::unique_ptr<DirectX::Model> m_enemyModel;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotate;
	bool rigth = false;

	float m_speed;

public:
	bool Getrigth() { return rigth; }
	float GetPos() { return m_position.x; }
};


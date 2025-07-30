#pragma once
#include "ItoLib/GameCamera.h"

class Player
{
public:
	Player();
	~Player();
	void Initialize(ID3D11Device* device);
	void Update(float elapsedTime);
	void Render(
		ID3D11DeviceContext* context, DirectX::CommonStates* states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	std::unique_ptr<DirectX::Model> m_player;
	DirectX::SimpleMath::Vector3 m_position;
	DirectX::SimpleMath::Quaternion m_rotate;

	bool m_cameraDebug = false;
	bool m_cameraGame = false;

	bool act = false;
	bool gameC = false;

	GameCamera m_camera;

public:
	DirectX::SimpleMath::Vector3 GetPlayerPosition();
	DirectX::SimpleMath::Quaternion GetPlayerRotate();
};

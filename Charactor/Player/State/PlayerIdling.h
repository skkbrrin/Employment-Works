#pragma once
#ifndef PLAYER_IDLING
#define PLAYER_IDLING

#include "TanakaLib/IState.h"
#include "TanakaLib/Graphics.h"
#include "TanakaLib/Message.h"

class Player;

class PlayerIdling : public IState
{
public:
	PlayerIdling(Player* player);
	~PlayerIdling();

	void Initialize();
	void OnMessege(Message::MessageID messageID);
	void PreUpdate();
	void Update(const float& elapsedTime);
	void PostUpdate();
	void Render();
	void Finalize();
	void ResetPlayer();

private:
	Player* m_player;
	Graphics* m_graphics;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_context;
	DX::DeviceResources* m_deviceResources;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::Model* m_playerModel;
};
#endif // !PLAYER_IDLING


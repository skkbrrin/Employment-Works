#pragma once
#ifndef PLAYER_IDLING
#define PLAYER_IDLING

#include "TanakaLib/IState.h"
#include "Scene/TutorialScene.h"
#include "Charactor/Player/Player.h"
#include "TanakaLib/Graphics.h"

class PlayerIdling : public IState
{
public:
	PlayerIdling();
	~PlayerIdling();
	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
};
#endif // !PLAYER_IDLING


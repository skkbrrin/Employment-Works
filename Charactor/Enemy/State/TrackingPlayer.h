#pragma once
#ifndef TRACKING_PLAYER
#define TRACKING_PLAYER

#include "TanakaLib/IState.h"
#include "Scene/TutorialScene.h"
#include "Charactor/Enemy/Enemy.h"
#include "TanakaLib/Graphics.h"

class TrackingPlayer : public IState
{
	TrackingPlayer();
	~TrackingPlayer();
	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
};

#endif // TRACKING_PLAYER
#pragma once
#ifndef ENEMY_IDLING
#define ENEMY_IDLING

#include "TanakaLib/IState.h"
#include "Scene/TutorialScene.h"
#include "Charactor/Enemy/Enemy.h"
#include "TanakaLib/Graphics.h"

class EnemyIdling : public IState
{
public:
	EnemyIdling();
	~EnemyIdling();
	void Initialize();
	void Update(const float& elapsedTime);
	void Render();
	void Finalize();
};
#endif // !ENEMY_IDLING


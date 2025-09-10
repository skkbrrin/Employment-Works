#include "pch.h"
#include "pch.h"
#include "ScoreManager.h"

ScoreManager::ScoreManager()

{
}

ScoreManager::~ScoreManager()
{
}

void ScoreManager::Initialize()
{
	m_attackScore = 0;
	m_timeScore = 0;
	m_totalScore = 0;
}

void ScoreManager::Update()
{
	AddTotalScore(GetAttackScore() + GetTimeScore());
}

void ScoreManager::Finalize()
{
}

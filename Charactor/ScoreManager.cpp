#include "pch.h"
#include "ScoreManager.h"

ScoreManager::ScoreManager()
{
    m_attackScore = 0;
    m_timeScore = 0;
    m_totalScore = 0;
}

void ScoreManager::Reset()
{
    m_attackScore = 0;
    m_timeScore = 0;
    m_totalScore = 0;
}


#pragma once
class ScoreManager
{
private:
	int m_attackScore;
	int m_timeScore;
	int m_totalScore;

public:
	ScoreManager();
	~ScoreManager();
	void Initialize();
	void Update();
	void Finalize();

public:
	int GetAttackScore() { return m_attackScore; }
	int GetTimeScore() { return m_timeScore; }
	int GetTotalScore() { return m_totalScore; }

	void AddAttackScore(int scoreA) { m_attackScore += scoreA; }
	void AddTimeScore(int scoreTime) { m_timeScore += scoreTime; }
	void AddTotalScore(int scoreTotal) { m_totalScore += scoreTotal; }
};


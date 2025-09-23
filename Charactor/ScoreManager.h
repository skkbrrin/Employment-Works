#pragma once

class ScoreManager
{
private:
    int m_attackScore;
    int m_timeScore;
    int m_totalScore;

    ScoreManager(); // private
public:
    static ScoreManager& Instance()
    {
        static ScoreManager instance;
        return instance;
    }

    void Reset();

    void Update()
    {
        m_totalScore = m_attackScore + m_timeScore;
    }


    // Getter
    int GetAttackScore() const { return m_attackScore; }
    int GetTimeScore() const { return m_timeScore; }
    int GetTotalScore() const { return m_totalScore; }

    // Setter
    void SetAttackScore(int scoreA) { m_attackScore = scoreA; }
    void SetTimeScore(int scoreTime) { m_timeScore = scoreTime; }
};

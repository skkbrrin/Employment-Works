#pragma once

class WoodManager
{
public:
    static WoodManager& Instance()
    {
        static WoodManager instance;
        return instance;
    }

    void Reset()
    {
        m_woodCount = 0;
    }

    void Add(int v)
    {
        m_woodCount += v;
    }

    int Get() const
    {
        return m_woodCount;
    }

    void Set(int zero)
    {
        m_woodCount = zero;
    }

private:
    WoodManager() = default;
    int m_woodCount = 0;
};

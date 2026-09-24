#pragma once

class WoodManager
{
public:
    static WoodManager& Instance()
    {
        static WoodManager instance;
        return instance;
    }

    // 取得数を戻す
    void Reset()
    {
        m_woodCount = 0;
    }

    // 追加
    void Add(int v)
    {
        m_woodCount += v;
    }

    // 取得
    int Get() const
    {
        return m_woodCount;
    }

    // 設定
    void Set(int zero)
    {
        m_woodCount = zero;
    }
     
    // クリア条件個数
    void SetClearCount(int num)
    {
        m_clearcount = num;
    }

    int GetClearCount()
    {
        return m_clearcount;
    }

    // クリアに必要分集めたか
    bool IsComplete() const
    {
        return m_woodCount >= m_clearcount;
    }

private:
    WoodManager() = default;
    int m_woodCount = 0;
    int m_clearcount = 30;
};

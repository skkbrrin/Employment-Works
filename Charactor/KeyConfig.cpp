#include "pch.h"
#include "KeyConfig.h"
#include <fstream>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

// デフォルトキー設定
void KeyConfig::DefaultKey()
{
    m_keyBindings[ActionID::Forward] = VK_UP;
    m_keyBindings[ActionID::Avoid] = VK_DOWN;
    m_keyBindings[ActionID::LeftRotation] = VK_LEFT;
    m_keyBindings[ActionID::RigthRotation] = VK_RIGHT;
}

// セッティング
void KeyConfig::SetKey(ActionID act, int key)
{
    m_keyBindings[act] = key;
}

// ゲッティング
int KeyConfig::GetKey(ActionID act) const
{
    auto it = m_keyBindings.find(act);
    return (it != m_keyBindings.end()) ? it->second : 0;
}

// キーが押されたら
bool KeyConfig::IsPressed(ActionID act) const
{
    int key = GetKey(act);
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}

// JSON ファイル保存
void KeyConfig::Save(const std::string& path)
{
    nlohmann::json j;

    // iterator ループで KeyConfig を JSON に変換
    for (auto it = m_keyBindings.begin(); it != m_keyBindings.end(); ++it) {
        int actInt = static_cast<int>(it->first);
        int key = it->second;
        j[std::to_string(actInt)] = key;  // C++14 では std::to_string がそのまま使える
    }

    // ofstream に string を直接渡せる
    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4); // インデント付きで保存
    }
    else {
        // ファイルオープン失敗時はエラー表示
        std::cerr << "Failed to open file for saving: " << path << std::endl;
    }
}

// JSON ファイル読み込み（安全版）
void KeyConfig::Load(const std::string& path)
{
    std::ifstream file(path.c_str()); // VS11対応
    if (!file.is_open()) {
        // ファイルがなければデフォルト設定で作る
        DefaultKey();
        Save(path);
        return;
    }

    json j;
    try {
        file >> j;
    }
    catch (json::parse_error& e) {
        // パース失敗時はデフォルト設定
        DefaultKey();
        Save(path);
        return;
    }

    m_keyBindings.clear();

    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        // 値が整数型か確認
        if (!it.value().is_number_integer()) continue;

        std::string keyStr = it.key();
        int val = it.value().get<int>();

        // キーID文字列を整数に変換
        try {
            int action = std::stoi(keyStr);
            m_keyBindings[static_cast<ActionID>(action)] = val;
        }
        catch (...) {
            continue;
        }
    }

    // 万一ファイルが空や不正で何も読み込めなかった場合はデフォルト設定
    if (m_keyBindings.empty()) {
        DefaultKey();
        Save(path);
    }
}

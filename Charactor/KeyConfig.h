#pragma once
#include <unordered_map>
#include <string>

// 行動種類
enum class ActionID
{
	Forward,// 前進
	Avoid,// 飛びのき回避
	LeftRotation,// 左回転
	RigthRotation,// 右回転
	NormalAttack,// 通常攻撃
	ComboAttack,// コンボ攻撃
	Pause,// ポーズ
};

class KeyConfig
{
public:
	KeyConfig() = default; // コンストラクタ
	~KeyConfig() = default; // デストラクタ

	void DefaultKey();
	
	void SetKey(ActionID act, int key);
	int GetKey(ActionID act) const;
	bool IsPressed(ActionID act) const; // プレイ中に押されたとき
	
	// JSONファイル用
	void Save(const std::string& path);
	void Load(const std::string& path);

private:
	std::unordered_map<ActionID, int> m_keyBindings; // キーバインド
};

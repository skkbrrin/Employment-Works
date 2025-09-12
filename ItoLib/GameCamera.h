#pragma once

#include "ItoLib//Camera.h"

class Player;

//ゲームカメラ
class GameCamera : public Ito::Camera
{
public:

	//カメラのタイプ
	enum class Type
	{
		Normal,
		Attack,
		Return
	};

private:

	//現在のカメラタイプ
	Type m_type;

	//回転角（ラジアン）
	float m_angle;

	DirectX::SimpleMath::Matrix m_view;

	// 補間用
	DirectX::SimpleMath::Vector3 m_eyeStart;
	DirectX::SimpleMath::Vector3 m_targetStart;
	DirectX::SimpleMath::Vector3 m_eyeEnd;
	DirectX::SimpleMath::Vector3 m_targetEnd;
	float m_lerpTime = 0.0f;
	float m_lerpDuration = 0.0f;

	DirectX::SimpleMath::Vector3 m_currentEye;
	DirectX::SimpleMath::Vector3 m_currentTarget;

	Player* m_player;

	// カットイン用
	bool m_showCutIn = false;
	float m_cutInTimer;

public:

	//コンストラクタ
	GameCamera();

	// 更新関数
	void Update(float elapsedTime);

	void ChangeMode(Type type);
	
	// プレイヤーの位置と回転を設定する関数
	void SetPlayer(Player* player) {
		m_player = player;
	}


	DirectX::SimpleMath::Vector3 GetEyeFromCurrentView() const { return m_currentEye; }
	DirectX::SimpleMath::Vector3 GetTargetFromCurrentView() const { return m_currentTarget; }
	Type GetType() { return m_type; }
	bool IsCutInActive() { return m_showCutIn; }
};
#pragma once

#include "ItoLib//Camera.h"

//ゲームカメラ
class GameCamera : public Ito::Camera
{
public:

	//カメラのタイプ
	enum class Type
	{
		Type_A,	//プレイヤーを追尾するカメラ
		Type_B	//プレイヤーの周りをまわるカメラ
	};

private:

	//現在のカメラタイプ
	Type m_type;

	//回転角（ラジアン）
	float m_angle;

	// プレイヤーの位置
	const DirectX::SimpleMath::Vector3* m_pPlayerPos;

	// プレイヤーの回転
	const DirectX::SimpleMath::Quaternion* m_pPlayerRotate;

public:

	//コンストラクタ
	GameCamera();

	// 更新関数
	void Update(float elapsedTime, int number);

	// プレイヤーの位置と回転を設定する関数
	void SetPlayer(
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate
	)
	{
		m_pPlayerPos = &position;
		m_pPlayerRotate = &rotate;
	}
};
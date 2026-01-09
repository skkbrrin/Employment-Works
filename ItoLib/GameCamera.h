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

	// プレイヤーからの相対オフセット
	DirectX::SimpleMath::Vector3 m_eyeOffset;
	DirectX::SimpleMath::Vector3 m_targetOffset;

	// デフォルトオフセット
	DirectX::SimpleMath::Vector3 m_defaultEyeOffset;
	DirectX::SimpleMath::Vector3 m_defaultTargetOffset;
public:

	//コンストラクタ
	GameCamera();

	// 更新関数
	void Update(float elapsedTime);

	// プレイヤーの位置と回転を設定する関数
	void SetPlayer(
		const DirectX::SimpleMath::Vector3* position,
		const DirectX::SimpleMath::Quaternion* rotate
	)
	{
		m_pPlayerPos = position;
		m_pPlayerRotate = rotate;
	}

	// カメラのオフセット設定
	void SetOffset( const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target )
	{
		m_eyeOffset = eye;
		m_targetOffset = target;
	}

	// リセット関数
	void Reset()
	{
		m_eyeOffset = m_defaultEyeOffset;
		m_targetOffset = m_defaultTargetOffset;
	}
};
#include "pch.h"
#include "GameCamera.h"


using namespace DirectX;

// コンストラクタ
GameCamera::GameCamera()
	: m_type(Type::Type_A)
	, m_angle(0.0f)
	, m_pPlayerPos{}
	, m_pPlayerRotate{}
{
}

// 更新関数
void GameCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case Type::Type_A:	// プレイヤーの後ろから追いかけるカメラ
	{
		// プレイヤー基準オフセット
		SimpleMath::Vector3 targetOffset(0.0f, 6.5f, -5.0f);
		SimpleMath::Vector3 eyeOffset(0.0f, 7.0f, -6.5f);

		// 両方回転させる
		targetOffset = SimpleMath::Vector3::Transform(
			targetOffset, *m_pPlayerRotate);
		eyeOffset = SimpleMath::Vector3::Transform(
			eyeOffset, *m_pPlayerRotate);

		// ワールド座標化
		SimpleMath::Vector3 targetPos = *m_pPlayerPos + targetOffset;
		SimpleMath::Vector3 eyePos = *m_pPlayerPos + eyeOffset;

		SetPositionTarget(eyePos, targetPos);
	}
	break;
	case Type::Type_B:	// プレイヤーの周りを回るカメラ
	{
		m_angle += XMConvertToRadians(30.0f) * elapsedTime;
		SimpleMath::Vector3 targetPos =
			*m_pPlayerPos + SimpleMath::Vector3(0.0f, 0.3f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 1.0f, 4.0f);
		eyePos = SimpleMath::Vector3::Transform(
			eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
	break;
	default:
		break;
	}
}
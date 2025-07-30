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
void GameCamera::Update(float elapsedTime, int number)
{
	switch (number)
	{
	case 1:
		m_type = Type::Type_A;
		break;
	case 2:
		m_type = Type::Type_B;
		break;
	default:
		break;
	}

	switch (m_type)
	{
	case Type::Type_A:	// プレイヤーの後ろから追いかけるカメラ
	{
		SimpleMath::Vector3 targetPos =
			*m_pPlayerPos + SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 2.0f, -7.0f);
		eyePos = SimpleMath::Vector3::Transform(eyePos, *m_pPlayerRotate);
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
	break;
	case Type::Type_B:	// プレイヤーの周りを回るカメラ
	{
		m_angle += XMConvertToRadians(30.0f) * elapsedTime;
		SimpleMath::Vector3 targetPos =
			*m_pPlayerPos + SimpleMath::Vector3(0.0f, 1.0f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 5.0f, 6.0f);
		eyePos = SimpleMath::Vector3::Transform(
			eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
	break;
	default:
		break;
	}
}
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
	case 3:
		m_type = Type::Type_C;
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
		SimpleMath::Vector3 eyePos(0.0f, 2.0f, -10.0f);
		eyePos = SimpleMath::Vector3::Transform(eyePos, *m_pPlayerRotate);
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
	break;
	case Type::Type_B:	// プレイヤーの周りを回るカメラ
	{
		m_angle += XMConvertToRadians(30.0f) * elapsedTime;
		SimpleMath::Vector3 targetPos =
			*m_pPlayerPos + SimpleMath::Vector3(0.0f, 2.0f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 2.3f, 2.0f);
		eyePos = SimpleMath::Vector3::Transform(
			eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
	break;
	case Type::Type_C: // 一人称視点
	{
		SimpleMath::Vector3 eyePos(0.0f, 2.0f, 0.0f);
		eyePos += *m_pPlayerPos;
		SimpleMath::Vector3 forward = SimpleMath::Vector3::Transform
		(SimpleMath::Vector3::UnitZ, *m_pPlayerRotate);
		forward.Normalize();
		SimpleMath::Vector3 targetPos =
			eyePos + forward * 5.0f;
		SetPositionTarget(eyePos, targetPos);
	}
		break;
	default:
		break;
	}
}
#include "pch.h"
#include "GameCamera.h"


using namespace DirectX;

// コンストラクタ
GameCamera::GameCamera()
	: m_type(Type::Type_A)
	, m_angle(0.0f)
	, m_pPlayerPos{}
	, m_pPlayerRotate{}
	, m_eyeOffset{0.0f, 7.0f, -6.5f}
	, m_targetOffset{0.0f, 6.5f, -5.0f}
	, m_defaultEyeOffset{m_eyeOffset}
	, m_defaultTargetOffset{m_targetOffset}
{
}

// 更新関数
void GameCamera::Update(float elapsedTime)
{
	if (!m_pPlayerPos || !m_pPlayerRotate)
		return;

	using namespace DirectX::SimpleMath;

	switch (m_type)
	{
	case Type::Type_A:
	{
		Vector3 targetOffset = Vector3::Transform(
			m_targetOffset, *m_pPlayerRotate);
		Vector3 eyeOffset = Vector3::Transform(
			m_eyeOffset, *m_pPlayerRotate);

		Vector3 targetPos = *m_pPlayerPos + targetOffset;
		Vector3 eyePos = *m_pPlayerPos + eyeOffset;

		SetPositionTarget(eyePos, targetPos);
	}
	break;
	
	default:
		break;
	}
}
#include "pch.h"
#include "GameCamera.h"
#include "Charactor/Player/Player.h"
#include "Charactor/Enemy/Enemy.h"

using namespace DirectX;

// コンストラクタ
GameCamera::GameCamera()
	: m_type(Type::Normal)
	, m_angle(0.0f)
	, m_eyeStart(0, 0, 0)
	, m_targetStart(0, 0, 1)
	, m_lerpTime(0.0f)
{
}

// 更新関数
void GameCamera::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	if (!m_player) return;

	switch (m_type)
	{
	case Type::Normal:
	{
		auto targetPos = m_player->GetPlayerPosition() + Vector3(0, 1, 0);

		// プレイヤーの背後オフセット
		Vector3 offset(0, 2, -10);

		// プレイヤーの回転を反映
		offset = Vector3::Transform(offset, m_player->GetPlayerRotate());

		auto eyePos = targetPos + offset;

		SetPositionTarget(eyePos, targetPos);

		// 内部保存用
		m_currentEye = eyePos;
		m_currentTarget = targetPos;
	}
	break;

	case Type::Attack:
	case Type::Return:
	{
		// 補間タイマー
		m_lerpTime += elapsedTime;
		float t = std::min(m_lerpTime / m_lerpDuration, 1.0f);

		// 位置と注視点の補間
		auto eye = Vector3::Lerp(m_eyeStart, m_eyeEnd, t);
		auto target = Vector3::Lerp(m_targetStart, m_targetEnd, t);

		SetPositionTarget(eye, target);
		
		// 内部保存
		m_currentEye = eye;
		m_currentTarget = target;

		// 三人称視点に補間で戻す
		if (t >= 1.0f)
		{
			if (m_type == Type::Return)
				m_type = Type::Normal; // 自動で三人称に戻す
		}
	}
	break;
	}

	// カットイン用
	if (m_showCutIn)
	{
		m_cutInTimer -= elapsedTime;
		if (m_cutInTimer <= 0.0f)
		{
			m_showCutIn = false; // 一定時間で消す
		}
	}

}



void GameCamera::ChangeMode(Type type)
{
	m_type = type;
	m_lerpTime = 0.0f;

	// 現在位置を start に保存
	m_eyeStart = m_currentEye;
	m_targetStart = m_currentTarget;

	if (type == Type::Attack)
	{
		auto playerPos = m_player->GetPlayerPosition();
		auto enemy = m_player->GetAttackTarget();

		if (enemy)
		{
			auto enemyPos = enemy->GetPos();

			// プレイヤーと敵の間を注視点に
			auto mid = (playerPos + enemyPos) * 0.5f;
			m_targetEnd = mid;

			auto toEnemy = enemyPos - playerPos;
			toEnemy.Normalize();
			
			// カメラを右にずらす用
			auto right = toEnemy.Cross(SimpleMath::Vector3::UnitY);
			right.Normalize();

			// カメラ位置
			m_eyeEnd = mid - toEnemy * 10.0f + right * 8.0f + SimpleMath::Vector3(0, 7, 0);
		}

		// 即座に切り替え
		m_currentEye = m_eyeEnd;
		m_currentTarget = m_targetEnd;
		SetPositionTarget(m_eyeEnd, m_targetEnd);

		// カットイン演出フラグ
		m_showCutIn = true;
		m_cutInTimer = 0.45f; // 表示時間(秒)
	}
	else if (type == Type::Return)
	{
		// 三人称カメラの位置（例）
		auto playerPos = m_player->GetPlayerPosition();
		m_targetEnd = playerPos + DirectX::SimpleMath::Vector3(0, 1.0f, 0);
		m_eyeEnd = m_targetEnd + DirectX::SimpleMath::Vector3(0, 2.0f, -10.0f);
		m_lerpDuration = 0.5f;
	}
}
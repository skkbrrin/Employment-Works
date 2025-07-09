#pragma once
#ifndef ISOCCERBALL_DEFINED
#define ISOCCERBALL_DEFINED
#include "Interface/IState.h"
#include "Main/pch.h"

class IMoving : public IState
{
public:
	// 加速度を設定する
	virtual void SeAcceralation(const DirectX::SimpleMath::Vector3& acceralation) = 0;
	// ボールに触れる
	virtual void Touch() = 0;
	// ボールをキックオフする
	virtual void Kick(const DirectX::SimpleMath::Vector3& kickDirection) = 0;
};

#endif ISOCCERBALL_DEFINED

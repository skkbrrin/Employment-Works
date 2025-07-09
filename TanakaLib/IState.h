#pragma once
#ifndef ISTATE_DEFINED
#define ISTATE_DEFINED
#include "IObserver.h"

// IStateインタフェースを定義する(GOF Stateパターン)
class IState
{
public:

public:
	// デストラクタ
	virtual ~IState() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 事前更新する
	virtual void PreUpdate() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime) = 0;
	// 事後更新する
	virtual void PostUpdate() = 0;
	// 描画する
	virtual void Render() = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};

#endif		// ISTATE_DEFINED


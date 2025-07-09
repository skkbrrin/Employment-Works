#pragma once
#ifndef IOBJECT_DEFINED
#define IOBJECT_DEFINED
#include "IObserver.h"

class IObject : public IObserver
{
public:
	// 回転するときの最大速度
	virtual float GetMaxTurnRate() const = 0;
	// 最大移動速度を取得する
	virtual float GetMaxSpeed() const = 0;
	// 最大の力を取得する
	virtual float GetMaxForce() const = 0;
	// 質量を取得する
	virtual float GetMass() const = 0;
	// スケールを取得する
	virtual float GetScale() const = 0;
	// クォータニオンを返す
	virtual DirectX::SimpleMath::Quaternion& GetRotation() = 0;
	// クォータニオンを設定する
	virtual void SetRotation(const DirectX::SimpleMath::Quaternion& quaternion) = 0;
	// 横ベクトルを取得する
	virtual DirectX::SimpleMath::Vector3& GetSide() = 0;
	// 向いている方向を取得する
	virtual DirectX::SimpleMath::Vector3& GetHeading() = 0;
	// 位置を取得する
	virtual DirectX::SimpleMath::Vector3& GetPosition() = 0;
	// 位置を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& position) = 0;
	// 速度を取得する
	virtual DirectX::SimpleMath::Vector3& GetVelocity() = 0;
	// 速度を設定する
	virtual void SetVelocity(const DirectX::SimpleMath::Vector3& velocity) = 0;
	// 加速度を取得する
	virtual DirectX::SimpleMath::Vector3& GetAcceralation() = 0;
	// 加速度を設定する
	virtual void SetAcceralation(const DirectX::SimpleMath::Vector3& acceralation) = 0;

public:
	// コンストラクタ
	virtual ~IObject() = default;
	// 初期化する
	virtual void Initialize() = 0;
	// 更新する
	virtual void Update(const float& elapsedTime) = 0;
	// 描画する
	virtual void Render() = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};

#endif			// IOBJECT_DEFINED

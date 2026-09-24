//--------------------------------------------------------------------------------------
// File: Number.h
//
// 数字を管理するクラス
//
// Date: 2025.4.3
//--------------------------------------------------------------------------------------
#pragma once

#include "ItoLib/TaskManager.h"
#include "SpriteSheet.h"

class Number : public Ito::Task
{
public:
	// コンストラクタ
	Number(std::unique_ptr<DirectX::SpriteBatch>* pSpriteBatch
		, ID3D11ShaderResourceView** pNumberSRV
		, uint32_t digit = 8
		, float scale = 1.0f
		, DirectX::FXMVECTOR color = DirectX::Colors::White
	);

	// デストラクタ
	~Number();
	// 更新
	bool Update(float elapsedTime) override;
	// 描画
	void Render()override;

private:

	// スプライトシート
	std::unique_ptr<SpriteSheet> m_sprites;

	// スプライトバッチポインタ
	std::unique_ptr<DirectX::SpriteBatch>* m_spriteBatch;

	// 数字のテクスチャハンドルポインタ
	ID3D11ShaderResourceView** m_pNumberSRV;

	// 表示位置
	DirectX::SimpleMath::Vector2 m_pos;

	// 表示桁数
	uint32_t m_digit;

	// スケール
	float m_scale;

	// 表示する数字
	uint32_t m_number;

	// 表示色
	DirectX::FXMVECTOR m_color;

	// スプライトフレーム
	const SpriteSheet::SpriteFrame* m_frame[10];

	// 表示するかどうか
	bool m_vis;

public:

	void SetPosition(DirectX::SimpleMath::Vector2 pos) { m_pos = pos; }

	void SetDigit(uint32_t digit) { m_digit = digit; }

	void SetScale(float scale) { m_scale = scale; }

	void SetNumber(uint32_t number) { m_number = number; }

	void SetVisible(bool vis) { m_vis = vis; }
};
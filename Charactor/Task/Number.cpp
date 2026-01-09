//--------------------------------------------------------------------------------------
// File: Number.cpp
//
// êîéöÇä«óùÇ∑ÇÈÉNÉâÉX
//
// Date: 2025.4.3
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "Number.h"

Number::Number(std::unique_ptr<DirectX::SpriteBatch>* pSpriteBatch, ID3D11ShaderResourceView** pNumberSRV, uint32_t digit, float scale, DirectX::FXMVECTOR color)
	: m_spriteBatch(pSpriteBatch)
	, m_pNumberSRV(pNumberSRV)
	, m_digit(digit)
	, m_scale(scale)
	, m_number(0)
	, m_frame{}
	, m_color(color)
{
	m_sprites = std::make_unique<SpriteSheet>();

	m_sprites->Load(*m_pNumberSRV, L"Resources/Textures/number.txt");

	for (size_t i = 0; i < 10; i++)
	{
		std::wostringstream oss;
		oss << "number_" << i;

		m_frame[i] = m_sprites->Find(oss.str().c_str());
		assert(m_frame[i] != 0);
	}
}

Number::~Number()
{
}

bool Number::Update(float elapsedTime)
{
	return true;
}

void Number::Render()
{
	uint32_t number = m_number;
	DirectX::SimpleMath::Vector2 pos = m_pos;

	float w = m_frame[0]->size.x;
	float h = m_frame[0]->size.y;

	pos.x += m_frame[0]->origin.x * m_scale;
	pos.y += m_frame[0]->origin.y * m_scale;

	pos.x += static_cast<float>(w) * m_scale * (m_digit - 1);

	do
	{
		uint32_t no = number % 10;
		number /= 10;
		m_sprites->Draw((*m_spriteBatch).get(), *m_frame[no], pos, m_color, 0.0f, m_scale);
		pos.x -= static_cast<float>(w) * m_scale;
	} while (number != 0);
}

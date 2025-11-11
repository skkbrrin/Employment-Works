#include "pch.h"
#include "Charactor/Player/Player.h"
#include "Scene/PlayScene.h"
#include "Ene"

using namespace DirectX;

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize( ID3D11Device* device , ID3D11DeviceContext* context)
{
	std::unique_ptr<DirectX::EffectFactory> fx = std::make_unique<DirectX::EffectFactory>(device);
	fx->SetDirectory(L"Resources/Models");
	m_player = DirectX::Model::CreateFromSDKMESH(device, L"Resources/Models/Player.sdkmesh", *fx);

    m_collition = Ito::ModelCollisionFactory::CreateCollision
    (Ito::ModelCollision::CollisionType::OBB, m_player.get());

    m_displayCol = std::make_unique<Ito::DisplayCollision>(device, context);

	m_rotate = SimpleMath::Quaternion::Identity;
	m_position = SimpleMath::Vector3(0.0f, -0.5f, 0.0f);

	m_fullHP = 100;
	m_HP = m_fullHP;
	m_attck = 10;

    // ƒTƒEƒ“ƒh
    AUDIO_ENGINE_FLAGS flags = AudioEngine_Default;
    m_audioEngine = std::make_unique<AudioEngine>(flags);

    try {
        m_sound = std::make_unique<SoundEffect>(m_audioEngine.get(), L"Resources/Sounds/Momiziyado.wav");
        m_soundInstance = m_sound->CreateInstance();
    }
    catch (const std::exception& e) {
        OutputDebugStringA(e.what());
    }
}

void Player::Update(float elapsedTime, Enemy* enemy)
{
	auto kb = Keyboard::Get().GetState();
	m_tracker.Update(kb);

    //’ÊíˆÚ“®
    if (kb.Left) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(2.0f));
    if (kb.Right) m_rotate = m_rotate * SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, XMConvertToRadians(-2.0f));

    if (kb.Up) m_position += SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 0.1f), m_rotate);
    if (m_tracker.pressed.Down) m_position -= SimpleMath::Vector3::Transform(SimpleMath::Vector3(0.0f, 0.0f, 7.0f), m_rotate);

    m_collition->UpdateBoundingInfo(m_position, m_rotate);
    m_collition->AddDisplayCollision(m_displayCol.get());
}


void Player::Render(ID3D11DeviceContext* context, DirectX::CommonStates* states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	SimpleMath::Matrix p_world;
    p_world = SimpleMath::Matrix::CreateFromQuaternion(m_rotate)
        * SimpleMath::Matrix::CreateTranslation(m_position);

	m_player->Draw(context, *states, p_world, view, proj);

    m_displayCol->DrawCollision(context, states, view, proj);
}

void Player::Finalize()
{
}

void Player::NormalAttack(float elapsedTime, const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (enemies.empty()) return;

    Enemy* nearestEnemy = nullptr; //< ‹ß‚­‚Ì“G‚Ìƒ|ƒCƒ“ƒ^
    float nearestDist = FLT_MAX; //< ‹ß‚³

    // ˆê”Ô‹ß‚¢“G‚ð’T‚·
    for (auto& ene : enemies)
    {
        if (!ene || ene->GetIsDie()) continue;
        float dist = (ene->GetPos() - m_position).Length();
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearestEnemy = ene.get();
        }
    }

    if (!nearestEnemy) return;

    // ‘O•û‚É‚¢‚é‚©‚Ç‚¤‚©”»’è
    DirectX::SimpleMath::Vector3 forward =
        DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, m_rotate);
    forward.Normalize();

    DirectX::SimpleMath::Vector3 toEnemy = nearestEnemy->GetPos() - m_position;
    toEnemy.Normalize();

    float dot = forward.Dot(toEnemy);
    float angleThreshold = cosf(XMConvertToRadians(180.0f)); //< Ž‹–ìŠp
    if (dot < angleThreshold) return;

    // UŒ‚‰Â”\‹——£
    if (nearestDist < 3.0f)
    {
        nearestEnemy->Damage(m_attck);
        // ƒTƒEƒ“ƒh
    }
    else
    {
        // ƒTƒEƒ“ƒh
    }
}

void Player::ComboAttack(float elapsedTime, const std::vector<std::unique_ptr<Enemy>>& enemies)
{
    if (enemies.empty()) return;

    std::vector<Enemy*> candidates; //< UŒ‚Œó•â

    DirectX::SimpleMath::Vector3 forward =
        DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::UnitZ, m_rotate);
    forward.Normalize();

    float angleThreshold = cosf(XMConvertToRadians(75.0f)); //< Ž‹–ìŠp
    float attackRange = 10.0f; //< UŒ‚‹——£

    // UŒ‚‰Â”\”ÍˆÍ‚Ì“G‚ðŽûW
    for (auto& ene : enemies)
    {
        if (!ene || ene->GetIsDie()) continue;

        DirectX::SimpleMath::Vector3 toEnemy = ene->GetPos() - m_position;
        float dist = toEnemy.Length();
        toEnemy.Normalize();

        float dot = forward.Dot(toEnemy);
        if (dist <= attackRange && dot >= angleThreshold)
        {
            candidates.push_back(ene.get());
        }
    }

    if (candidates.empty()) return;

    // Œó•â‘Sˆõ‚ÉUŒ‚
    for (Enemy* target : candidates)
    {
        m_position = target->GetPos(); // uŠÔˆÚ“®
        target->Damage(m_attck);
    }
}

void Player::CreateDevices()
{
   
}

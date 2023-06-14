#include "PrecompileHeader.h"
#include "SlashEffect.h"

#include <GameEngineCore/GameEngineComponent.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include "Player.h"

#include "SlashLaser_Effect.h"

SlashEffect::SlashEffect()
{
}

SlashEffect::~SlashEffect()
{
}

void SlashEffect::Start()
{
	if (false == GameEngineInput::IsKey("SlashEffect_DebugSwitch"))
	{
		GameEngineInput::CreateKey("SlashEffect_DebugSwitch", 'Q');
	}

	// 스프라이트네임이 nullptr 일때만 
	if (nullptr == GameEngineSprite::Find("slash"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("katanazero_resources");
		Dir.Move("katanazero_resources");
		Dir.Move("Texture");
		Dir.Move("ClubLevel");
		Dir.Move("Effect");

		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("slash").GetFullPath());
		std::vector<GameEngineFile> File = Dir.GetAllFile({ ".Png", });
	}

	// baseactor override 하는게 맞을듯 
	// 사용할 컴포넌트세팅
	ComponentSetting();
}

void SlashEffect::Update(float _DeltaTime)
{
	DebugUpdate();
	MoveUpdate(_DeltaTime);
	CollisionUpdate(_DeltaTime);
}

void SlashEffect::Render(float _DeltaTime)
{
	
}

void SlashEffect::DebugUpdate()
{
}

void SlashEffect::ComponentSetting()
{
	// 완
	m_Render = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::PLAYER_EFFECT);
	m_Render->SetScaleRatio(2.0f);
	m_Render->CreateAnimation({ .AnimationName = "slash_effect", .SpriteName = "slash", .Start = 0, .End = 4 ,
								.FrameInter = 0.04f , .Loop = false , .ScaleToTexture = true });
	m_Render->ChangeAnimation("slash_effect");

	// 이펙트 위치, 각도계산
	float RenderPivot = Player::MainPlayer->GetRenderPivot();
	const float4 PlayerPos = Player::MainPlayer->GetTransform()->GetLocalPosition();
	const float4 AttackPos = Player::MainPlayer->GetAttackPos();

	float4 Dir = AttackPos - PlayerPos;
	m_Angle = atan2f(Dir.y, Dir.x) * GameEngineMath::RadToDeg;
	m_Render->GetTransform()->SetLocalRotation({ 0 , 0 , m_Angle });

	// 충돌체 
	// 스케일은 임시크기 
	m_Collision = CreateComponent<GameEngineCollision>(ColOrder::PLAYER_ATTACK);
	m_Collision->GetTransform()->SetLocalScale(m_ColScale);
	m_Collision->GetTransform()->SetLocalRotation({ 0 , 0 , m_Angle });
	m_Collision->SetColType(ColType::OBBBOX3D);
	m_Collision->DebugOn();
	
	// 이벤트추가
	m_Render->SetAnimationStartEvent("slash_effect", static_cast<size_t>(3), std::bind(&SlashEffect::CollisionOff, this));
}

void SlashEffect::CollisionUpdate(float _DeltaTime)
{
	// 이펙트가 몬스터와 충돌했다면 
	// 개그지같이만들엇네 
	std::shared_ptr<GameEngineCollision> Col = m_Collision->Collision(ColOrder::MONSTER, ColType::OBBBOX3D, ColType::OBBBOX3D);

	// 여기 들어오면 충돌 한거고. 
	if (nullptr != Col)
	{
		// 충돌한 몬스터의 좌표 
		float4 ColPos = Col->GetTransform()->GetParent()->GetLocalPosition();
		const float4 PlayerPos = Player::MainPlayer->GetTransform()->GetLocalPosition();

		float4 Dir = ColPos - PlayerPos;

		float DirY = Dir.y;
		float EffectX = 0.0f; 
		float EffectY = 0.0f;
		if (ColPos.x >= PlayerPos.x)
		{
			EffectX = -800.0f;
		}

		else
		{
			EffectX = 800.0f;
		}

		if (ColPos.y >= PlayerPos.y)
		{
			EffectY = -330.0f;
		}

		else
		{
			EffectY = 330.0f;
		}
		
		// 그러면 레이저 이펙트를 생성하는데. 
		std::shared_ptr<SlashLaser_Effect> Effect = GetLevel()->CreateActor<SlashLaser_Effect>(static_cast<int>(RenderOrder::EFFECT));
		
		// 위치는 좀더 신경써야할듯. 
		Effect->GetTransform()->SetLocalPosition({ EffectX , EffectY });
		Effect->SetMovePos(ColPos);
	}
}

void SlashEffect::MoveUpdate(float _DeltaTime)
{
	// 여기서 충돌체크를해서 충돌체를 오프시키는게 편할거같은데
	if (nullptr != m_Render)
	{
		if (m_Render->IsAnimationEnd())
		{
			this->Death();
			return;
		}
	}

	// 충돌체는 이펙트의 3번프레임에서 제거

	if (nullptr == Player::MainPlayer)
	{
		MsgAssert("플레이어가 nullptr 입니다.");
		return;
	}

	m_PlayerPos = Player::MainPlayer->GetTransform()->GetLocalPosition();
	m_Render->GetTransform()->SetLocalPosition({ m_PlayerPos.x , m_PlayerPos.y + m_RenderPivot });	
	if (false == Player::MainPlayer->GetDir())
	{
		m_Collision->GetTransform()->SetLocalPosition({ m_PlayerPos.x + -m_ColPivot , m_PlayerPos.y + m_RenderPivot });
		return;
	}

	m_Collision->GetTransform()->SetLocalPosition({ m_PlayerPos.x + m_ColPivot , m_PlayerPos.y + m_RenderPivot - 10.0f });
}

void SlashEffect::CollisionOff()
{
	if (nullptr != m_Collision)
	{
		m_Collision->Off();
	}
}

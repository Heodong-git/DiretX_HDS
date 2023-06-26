#include "PrecompileHeader.h"
#include "Boss_HeadHunter.h"

#include <GameEngineBase/GameEngineRandom.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>


#include "PixelCollider.h"
#include "Player.h"

#include "HeadHunter_RifleEffect.h"
#include "TeleportEffect.h"
#include "GunSpark_Effect.h"

#include "Monster_Gangster.h"
#include "Monster_Grunt.h"
#include "Monster_Pomp.h"


Boss_HeadHunter::Boss_HeadHunter()
{
}

Boss_HeadHunter::~Boss_HeadHunter()
{
}

void Boss_HeadHunter::Start()
{
	ComponentSetting();
	LoadAndCreateAnimation();
	ChangeState(BossState::INTRO);

	// 단순하게 몬스터 소환위치만 1번부터 4번까지 넣어준다.
	m_SummonsPoss.reserve(4);

	float4 Gate1Pos = float4 { -453.0f, -203.0f};
	float4 Gate2Pos = float4 { -161.0f, -203.0f};
	float4 Gate3Pos = float4 { 180.0f, -203.0f};
	float4 Gate4Pos = float4 { 440.0f, -203.0f};

	m_SummonsPoss.push_back(Gate1Pos);
	m_SummonsPoss.push_back(Gate2Pos);
	m_SummonsPoss.push_back(Gate3Pos);
	m_SummonsPoss.push_back(Gate4Pos);
}

void Boss_HeadHunter::Update(float _DeltaTime)
{
	DebugUpdate();
	HurtCheck(_DeltaTime);
	NextTransUpdate();
	UpdateState(_DeltaTime);
}

void Boss_HeadHunter::Render(float _DeltaTime)
{
}

void Boss_HeadHunter::NextTransUpdate()
{
	m_NextTrans->SetLocalPosition(GetTransform()->GetWorldPosition());
}

void Boss_HeadHunter::ComponentSetting()
{
	m_NextTrans = std::make_shared<GameEngineTransform>();

	m_MainRender = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::BOSS);
	m_MainRender->GetTransform()->SetLocalPosition(float4{ 0.0f, 42.0f });

	m_Collision = CreateComponent<GameEngineCollision>(ColOrder::BOSS);
	m_Collision->GetTransform()->SetLocalPosition(float4{ 0.0f, 37.0f });
	m_Collision->GetTransform()->SetLocalScale(float4{ 30.0f, 75.0f });
	m_Collision->SetColType(ColType::OBBBOX3D);

	m_DebugRender = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::DEBUG);
	m_DebugRender->GetTransform()->SetLocalScale(float4{ 4.0f , 4.0f });
	m_DebugRender->Off();

	m_DebugRender_Right = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::DEBUG);
	m_DebugRender_Right->GetTransform()->SetLocalPosition(m_DebugPivot);
	m_DebugRender_Right->GetTransform()->SetLocalScale(float4{ 4.0f , 4.0f });
	m_DebugRender_Right->Off();
}

void Boss_HeadHunter::LoadAndCreateAnimation()
{
	if (nullptr == GameEngineSprite::Find("headhunter_idle"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("katanazero_resources");
		Dir.Move("katanazero_resources");
		Dir.Move("Texture");
		Dir.Move("ClubLevel");
		Dir.Move("headhunter");

		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_idle").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_intro").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_takeout_rifle").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_roll").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_hurt").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_recover").GetFullPath());
		
	}

	m_MainRender->CreateAnimation({ .AnimationName = "headhunter_idle", .SpriteName = "headhunter_idle", .Start = 0, .End = 11 ,
									  .FrameInter = 0.09f , .Loop = true , .ScaleToTexture = true });
	m_MainRender->CreateAnimation({ .AnimationName = "headhunter_intro", .SpriteName = "headhunter_intro", .Start = 0, .End = 2 ,
									  .FrameInter = 0.06f , .Loop = false , .ScaleToTexture = true });
	m_MainRender->CreateAnimation({ .AnimationName = "headhunter_takeout_rifle", .SpriteName = "headhunter_takeout_rifle", .Start = 0, .End = 7 ,
								  .FrameInter = 0.055f , .Loop = false , .ScaleToTexture = true });
	m_MainRender->CreateAnimation({ .AnimationName = "headhunter_roll", .SpriteName = "headhunter_roll", .Start = 0, .End = 6 ,
							  .FrameInter = 0.055f , .Loop = false , .ScaleToTexture = true });
	m_MainRender->CreateAnimation({ .AnimationName = "headhunter_recover", .SpriteName = "headhunter_recover", .Start = 0, .End = 3 ,
							  .FrameInter = 0.03f , .Loop = false , .ScaleToTexture = true });

	std::vector<float> vFrameTime = std::vector<float>();
	vFrameTime.push_back(0.1f);
	vFrameTime.push_back(0.1f);
	vFrameTime.push_back(0.15f);
	vFrameTime.push_back(0.07f);
	vFrameTime.push_back(0.07f);
	vFrameTime.push_back(0.07f);
	vFrameTime.push_back(0.05f);
	vFrameTime.push_back(0.05f);
	vFrameTime.push_back(0.05f);
	vFrameTime.push_back(0.05f);

	AnimationParameter Para = {};
	Para.AnimationName = "headhunter_hurt";
	Para.SpriteName = "headhunter_hurt";
	Para.Start = 0;
	Para.End = 9;
	Para.FrameTime = vFrameTime;
	Para.Loop = false;
	Para.ScaleToTexture = true;

	m_MainRender->CreateAnimation(Para);
	
	m_MainRender->SetAnimationStartEvent("headhunter_takeout_rifle", static_cast<size_t>(5), std::bind(&Boss_HeadHunter::CreateRifleEffect, this));
	m_MainRender->ChangeAnimation("headhunter_idle");
	m_MainRender->SetScaleRatio(2.0f);
}

void Boss_HeadHunter::LoadSound()
{
}

void Boss_HeadHunter::CreateRifleEffect()
{
	float4 MyPos = GetTransform()->GetLocalPosition();
	m_Effect = GetLevel()->CreateActor<HeadHunter_RifleEffect>();
	m_Effect->SetActor(DynamicThis<Boss_HeadHunter>());
	if (true == m_Dir)
	{
		m_Effect->GetTransform()->SetLocalPosition(MyPos + float4{ m_RifleEffectPivot.x , m_RifleEffectPivot.y });
	}
	
	else if (false == m_Dir)
	{
		m_Effect->GetTransform()->SetLocalPosition(MyPos + float4{ -m_RifleEffectPivot.x, m_RifleEffectPivot.y});
	}
}

void Boss_HeadHunter::CreateTpEffect()
{
	std::shared_ptr<TeleportEffect> Effect = GetLevel()->CreateActor<TeleportEffect>();
	Effect->GetTransform()->SetLocalPosition(GetTransform()->GetLocalPosition());

	if (true == m_Dir)
	{
		Effect->GetTransform()->SetLocalNegativeScaleX();
	}

	else if (false == m_Dir)
	{
		Effect->GetTransform()->SetLocalPositiveScaleX();
	}
}

void Boss_HeadHunter::Reset()
{
	GetTransform()->SetLocalPosition(GetInitPos());
	ChangeState(BossState::INTRO);
	m_IdleDuration = 0.25f;
	m_HitCount = 8;
	m_FirstSummons = false;
	m_SecondSummons = false;

	// 만들어놨던 액터들 다 데스처리
	if (0 != m_SummonsMonsters.size())
	{
		for (size_t i = 0; i < m_SummonsMonsters.size(); ++i)
		{
			m_SummonsMonsters[i]->Death();
		}

		m_SummonsMonsters.clear();
	}
}

void Boss_HeadHunter::HurtCheck(float _DeltaTime)
{
	// 내가 플레이어의 공격과 충돌했다면.
	std::shared_ptr<GameEngineCollision> Player_AttCol = m_Collision->Collision(ColOrder::PLAYER_ATTACK, ColType::OBBBOX3D, ColType::OBBBOX3D);
	if (nullptr != Player_AttCol)
	{
		// 내가 플레이어의 공격과 충돌했다면 Hurt 상태로 전환
		// 1. 충돌했다면 바로 충돌체 off,
		// 2. hurt state로 전환
		m_Collision->Off();
		--m_HitCount;

		// 플레이어 x축 계산  
		float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		float4 MyPos = GetTransform()->GetWorldPosition();
		m_HitPos = GetTransform()->GetLocalPosition();
		// 플레이어보다 내 위치의 x값이 크다면 
		if (PlayerPos.x <= MyPos.x)
		{
			m_HitEndPos = m_HitPos + float4{ 100.0f, 0.0f};
		}

		else if (PlayerPos.x > MyPos.x)
		{
			m_HitEndPos = m_HitPos + float4{ -100.0f, 0.0f};
		}
		

		m_MiddlePos = m_HitPos + float4{ (m_HitEndPos.x - m_HitPos.x) / 2.0f , 25.0f };
		ChangeState(BossState::HURT);
		return;
	}
}

void Boss_HeadHunter::DebugUpdate()
{
	if (true == GameEngineInput::IsDown("player_debugswitch"))
	{
		DebugSwitch();

		if (true == IsDebug())
		{
			
			m_DebugRender->On();
			m_DebugRender_Right->On();
		}

		else if (false == IsDebug())
		{
			
			m_DebugRender->Off();
			m_DebugRender_Right->Off();
		}
	}
}

void Boss_HeadHunter::DirCheck()
{
	if (true == m_Dir)
	{
		GetTransform()->SetLocalPositiveScaleX();
	}

	else if (false == m_Dir)
	{
		GetTransform()->SetLocalNegativeScaleX();
	}
}

void Boss_HeadHunter::ChangeDir()
{
	// 플레이어 x축 계산  
	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	float4 MyPos = GetTransform()->GetWorldPosition();

	// 플레이어보다 내 위치의 x값이 크다면 
	if (PlayerPos.x <= MyPos.x)
	{
		m_Dir = false;
	}

	else if (PlayerPos.x > MyPos.x)
	{
		m_Dir = true;
	}
}

void Boss_HeadHunter::ChangeState(BossState _State)
{
	m_NextState = _State;
	m_PrevState = m_CurState;
	m_CurState = m_NextState;

	// start 
	switch (m_NextState)
	{
	case BossState::INTRO:
		IntroStart();
		break;
	case BossState::IDLE:
		IdleStart();
		break;
	case BossState::RIFLE:
		RifleStart();
		break;
	case BossState::GUN:
		GunStart();
		break;
	case BossState::ROLL:
		RollStart();
		break;
	case BossState::HURT:
		HurtStart();
		break;
	case BossState::RECOVER:
		RecoverStart();
		break;
	case BossState::TRANSPARENCY:
		TransparencyStart();
		break;
	case BossState::REAPPEAR:
		ReAppearStart();
		break;
	}

	// 이전 state의 end 
	switch (m_PrevState)
	{
	case BossState::INTRO:
		IntroEnd();
		break;
	case BossState::IDLE:
		IdleEnd();
		break;
	case BossState::RIFLE:
		RifleEnd();
		break;
	case BossState::GUN:
		GunEnd();
		break;
	case BossState::ROLL:
		RollEnd();
		break;
	case BossState::HURT:
		HurtEnd();
		break;
	case BossState::RECOVER:
		RecoverEnd();
		break;
	case BossState::TRANSPARENCY:
		TransparencyEnd();
		break;
	case BossState::REAPPEAR:
		ReAppearEnd();
		break;
	}
}

void Boss_HeadHunter::UpdateState(float _DeltaTime)
{
	// 현재 상태의 update 호출 
	switch (m_CurState)
	{
	case BossState::INTRO:
		IntroUpdate(_DeltaTime);
		break;
	case BossState::IDLE:
		IdleUpdate(_DeltaTime);
		break;
	case BossState::RIFLE:
		RifleUpdate(_DeltaTime);
		break;
	case BossState::GUN:
		GunUpdate(_DeltaTime);
		break;
	case BossState::ROLL:
		RollUpdate(_DeltaTime);
		break;
	case BossState::HURT:
		HurtUpdate(_DeltaTime);
		break;
	case BossState::RECOVER:
		RecoverUpdate(_DeltaTime);
		break;
	case BossState::TRANSPARENCY:
		TransparencyUpdate(_DeltaTime);
		break;
	case BossState::REAPPEAR:
		ReAppearUpdate(_DeltaTime);
		break;
	}
}

// 보스가 만들어지면 인트로로 진입, 순간이동 애니메이션으로 등장 
void Boss_HeadHunter::IntroStart()
{
	// 이때 만약, 이전상태가 투명상태였다면 전투진행중 이었던거고.
	// 랜덤한 위치에 다시 나타나서 전투진행. 

	m_Dir = false;
	DirCheck();
	m_MainRender->ChangeAnimation("headhunter_intro"); 
}

void Boss_HeadHunter::IntroUpdate(float _DeltaTime)
{
	if (m_MainRender->IsAnimationEnd())
	{
		ChangeState(BossState::IDLE);
		return;
	}
}

void Boss_HeadHunter::IntroEnd()
{
}


// 아이들상태 진입시 대기시간을 만들고, 그 시간을 초과하게 되면 바로 RIPLE 상태로 변경,
// 그냥 1초뒤에 
// 아니면 단순하게 플레이어와 충돌하게 되면 바로 라이플상태로 변경. 
void Boss_HeadHunter::IdleStart()
{
	m_Collision->On();
	ChangeDir();
	DirCheck();
	m_MainRender->ChangeAnimation("headhunter_idle");
}

void Boss_HeadHunter::IdleUpdate(float _DeltaTime)
{
	
	// 현재 페이즈가 1페이즈 일 경우의 동작,  
	if (BossPhase::FIRST == m_CurPhase)
	{

		if (0.0f >= m_IdleDuration)
		{
			// 아이들지속시간이 1초를 경과했을 경우 라이플 상태로 변경
			ChangeState(BossState::RIFLE);
			return;
		}

		// 여기서 애니메이션이 종료되면이 아니라, 랜덤하게 라이플 상태로 변경
		if (true == m_MainRender->IsAnimationEnd())
		{
			ChangeState(BossState::RIFLE);
			return;
		}
	}

	m_IdleDuration -= _DeltaTime;
	ChangeDir();
	DirCheck();
}

void Boss_HeadHunter::IdleEnd()
{
	m_IdleDuration = 0.25f;
}

void Boss_HeadHunter::RifleStart()
{
	ChangeDir();
	DirCheck();
	m_MainRender->ChangeAnimation("headhunter_takeout_rifle");
}

// 라이플 애니메이션이 종료되면 일단 퍼즈. 
void Boss_HeadHunter::RifleUpdate(float _DeltaTime)
{
	// 이펙트가 제거 
	if (true == m_MainRender->IsAnimationEnd() && true == m_Effect->IsDeath())
	{ 
		m_Effect = nullptr;
		ChangeState(BossState::ROLL);
		return;
	}
}

void Boss_HeadHunter::RifleEnd()
{
}

void Boss_HeadHunter::GunStart()
{
}

void Boss_HeadHunter::GunUpdate(float _DeltaTime)
{
}

void Boss_HeadHunter::GunEnd()
{
}

void Boss_HeadHunter::RollStart()
{
	m_Collision->Off();
	ChangeDir();
	DirCheck();
	m_MainRender->ChangeAnimation("headhunter_roll");
}

void Boss_HeadHunter::RollUpdate(float _DeltaTime)
{
	if (true == m_MainRender->IsAnimationEnd())
	{
		ChangeState(BossState::IDLE);
		return;
	}

	// 진행방향이 오른쪽이라면 
	if (true == m_Dir)
	{
	
		m_NextTrans->AddLocalPosition(float4::Right * m_RollSpeed * _DeltaTime);
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + m_DebugPivot))
		{
			// 넥스트트랜스를 이동키시고, 이동한 위치가 흰색일때만 내가 이동한다.
			GetTransform()->AddLocalPosition(float4::Right * m_RollSpeed * _DeltaTime);
		}
	}

	else if (false == m_Dir)
	{
		m_NextTrans->AddLocalPosition(float4::Left * m_RollSpeed * _DeltaTime);
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + float4 { -m_DebugPivot.x, m_DebugPivot.y }))
		{
			// 넥스트트랜스를 이동키시고, 이동한 위치가 흰색일때만 내가 이동한다.
			GetTransform()->AddLocalPosition(float4::Left * m_RollSpeed * _DeltaTime);
		}
	}
}

void Boss_HeadHunter::RollEnd()
{
	m_Collision->On();
}

void Boss_HeadHunter::HurtStart()
{
	// 시작할때 플레이어 위치x, 내위치 x를 비교, 방향을 정한다.
	ChangeDir();
	DirCheck();
	m_MainRender->ChangeAnimation("headhunter_hurt");
	m_MainRender->GetTransform()->AddLocalPosition(float4{ 0.0f, -10.0f });
}

void Boss_HeadHunter::HurtUpdate(float _DeltaTime)
{
	// 특정위치에 도달했다면 return 일단 그냥 리턴인데, 9번프레임이라면 
	if (1.0f <= m_Ratio)
	{
		if (true == m_MainRender->IsAnimationEnd())
		{
			PixelCollider::PixelCol->GroundCheck(this);
			ChangeState(BossState::RECOVER);
			return;
		}
		
		return;
	}

	m_Ratio += _DeltaTime / 0.5f ;
	if (1.0f <= m_Ratio)
	{
		m_Ratio = 1.0f;
	}

	// 방향에 따라서
	if (true == m_Dir)
	{
		float4 MovePos = float4::Lerp(m_HitPos, m_MiddlePos, m_Ratio * 2.0f);
		float4 MovePos2 = float4::Lerp(m_MiddlePos, m_HitEndPos, m_Ratio);
		float4 MovePos3 = float4::Lerp(MovePos, MovePos2, m_Ratio);

		m_NextTrans->SetWorldPosition(MovePos3);
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + float4 { -m_DebugPivot.x, m_DebugPivot.y }))
		{
			// 
			// 넥스트트랜스를 이동키시고, 이동한 위치가 흰색일때만 내가 이동한다.
			GetTransform()->SetWorldPosition(MovePos3);
		}

		// 만약 검은색  픽셀이라면 아래로만 이동시킨다. 
		// 만약 검은색  픽셀이라면 아래로만 이동시킨다. 
		else if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + float4 { -m_DebugPivot.x, m_DebugPivot.y }) &&
			PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition()))
		{
			GetTransform()->AddLocalPosition(float4::Down * 20.0f * _DeltaTime);
		}
	}
	else if (false == m_Dir)
	{
		float4 MovePos = float4::Lerp(m_HitPos, m_MiddlePos, m_Ratio);
		float4 MovePos2 = float4::Lerp(m_MiddlePos, m_HitEndPos, m_Ratio);
		float4 MovePos3 = float4::Lerp(MovePos, MovePos2, m_Ratio);

		m_NextTrans->SetWorldPosition(MovePos3);
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + m_DebugPivot))
		{
			// 넥스트트랜스를 이동키시고, 이동한 위치가 흰색일때만 내가 이동한다.
			GetTransform()->SetWorldPosition(MovePos3);
		}

		// 만약 검은색  픽셀이라면 아래로만 이동시킨다. 
		else if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition() + m_DebugPivot) && 
				 PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_NextTrans->GetWorldPosition()))
		{
			GetTransform()->AddLocalPosition(float4::Down * 20.0f * _DeltaTime);
		}
	}
}

void Boss_HeadHunter::HurtEnd()
{
	// 이 상태가 종료될 때 충돌체를 on
	m_Collision->On();
	m_HitPos = {};
	m_HitEndPos = {};
	m_MainPos = {};
	m_Ratio = 0.0f;
	m_MainRender->GetTransform()->AddLocalPosition(float4{ 0.0f, 10.0f });
}

void Boss_HeadHunter::RecoverStart()
{
	// 시작할때 플레이어 위치x, 내위치 x를 비교, 방향을 정한다.
	m_MainRender->GetTransform()->AddLocalPosition(float4{ 0.0f, -10.0f });
	m_MainRender->ChangeAnimation("headhunter_recover");
}

void Boss_HeadHunter::RecoverUpdate(float _DeltaTime)
{
	// 텔레포트 이펙트를 생성하고 나면 여기서 아이들이 아니라 투명 상태로 변경. 
	// 투명상태에서는 대기시간 3초를 가지고, 랜덤한 위치에 다시 등장한다? 
	if (true == m_MainRender->IsAnimationEnd())
	{
		CreateTpEffect();

		// 사라지고 나서 어떻게 할거니 
		ChangeState(BossState::TRANSPARENCY);
		return;
	}
}

void Boss_HeadHunter::RecoverEnd()
{
	m_MainRender->GetTransform()->AddLocalPosition(float4{ 0.0f, 10.0f });
	// test
	// m_MainRender->Off();
}

// 일단 투명상태, 투명상태일때 시간값을 증가시키고, 
// 일정시간을 초과하면
// 랜덤한 위치에 재등장 하도록 
// 인트로에서 이전상태가 트랜스 어쩌구였다면.. 으로 시작 
void Boss_HeadHunter::TransparencyStart()
{
	// 이때 충돌체를 off
	m_Collision->Off();
}

// 웨이브는 웨이브대로 진행을 하고, 보스몬스터는 투명상태 해제 후 라이플state 로 변경이 되네 

// 투명상태 시작하면 몬스터를 한번에 다 만들어두고, Off 상태,
// 앞에 나온 몬스터들이 사망상태가 될때마다 업데이트를 on, chase 상태로 변경한다. 이게 맞네 

void Boss_HeadHunter::TransparencyUpdate(float _DeltaTime)
{
	// 현재 1페이즈, 첫번째 소환이 진행되지 않았다면.
	if (BossPhase::FIRST == m_CurPhase && false == m_FirstSummons)
	{
		m_SummonsMonsters.reserve(8);
		// 몬스터를 생성하는데, 생성하고 내 액터를 저장하는 벡터에 넣어두고, 만약 리셋된다면 Death 처리? 
		m_FirstSummons = true;
		// 첫번째 소환상태를 true로 변경하고,
		// 특정위치에 몬스터를 소환한다. 
		// 첫번째 몬스터는 grunt, 소환위치는 

		// 몬스터도 가지고 있는다. 단순하게 그냥 가지고 있는게 편한거같은데 
		// 가지고 있지뭐 
		std::shared_ptr<Monster_Grunt> Monster = GetLevel()->CreateActor<Monster_Grunt>();
		int RandomValue = GameEngineRandom::MainRandom.RandomInt(0, 3);
		Monster->GetTransform()->SetLocalPosition(m_SummonsPoss[RandomValue]);
		Monster->ChangeState(GruntState::CHASE);

		// 동시에 같은곳에서 나오지 않게하려면, 랜덤값을 뽑고, 두번째몬스터는 그 랜덤값에 1또는 2를 더한 위치에 생성한다. 
		m_SummonsMonsters.push_back(Monster->DynamicThis<BaseActor>());

		std::shared_ptr<Monster_Grunt> Monster2 = GetLevel()->CreateActor<Monster_Grunt>();
		
		// 다음위치
		int Value = RandomValue + 1;
		if (Value >= static_cast<int>(m_SummonsPoss.size()))
		{
			Value = 0;
		}

		Monster2->GetTransform()->SetLocalPosition(m_SummonsPoss[Value]);
		Monster2->ChangeState(GruntState::CHASE);
		

		m_SummonsMonsters.push_back(Monster2->DynamicThis<BaseActor>());
	}
	// 내가 1히트라면. 몹을 등장시켜야함. 그냥 그대로 투명, start 에서 등장시키고. 
	// 만들어진 몹들이 다 죽으면, bool 변수를 true 로 만들어서 재등장상태로 변경
	// 마찬가지로 두번히트되엇을때, start에서 또 생성해
}

void Boss_HeadHunter::TransparencyEnd()
{
	// 투명상태 종료시 충돌체 on 
	m_Collision->On();
}

// 재등장 애니메이션이 끝나면, 랜덤한 패턴을 사용하도록. 
// 만약 현재 페이즈가 1페이즈라면, 라이플 스테이트로 변경 
void Boss_HeadHunter::ReAppearStart()
{
}

void Boss_HeadHunter::ReAppearUpdate(float _DeltaTime)
{
}

void Boss_HeadHunter::ReAppearEnd()
{
}

// f2로 재시작할 경우 원래 앞쪽 충돌체와 충돌하게 되면 레벨이 play 상태로 변경되는데 보스는 먼저 움직임. 내일하자 집중도안되는데 
#include "PrecompileHeader.h"
#include "Player.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEnginePlatform/GameEngineInput.h>
#include <GameEngineBase/GameEngineTime.h>

#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineSprite.h>
#include <GameEngineCore/GameEngineResource.h>
#include <GameEngineCore/GameEngineCollision.h>

// 타입인포 
#include <typeinfo>

// 레벨이 추가될때마다 헤더추가 후 Reset 함수에 추가.. 
#include "ClubLevel_00.h"
#include "ClubLevel_01.h"
#include "ClubLevel_02.h"
#include "ClubLevel_03.h"
#include "ClubLevel_04.h"
#include "ClubLevel_Boss.h"

// 카메라
#include "PlaySupporter.h"

#include "BaseLevel.h"
#include "Cursor.h"
#include "PixelCollider.h"
#include "SlashEffect.h"
#include "JumpEffect.h"
#include "LandEffect.h"
#include "DashEffect.h"

Player* Player::MainPlayer = nullptr;

Player::Player()
{
	// 플레이어는 어차피 하나 
	MainPlayer = this;
}

Player::~Player()
{
}

void Player::Start()
{
	// 액터 스타트 기본인터페이스 구성
	// 키생성

	// 컴포넌트 세팅
	ComponentSetting();
	// 필요한 리소스 로드및 애니메이션 생성
	LoadAndCreateAnimation();

	if (false == GameEngineInput::IsKey("player_slash"))
	{
		GameEngineInput::CreateKey("player_DebugSwitch", 'Q');
		GameEngineInput::CreateKey("player_slash", VK_LBUTTON);
		GameEngineInput::CreateKey("player_skill_slow", VK_LSHIFT);
		GameEngineInput::CreateKey("player_left_Move", 'A');
		GameEngineInput::CreateKey("player_right_Move", 'D');
		GameEngineInput::CreateKey("player_jump", 'W');
		GameEngineInput::CreateKey("player_crouch", 'S');
	}
}

void Player::LoadAndCreateAnimation()
{
	// 파일로드 기본인터페이스 
	{
		if (nullptr == GameEngineSprite::Find("player_idle"))
		{
			GameEngineDirectory Dir;
			Dir.MoveParentToDirectory("katanazero_resources");
			Dir.Move("katanazero_resources");
			Dir.Move("Texture");
			Dir.Move("ClubLevel");
			Dir.Move("player");

			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_idle").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_attack").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_idle_to_run").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_run").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_run_to_idle").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_crouch").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_flip").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_doorbreak").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_doorbreak_full").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_dooropen_gentle").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_fall").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_hurtfly_begin").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_hurtfly_loop").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_hurtground").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_hurtrecover").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_jump").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_wallslide").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_roll").GetFullPath());
			GameEngineSprite::LoadFolder(Dir.GetPlusFileName("player_die").GetFullPath());

			std::vector<GameEngineFile> File = Dir.GetAllFile({ ".Png", });
		}
	}

	m_Render->CreateAnimation({ .AnimationName = "player_idle", .SpriteName = "player_idle", .Start = 2, .End = 10 ,
									  .FrameInter = 0.12f , .Loop = true , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_attack", .SpriteName = "player_attack", .Start = 0, .End = 6 ,
									  .FrameInter = 0.03f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_idle_to_run", .SpriteName = "player_idle_to_run", .Start = 0, .End = 3 ,
								  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_run", .SpriteName = "player_run", .Start = 0, .End = 9 ,
								  .FrameInter = 0.05f , .Loop = true , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_run_to_idle", .SpriteName = "player_run_to_idle", .Start = 0, .End = 4 ,
								  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_roll", .SpriteName = "player_roll", .Start = 0, .End = 6 ,
									  .FrameInter = 0.03f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_crouch", .SpriteName = "player_crouch", .Start = 0, .End = 0 ,
							  .FrameInter = 0.01f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_flip", .SpriteName = "player_flip", .Start = 0, .End = 10,
						  .FrameInter = 0.03f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_doorbreak", .SpriteName = "player_doorbreak", .Start = 0, .End = 5,
						  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_doorbreak_full", .SpriteName = "player_doorbreak_full", .Start = 0, .End = 9,
						  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_dooropen_gentle", .SpriteName = "player_dooropen_gentle", .Start = 0, .End = 8,
						  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_fall", .SpriteName = "player_fall", .Start = 0, .End = 3,
						  .FrameInter = 0.05f , .Loop = true , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_hurtfly_begin", .SpriteName = "player_hurtfly_begin", .Start = 0, .End = 1,
						  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_hurtfly_loop", .SpriteName = "player_hurtfly_loop", .Start = 0, .End = 3,
						  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_hurtground", .SpriteName = "player_hurtground", .Start = 0, .End = 5,
					  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_hurtrecover", .SpriteName = "player_hurtrecover", .Start = 0, .End = 8,
					  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_jump", .SpriteName = "player_jump", .Start = 0, .End = 3,
					  .FrameInter = 0.05f , .Loop = true , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_wallslide", .SpriteName = "player_wallslide", .Start = 0, .End = 1,
					  .FrameInter = 0.05f , .Loop = false , .ScaleToTexture = true });

	m_Render->CreateAnimation({ .AnimationName = "player_die", .SpriteName = "player_die", .Start = 0, .End = 11,
					  .FrameInter = 0.1f , .Loop = false , .ScaleToTexture = true });

	m_Render->ChangeAnimation("player_idle");
}

void Player::Update(float _DeltaTime)
{
	DirCheck();
	SkillUpdate(_DeltaTime);
	
	// 상태업데이트
	UpdateState(_DeltaTime);

	// 카메라무브 업데이트
	if (nullptr != PlaySupporter::MainSupporter)
	{
		PlaySupporter::MainSupporter->Update(_DeltaTime);
	}

	// 디버그 업데이트
	DebugUpdate();

	// 데스체크, 만약 스테이지 제한시간을 넘어섰다면 
	if (0.0f >= GetReturnCastLevel()->GetLimitTime())
	{
		if (PlayerState::DEATH == m_CurState)
		{
			return;
		}

		ChangeState(PlayerState::DEATH);
	}
}

void Player::Render(float _DeltaTime)
{
}

void Player::Reset()
{
	float4 SetPos = GetInitPos();

	GetTransform()->SetLocalPosition(SetPos);
	ResetSkillLimitTime();
	ResetDir();	
	ChangeState(PlayerState::IDLE);
}

void Player::ComponentSetting()
{
	// 픽셀충돌 
	CreateComponent<PixelCollider>();

	// 메인렌더 
	m_Render = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::PLAYER);
	m_Render->GetTransform()->SetLocalPosition({ 0, m_RenderPivot });
	m_Render->SetScaleRatio(2.0f);

	// 콜리전 
	m_Collision = CreateComponent<GameEngineCollision>(static_cast<int>(ColOrder::PLAYER));
	m_Collision->GetTransform()->SetLocalScale(m_ColScale);
	m_Collision->GetTransform()->SetLocalPosition({ 0, m_RenderPivot });

	// --------------------------- Debug Render ------------------------------

	m_DebugRender_Bottom = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_Bottom->GetTransform()->SetLocalScale(m_DebugRenderScale);

	m_DebugRender_Bottom_Down = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_Bottom_Down->GetTransform()->SetLocalScale(m_DebugRenderScale);
	m_DebugRender_Bottom_Down->GetTransform()->SetLocalPosition({ 0.0f, -1.0f });

	m_DebugRender_Left = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_Left->GetTransform()->SetLocalScale(m_DebugRenderScale);
	m_DebugRender_Left->GetTransform()->SetLocalPosition({ -36.0f, m_RenderPivot });
	
	m_DebugRender_Right = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_Right->GetTransform()->SetLocalScale(m_DebugRenderScale);
	m_DebugRender_Right->GetTransform()->SetLocalPosition({ 36.0f, m_RenderPivot });

	m_DebugRender_Top = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_Top->GetTransform()->SetLocalScale(m_DebugRenderScale);
	m_DebugRender_Top->GetTransform()->SetLocalPosition({ 0.0f , m_RenderPivot * 2.0f });

	// 흠.. 
	m_DebugRender_WallRight = CreateComponent<GameEngineSpriteRenderer>();
	m_DebugRender_WallRight->GetTransform()->SetLocalScale(m_DebugRenderScale);
	m_DebugRender_WallRight->GetTransform()->SetLocalPosition({ 2.0f , m_RenderPivot });
}

void Player::DirCheck()
{
	if (true == GameEngineInput::IsPress("player_right_move"))
	{
		m_Direction = true;
		GetTransform()->SetLocalPositiveScaleX();
	}

	if (true == GameEngineInput::IsPress("player_left_move"))
	{
		m_Direction = false;
		GetTransform()->SetLocalNegativeScaleX();
	}
}

// ---------------------------------------- Debug -----------------------------------------
void Player::DebugUpdate()
{
	if (true == GameEngineInput::IsDown("player_debugswitch"))
	{
		DebugSwitch();
	}

	if (true == IsDebug())
	{
		m_DebugRender_Bottom->On();
		m_DebugRender_Left->On();
		m_DebugRender_Right->On();
		m_DebugRender_Top->On();
		m_DebugRender_WallRight->On();
		m_DebugRender_Bottom_Down->On();
	}

	else if (false == IsDebug())
	{
		m_DebugRender_Bottom->Off();
		m_DebugRender_Left->Off();
		m_DebugRender_Right->Off();
		m_DebugRender_Top->Off();
		m_DebugRender_WallRight->Off();
		m_DebugRender_Bottom_Down->Off();
	}
}

// 후순위
void Player::SkillUpdate(float _DeltaTime)
{
	if (PlayerState::DEATH == m_CurState)
	{
		// 스킬 false
		m_IsSkill = false;
		// 색돌리고 
		m_Render->ColorOptionValue.MulColor.r = 1.0f;
		// 슬로우리셋
		SlowReset();
		return;
	}

	// 스케일적용 X 델타타임을 받아둔다.  
	float OriginTime = GameEngineTime::GlobalTime.GetDeltaTime();

	// 만약 배터리가 모두 소모 되었을 때의 충전로직
	// 아래 if 문이랑 합쳐질거같기도한데 일단은 되니까 나중에
	if (true == m_BatteryCharge)
	{
		m_SkillLimitTime += OriginTime;
		m_LimitTimeValue = m_SkillLimitTime;
		if (true == GameEngineInput::IsPress("player_skill_slow"))
		{
			m_Render->ColorOptionValue.MulColor.r = 1.0f;

			if (9.0f <= m_SkillLimitTime)
			{
				m_SkillLimitTime = 9.0f;
				m_BatteryCharge = false;
			}
			return;
		}

		else if (true == GameEngineInput::IsUp("player_skill_slow"))
		{
			m_SkillLimitTime = m_LimitTimeValue;
			m_LimitTimeValue = 0.0f;
			m_BatteryCharge = false;
			return;
		}
	}

	// 눌려있으면 스킬발동중 인거고 , 배터리 차지가 false 일때만 들어온다. 
	if (true == GameEngineInput::IsPress("player_skill_slow"))
	{
		m_Render->ColorOptionValue.MulColor.r = 0.1f;
		
		// 만약 누르고 있는중에 지속시간이 0.0초보다 작아지게 되면 스킬을 종료하고
		// 스킬 사용 상태를 false로 변경, 제한시간을 0.0초로 초기화한다. 
		if (0.0f >= m_SkillLimitTime)
		{
			m_BatteryCharge = true;
			SlowReset();
			m_SkillLimitTime = 0.0f;
			m_IsSkill = false;
			return;
		}

		// 지금 누르고 있는 상태이기 때문에 스킬온 
		m_IsSkill = true;

		// 지속시간을 감소시킨다. 
		m_SkillLimitTime -= OriginTime;

		// 델타타임에 스케일을 적용한다. 
		Slow();
	}

	// 여긴 어차피 안눌린 상태니까 기존의 델타타임이 들어올거고 
	else if (false == GameEngineInput::IsPress("player_skill_slow"))
	{
		m_Render->ColorOptionValue.MulColor.r = 1.0f;
		// 안눌렸으면 바로 타임스케일리셋 
		SlowReset();
		m_IsSkill = false;
		if (9.0f <= m_SkillLimitTime)
		{
			m_SkillLimitTime = 9.0f;
			return;
		}

		m_SkillLimitTime += _DeltaTime;
	}
}

// ---------------------------------skill --------------------------------------
void Player::Slow()
{
	if (true == m_IsSkill)
	{
		GameEngineTime::GlobalTime.SetGlobalTimeScale(0.15f);
	}
}

void Player::SlowReset()
{
	GameEngineTime::GlobalTime.SetGlobalTimeScale(1.0f);
}


// ---------------------------------------- state ------------------------------------------ 
void Player::UpdateState(float _DeltaTime)
{
	// 현재 상태의 update 호출 
	switch (m_CurState)
	{
	case PlayerState::IDLE:
		IdleUpdate(_DeltaTime);
		break;
	case PlayerState::IDLETORUN:
		IdleToRunUpdate(_DeltaTime);
		break;
	case PlayerState::MOVE:
		MoveUpdate(_DeltaTime);
		break;
	case PlayerState::ROLL:
		RollUpdate(_DeltaTime);
		break;
	case PlayerState::JUMP:
		JumpUpdate(_DeltaTime);
		break;
	case PlayerState::SLASH:
		SlashUpdate(_DeltaTime);
		break; 
	case PlayerState::CROUCH:
		CrouchUpdate(_DeltaTime);
		break;
	case PlayerState::FLIP:
		FlipUpdate(_DeltaTime);
		break;
	case PlayerState::FALL:
		FallUpdate(_DeltaTime);
		break;
	case PlayerState::RIGHTWALL:
		RightWallUpdate(_DeltaTime);
		break;
	case PlayerState::LEFTWALL:
		LeftWallUpdate(_DeltaTime);
		break;
	case PlayerState::DEATH:
		DeathUpdate(_DeltaTime);
		break;
	}
}


// state 변경, 변경될 상태의 start, 이전 상태의 end 수행
void Player::ChangeState(PlayerState _State)
{
	m_NextState = _State;
	m_PrevState = m_CurState;
	m_CurState = m_NextState;

	// start 
	switch (m_NextState)
	{
	case PlayerState::IDLE:
		IdleStart();
		break;
	case PlayerState::IDLETORUN:
		IdleToRunStart();
		break;
	case PlayerState::MOVE:
		MoveStart();
		break;
	case PlayerState::ROLL:
		RollStart();
		break;
	case PlayerState::JUMP:
		JumpStart();
		break;
	case PlayerState::SLASH:
		SlashStart();
		break;
	case PlayerState::CROUCH:
		CrouchStart();
		break;
	case PlayerState::FLIP:
		FlipStart();
		break;
	case PlayerState::FALL:
		FallStart();
		break;
	case PlayerState::RIGHTWALL:
		RightWallStart();
		break;
	case PlayerState::LEFTWALL:
		LeftWallStart();
		break;
	case PlayerState::DEATH:
		DeathStart();
		break;
	}

	// 이전 state의 end 
	switch (m_PrevState)
	{
	case PlayerState::IDLE:
		IdleEnd();
		break;
	case PlayerState::IDLETORUN:
		IdleToRunEnd();
		break;
	case PlayerState::MOVE:
		MoveEnd();
		break;
	case PlayerState::ROLL:
		RollEnd();
		break;
	case PlayerState::JUMP:
		JumpEnd();
		break;
	case PlayerState::SLASH:
		SlashEnd();
		break;
	case PlayerState::CROUCH:
		CrouchEnd();
		break;
	case PlayerState::FLIP:
		FlipEnd();
		break;
	case PlayerState::FALL:
		FallEnd();
		break;
	case PlayerState::RIGHTWALL:
		RightWallEnd();
		break;
	case PlayerState::LEFTWALL:
		LeftWallEnd();
		break;
	case PlayerState::DEATH:
		DeathEnd();
		break;
	}
}

void Player::IdleStart()
{
	// 이전 스테이트가 데스라면 스테이지 미클리어,
	// 난무조건 정방향 ( 오른쪽 ) 
	if (PlayerState::DEATH == m_PrevState)
	{
		GetTransform()->SetLocalPositiveScaleX();
	}

	m_Render->ChangeAnimation("player_idle");

	// 이전 상태가 점프였는데, Idle로 변경 되었다면 내가 땅이라는 뜻이기 때문에 착지이펙트 생성
	if (PlayerState::JUMP == m_PrevState)
	{
		GetLevel()->CreateActor<LandEffect>();
	}
}

void Player::IdleUpdate(float _DeltaTime)
{
	// 공격, 마우스 좌클릭 
	if (true == GameEngineInput::IsDown("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}

	// 점프키 , w
	if (true == GameEngineInput::IsDown("player_jump"))
	{
		ChangeState(PlayerState::JUMP);
		return;
	}

	// 크라우치 , s
	if (true == GameEngineInput::IsPress("player_crouch"))
	{
		ChangeState(PlayerState::CROUCH);
		return;
	}

	// 우측이동 , d
	if (true == GameEngineInput::IsPress("player_right_move"))
	{
		ChangeState(PlayerState::IDLETORUN); 
		return;
	}

	// 좌측이동 , a 
	else if (true == GameEngineInput::IsPress("player_left_move"))
	{
		ChangeState(PlayerState::IDLETORUN);
		return;
	}
}

void Player::IdleEnd()
{
}

void Player::IdleToRunStart()
{
	m_Render->ChangeAnimation("player_idle_to_run");
}

void Player::IdleToRunUpdate(float _DeltaTime)
{
	if (true == m_Render->IsAnimationEnd())
	{
		// 애니메이션이 종료된 시점에 왼쪽키나 오른쪽키가 눌려있다면
		// 무브 상태로 전환 
		if (true == GameEngineInput::IsPress("player_right_move") ||
			true == GameEngineInput::IsPress("player_left_move"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}

		// 눌려 있지 않다면, 다시 IDLE 
		ChangeState(PlayerState::IDLE);
		return;
	}

	if (true == GameEngineInput::IsDown("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}

	if (true == GameEngineInput::IsPress("player_jump"))
	{
		ChangeState(PlayerState::JUMP);
		return;
	}

	// 내위치의 픽셀체크를 해서, 내 위치가 흰색 픽셀 일때만 이동이 가능하다. 
	if (true == GameEngineInput::IsPress("player_right_move"))
	{
		if (true == GameEngineInput::IsPress("player_left_move"))
		{
			return;
		}

		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			DirCheck();
			GetTransform()->AddLocalPosition(float4::Right * m_StartMoveSpeed * _DeltaTime);
		}
		return;
	}

	if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
	{
		if (true == GameEngineInput::IsPress("player_left_move"))
		{
			
			if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition() + float4{-1,0}))
			{
				
				if (true == GameEngineInput::IsPress("player_right_move"))
				{
					return;
				}

				DirCheck();
				GetTransform()->AddLocalPosition(float4::Left * m_StartMoveSpeed * _DeltaTime);
				return;
				
			}
		}
	}

	
}

void Player::IdleToRunEnd()
{
}

void Player::MoveStart()
{
	GetLevel()->CreateActor<DashEffect>(static_cast<int>(RenderOrder::PLAYER_EFFECT));
	m_Render->ChangeAnimation("player_run");
}

void Player::MoveUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}

	if (true == GameEngineInput::IsDown("player_jump"))
	{
		ChangeState(PlayerState::JUMP);
		return;
	}

	if (true == GameEngineInput::IsDown("player_crouch"))
	{
		ChangeState(PlayerState::CROUCH);
		return;
	}

	if (false == GameEngineInput::IsPress("player_left_move") && 
		false == GameEngineInput::IsPress("player_right_move"))
	{
		ChangeState(PlayerState::IDLE);
		return;
	}

	// 내가 오른쪽으로 이동하려고해
	if (true == GameEngineInput::IsPress("player_right_Move"))
	{
		if (true == GameEngineInput::IsDown("player_left_move"))
		{
			return;
		}

		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			DirCheck();
			GetTransform()->AddLocalPosition(float4::Right * m_MoveSpeed * _DeltaTime);
			return;
		}
	}

	if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
	{
		if (true == GameEngineInput::IsPress("player_left_move"))
		{

			if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition() + float4{ -5,0 }))
			{

				if (true == GameEngineInput::IsPress("player_right_move"))
				{
					return;
				}

				DirCheck();
				GetTransform()->AddLocalPosition(float4::Left * m_StartMoveSpeed * _DeltaTime);
				return;

			}
		}
	}
}

void Player::MoveEnd()
{
}

void Player::SlashStart()
{
	m_AttackPos = Cursor::MainCursor->GetTransform()->GetLocalPosition();
	m_Render->ChangeAnimation("player_attack");
	GetLevel()->CreateActor<SlashEffect>(static_cast<int>(RenderOrder::PLAYER_EFFECT));
}

// 마우스찍은 위치에 도착해서 오래 머물경우 애니메이션 버그있음. 
void Player::SlashUpdate(float _DeltaTime)
{
	// fall에서는 어차피 내가 땅이라면 알아서 아이들로 바꾼다. 
	if (true == m_Render->IsAnimationEnd())
	{
		ChangeState(PlayerState::FALL);
		return;
	}

	// 내 머리통 픽셀이 검은색이라면
	if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Top->GetTransform()->GetWorldPosition()))
	{
		ChangeState(PlayerState::FALL);
		return;
	}

	float4 MyPos = GetTransform()->GetWorldPosition();

	// 공격포지션이 내오른쪽이면 정위치 
	if (MyPos.x <= m_AttackPos.x)
	{
		//m_Direction = true;
		GetTransform()->SetLocalPositiveScaleX();
	}
	// 내 왼쪽이면 반대 
	else if (MyPos.x > m_AttackPos.x)
	{
		//	m_Direction = false;
		GetTransform()->SetLocalNegativeScaleX();
	}
	// 공격 방향을 구한다. 
	float4 MoveDir = m_AttackPos - MyPos;
	float4 MoveDirOrigin = MoveDir;
	MoveDir.Normalize();
	MoveDirOrigin.Normalize();

	// 내 왼쪽이나 오른쪽이 벽인 상태에서
	// 내가 벽쪽으로 공격지점을 찍는다면. 
	if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()) ||
		PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Left->GetTransform()->GetWorldPosition()))
	{
		if (m_AttackPos.x < MyPos.x)
		{
			MoveDir.x = 0.0f;
		}

		// 공격위치가 나보다 낮다면 y축은 움직이지 않고 x축만 움직인다. 
		if (m_AttackPos.y < MyPos.y)
		{
			MoveDir.y = 0.0f;
		}
	}

	// 업데이트중 만약 내 아래가 검은색 픽셀이라면 바닥에 닿았다는 것. y축은 움직이지 않는다.
	if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom_Down->GetTransform()->GetWorldPosition()))
	{
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()) &&
			PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Left->GetTransform()->GetWorldPosition()))
		{
			// 공격위치가 나보다 높다면 x축은 움직이지 않고 y축만 움직인다. 
			if (m_AttackPos.y < MyPos.y)
			{
				MoveDirOrigin.y = 0.0f;
			}

			GetTransform()->AddLocalPosition(float4{ MoveDir.x * 1.5f , MoveDirOrigin.y * 1.2f } *m_MoveSpeed * _DeltaTime);
			return;
		}
	}

	GetTransform()->AddLocalPosition(float4{ MoveDir.x * 1.5f , MoveDir.y * 1.2f } *m_MoveSpeed * _DeltaTime);
}

// 공격이 종료되면 공격위치를 초기화 
void Player::SlashEnd()
{
	m_AttackPos = { 0 , 0 };
}


void Player::JumpStart()
{
	// 점프 상태가 false 일 경우 
	if (false == m_IsJumping)
	{
		// true 로 만들어준다. 
		m_IsJumping = true;
		m_CurrentVerticalVelocity = m_JumpPower;
	}

	m_Render->ChangeAnimation("player_jump");
	GetLevel()->CreateActor<JumpEffect>(static_cast<int>(RenderOrder::PLAYER_EFFECT));
}

void Player::JumpUpdate(float _DeltaTime)
{
	// 점프
	GetTransform()->AddLocalPosition(float4::Up * m_CurrentVerticalVelocity * _DeltaTime);
	// 점프의 힘에 중력을 더해준다. 
	m_CurrentVerticalVelocity += -m_GravityPower * _DeltaTime;

	// 만약 점프 상태일 때 내 아래픽셀이 검은색이라면 땅인거야 이제. 
	if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom_Down->GetTransform()->GetWorldPosition()))
	{
		// 값 초기화 후 아이들로 변경
		m_CurrentVerticalVelocity = 0.0f;
		ChangeState(PlayerState::IDLE);
		return;
	}
	
	if (true == GameEngineInput::IsPress("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}

	if (true == GameEngineInput::IsPress("player_crouch"))
	{
		ChangeState(PlayerState::FALL);
		return;
	}

	// 상단픽셀 부딪히면 ㅇㅇ 
	if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Top->GetTransform()->GetWorldPosition()))
	{
		ChangeState(PlayerState::FALL);
		return;
	}
	
	if (true == GameEngineInput::IsPress("player_right_Move"))
	{
		// true 이면 맵 밖인걸로
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			m_Direction = true;
			GetTransform()->SetLocalPositiveScaleX();
			GetTransform()->AddLocalPosition(float4::Right * m_MoveSpeed * _DeltaTime);
			return;
		}
	}

	else if (true == GameEngineInput::IsPress("player_left_Move"))
	{
		// true 이면 맵 밖인걸로
		if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			m_Direction = false;
			GetTransform()->SetLocalNegativeScaleX();
			GetTransform()->AddLocalPosition(float4::Left * m_MoveSpeed * _DeltaTime);
			return;
		}
	}
}

void Player::JumpEnd()
{
	m_CurrentVerticalVelocity = 0.0f;
	m_IsJumping = false;
}

void Player::CrouchStart()
{
	m_Render->ChangeAnimation("player_crouch");
}

void Player::CrouchUpdate(float _DeltaTime)
{
	if (false == GameEngineInput::IsPress("player_crouch"))
	{
		ChangeState(PlayerState::IDLE);
		return;
	}

	if (true == GameEngineInput::IsDown("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}
	
	// 크라우치 상태에서 우측키 Down, 
	if (true == GameEngineInput::IsDown("player_right_move"))
	{
		// 내 오른쪽이 벽이라면 return
		if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			return;
		}

		ChangeState(PlayerState::ROLL);
		return;
	}

	else if (true == GameEngineInput::IsDown("player_left_move"))
	{
		if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			return;
		}

		ChangeState(PlayerState::ROLL);
		return;
	}
}

void Player::CrouchEnd()
{
}

// 얘부터 다시 
void Player::RollStart()
{
	m_Render->ChangeAnimation("player_roll");
}

void Player::RollUpdate(float _DeltaTime)
{
	// 회피중 공격키를 누른다면 슬래시로 전환 
	if (true == GameEngineInput::IsDown("player_slash"))
	{
		ChangeState(PlayerState::SLASH);
		return;
	}

	if (true == m_RightRoll && false == m_LeftRoll)
	{
		// 플립 애니메이션이 종료 되었다면
		if (true == m_Render->IsAnimationEnd())
		{
			// 내위치가 땅이라면 
			if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom->GetTransform()->GetWorldPosition()) &&
				PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom_Down->GetTransform()->GetWorldPosition()))
			{
				// 땅인데 아래키가 눌려있다면 
				if (true == GameEngineInput::IsPress("player_crouch"))
				{
					ChangeState(PlayerState::CROUCH);
					return;
				}

				// 키가 눌려있지 않다면 IDLE로 전환
				ChangeState(PlayerState::IDLE);
				return;
			}
		}

		if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			ChangeState(PlayerState::IDLE);
			return;
		}

		GetTransform()->AddLocalPosition(float4::Right * m_RollSpeed * _DeltaTime);
	}

	else if (true == m_LeftRoll)
	{
		// 플립 애니메이션이 종료 되었다면
		if (true == m_Render->IsAnimationEnd())
		{
			// 내위치가 땅이라면 
			if (PixelCollider::g_WhitePixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom->GetTransform()->GetWorldPosition()) &&
				PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Bottom_Down->GetTransform()->GetWorldPosition()))
			{
				// 땅인데 아래키가 눌려있다면 
				if (true == GameEngineInput::IsPress("player_crouch"))
				{
					ChangeState(PlayerState::CROUCH);
					return;
				}

				// 그게 아니라면 아이들 
				ChangeState(PlayerState::IDLE);
				return;
			}
		}

		if (PixelCollider::g_BlackPixel == PixelCollider::PixelCol->PixelCollision(m_DebugRender_Right->GetTransform()->GetWorldPosition()))
		{
			ChangeState(PlayerState::IDLE);
			return;
		}

		GetTransform()->AddLocalPosition(float4::Left * m_RollSpeed * _DeltaTime);
	}

	// 애니메이션이 종료되면 
	if (true == m_Render->IsAnimationEnd())
	{
		if (true == GameEngineInput::IsPress("player_crouch"))
		{
			ChangeState(PlayerState::CROUCH);
			return;
		}

		ChangeState(PlayerState::IDLE);
		return;
	}

	if (true == GameEngineInput::IsPress("player_right_Move"))
	{
		// 애니메이션 방향보정
		m_Direction = true;
		GetTransform()->SetLocalPositiveScaleX();
		m_RightRoll = true;

		return;
	}

	else if (true == GameEngineInput::IsPress("player_left_Move") && false == m_RightRoll)
	{
		m_Direction = false;
		GetTransform()->SetLocalNegativeScaleX();
		m_LeftRoll = true;

		return;
	}
}

void Player::RollEnd()
{
	m_RightRoll = false;
	m_LeftRoll = false;

	if (true != GameEngineInput::IsPress("player_crouch"))
	{
		if (true == GameEngineInput::IsPress("player_right_move") || true == GameEngineInput::IsPress("player_left_move"))
		{
			ChangeState(PlayerState::MOVE);
			return;
		}
	}
}

// 플립부터 다시시작.
void Player::FlipStart()
{
	
}

// 플립상태에서는 무적이기 때문에 충돌체 off 처리 예정
// 좌우측 픽셀검사로 벽이면 이동 ㄴㄴ 추가해야함 
// 이녀석이 플립이 아니라 롤이었구나 
void Player::FlipUpdate(float _DeltaTime)
{
	
}

void Player::FlipEnd()
{

}

void Player::FallStart()
{
	m_Render->ChangeAnimation("player_fall");
}

// 왼쪽오른쪽 픽셀체크 추가해야함 
void Player::FallUpdate(float _DeltaTime)
{
	// 현재 땅이라면
	if (true == PixelCollider::PixelCol->GroundCheck(this))
	{
		ChangeState(PlayerState::IDLE);
		return;
	}


	// 땅이아니라면
	else if (false == PixelCollider::PixelCol->GroundCheck(this))
	{
		GetTransform()->AddLocalPosition(float4{ 0 , -1 } *300.0f * _DeltaTime);
	}

	// 이렇게 해주는게 무빙이 안이상함 
	if (true == GameEngineInput::IsPress("player_crouch"))
	{
		if (true == GameEngineInput::IsDown("player_slash"))
		{
			ChangeState(PlayerState::SLASH);
			return;
		}

		GetTransform()->AddLocalPosition(float4::Down * m_FallPower * _DeltaTime);

		// 현재 땅이라면
		if (true == PixelCollider::PixelCol->GroundCheck(this))
		{
			ChangeState(PlayerState::IDLE);
			return;
		}

		// 땅이아니라면
		else if (false == PixelCollider::PixelCol->GroundCheck(this))
		{
			GetTransform()->AddLocalPosition(float4{ 0 , -1 } *300.0f * _DeltaTime);
			return;
		}
	}

	if (true == GameEngineInput::IsPress("player_right_Move"))
	{
		if (false == PixelCollider::PixelCol->RightPixelCheck())
		{
			m_Direction = true;
			GetTransform()->SetLocalPositiveScaleX();
			GetTransform()->AddLocalPosition(float4::Right * m_MoveSpeed * _DeltaTime);
			return;
		}
	}

	else if (true == GameEngineInput::IsPress("player_left_Move"))
	{
		if (false == PixelCollider::PixelCol->LeftPixelCheck())
		{
			m_Direction = false;
			GetTransform()->SetLocalNegativeScaleX();
			GetTransform()->AddLocalPosition(float4::Left * m_MoveSpeed * _DeltaTime);
			return;
		}
	}
}

void Player::FallEnd()
{
	GetLevel()->CreateActor<LandEffect>();
}

void Player::RightWallStart()
{
	//m_Render->ChangeAnimation("player_wallslide");
}

void Player::RightWallUpdate(float _DeltaTime)
{
	/*if (true == PixelCollider::PixelCol->GroundCheck(this))
	{
		ChangeState(PlayerState::IDLE);
		return;
	}

	else
	{
		GetTransform()->AddLocalPosition(float4::Down * 200.0f * _DeltaTime);
		return;
	}*/
}

void Player::RightWallEnd()
{
}

void Player::LeftWallStart()
{
}

void Player::LeftWallUpdate(float _DeltaTime)
{
}

void Player::LeftWallEnd()
{
}

// 일단 보류
// 너무부자연스러움 
// 죽었을때 좀 예쁘게 날아가게 바꿔야함
void Player::DeathStart()
{
	DirCheck();
	m_Render->ChangeAnimation("player_die");
	m_Render->GetTransform()->AddLocalPosition({ 0 , -15.0f });
}

void Player::DeathUpdate(float _DeltaTime)
{
	if (true == m_Render->IsAnimationEnd())
	{
		return;
	}

	if (false == PixelCollider::PixelCol->GroundCheck(this))
	{
		if (true == PixelCollider::PixelCol->GroundCheck(this))
		{
			return;
		}

		GetTransform()->AddLocalPosition(float4::Down * 50.0f * _DeltaTime);
	}
}

void Player::DeathEnd()
{
	m_Render->GetTransform()->AddLocalPosition({ 0 , 15.0f });
}


#include "PrecompileHeader.h"
#include "HeadHunter_RifleEffect.h"

#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>

#include "Boss_HeadHunter.h"
#include "Player.h"
#include "PlaySupporter.h"

HeadHunter_RifleEffect::HeadHunter_RifleEffect()
{
}

HeadHunter_RifleEffect::~HeadHunter_RifleEffect()
{
}

void HeadHunter_RifleEffect::SetType(RifleEffectType _Type)
{
	m_Type = _Type;
	switch (m_Type)
	{
	case RifleEffectType::NORMAL:
		m_Render->SetAnimationStartEvent("headhunter_rifle_effect", static_cast<size_t>(2), std::bind(&HeadHunter_RifleEffect::RifleShotSoundPlay, this));
		m_Render->GetTransform()->AddLocalPosition(float4{ 0.0f , 0.0f , -10.0f});
		break;
	case RifleEffectType::SWEEP:
	{
		// 만약 이펙트가 스윕이라면 렌더러 위치를 내 렌더크기의 절반만큼 밀어 
		m_Render->GetTransform()->AddLocalPosition(float4{ 500.0f , 0.0f });
		m_Render->ChangeAnimation("headhunter_rifle_effect_sweep");
		m_Collision->GetTransform()->AddLocalPosition(float4{ 500.0f , 0.0f });
	}
		break;
	}
}

void HeadHunter_RifleEffect::ChangeLoopAnimation()
{
	m_Render->ChangeAnimation("headhunter_rifle_effect_sweep_loop");
}

void HeadHunter_RifleEffect::EffectDeath()
{
	ChangeState(EffectState::DEATH);
	m_Collision->Off();
	m_Render->Off();
	m_IsRecording = false;
}

void HeadHunter_RifleEffect::RifleShotSoundPlay()
{
	int RandomValue = CreateRandomValue(3);
	switch (RandomValue)
	{
	case 1:
		m_SoundPlayer = GameEngineSound::Play("headhunter_rifle_shot_01.wav");
		break;
	case 2:
		m_SoundPlayer = GameEngineSound::Play("headhunter_rifle_shot_02.wav");
		break;
	case 3:
		m_SoundPlayer = GameEngineSound::Play("headhunter_rifle_shot_03.wav");
		break;
	}
}

void HeadHunter_RifleEffect::Start()
{
	if (nullptr == GameEngineSprite::Find("headhunter_rifle_effect"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("katanazero_resources");
		Dir.Move("katanazero_resources");
		Dir.Move("Texture");
		Dir.Move("ClubLevel");
		Dir.Move("headhunter");

		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_rifle_effect").GetFullPath());
		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("headhunter_rifle_effect_trans").GetFullPath());
		std::vector<GameEngineFile> File = Dir.GetAllFile({ ".Png", });
	}

	m_Render = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::BOSS_EFFECT);
	m_Render->GetTransform()->SetLocalScale(float4{ 1000.0f , 2.0f, 1.0f });
	m_Render->GetTransform()->SetLocalPosition(float4{ 0.0f , 0.0f, -3.0f });

	m_Collision = CreateComponent<GameEngineCollision>(ColOrder::BOSS_ATTACK);
	m_Collision->GetTransform()->SetLocalScale(float4{ 1000.0f , 20.0f , 1.0f  });
	m_Collision->SetColType(ColType::OBBBOX3D);
	m_Collision->Off();
	
	{
		// 기본 라이플이펙트 
		std::vector<float> vFrameTime = std::vector<float>();
		vFrameTime.push_back(0.8f);
		vFrameTime.push_back(0.3f);
		vFrameTime.push_back(0.2f);

		AnimationParameter Para = {};
		Para.AnimationName = "headhunter_rifle_effect";
		Para.SpriteName = "headhunter_rifle_effect";
		Para.Start = 0;
		Para.End = 2;
		Para.FrameTime = vFrameTime;
		Para.Loop = false;
		Para.ScaleToTexture = false;

		m_Render->CreateAnimation(Para);
	}
	{
		// 스윕라이플 이펙트 
		std::vector<float> vFrameTime = std::vector<float>();
		vFrameTime.push_back(0.1f);
		vFrameTime.push_back(0.08f);
		vFrameTime.push_back(0.08f);

		AnimationParameter Para = {};
		Para.AnimationName = "headhunter_rifle_effect_sweep";
		Para.SpriteName = "headhunter_rifle_effect_trans";
		Para.Start = 0;
		Para.End = 2;
		Para.FrameTime = vFrameTime;
		Para.Loop = false;
		Para.ScaleToTexture = false;

		m_Render->CreateAnimation(Para);
	}
	{
		// 스윕라이플 이펙트 loop
		std::vector<float> vFrameTime = std::vector<float>();
		vFrameTime.push_back(0.15f);
		vFrameTime.push_back(0.15f);

		AnimationParameter Para = {};
		Para.AnimationName = "headhunter_rifle_effect_sweep_loop";
		Para.SpriteName = "headhunter_rifle_effect_trans";
		Para.Start = 1;
		Para.End = 2;
		Para.FrameTime = vFrameTime;
		Para.Loop = true;
		Para.ScaleToTexture = false;

		m_Render->CreateAnimation(Para);
	}
	
	m_Render->SetAnimationStartEvent("headhunter_rifle_effect_sweep", static_cast<size_t>(1), std::bind(&HeadHunter_RifleEffect::ChangeLoopAnimation, this));
	m_Render->ChangeAnimation("headhunter_rifle_effect");
	
	//m_Render->SetAnimationStartEvent("headhunter_rifle_effect", static_cast<size_t>(2), std::bind(&HeadHunter_RifleEffect::RifleShotSoundPlay, this));
}

void HeadHunter_RifleEffect::Update(float _DeltaTime)
{
	if (true == Player::MainPlayer->IsSkill())
	{
		m_SoundPlayer.SetPitch(0.5f);
	}
	else if (false == Player::MainPlayer->IsSkill())
	{
		m_SoundPlayer.SetPitch(1.0f);
	}

	m_RecordingFrame = !m_RecordingFrame;

	if (BaseLevel::LevelState::RECORDING_PROGRESS == GetReturnCastLevel()->GetCurState())
	{
		if (EffectState::RECORDING_PROGRESS != m_CurState)
		{
			ChangeState(EffectState::RECORDING_PROGRESS);
			return;
		}
	}

	if (EffectState::RECORDING_PROGRESS == m_CurState)
	{
		// 만약 좌클릭 입력시 바로 death 
		if (true == GameEngineInput::IsDown("EngineMouseLeft"))
		{
			this->Death();
			return;
		}

		Reverse(m_Render.get());

		// 역재생 함수 호출 후 , 나의 인포사이즈가 0 이라면 나를 death 
		if (0 == Infos.size())
		{
			this->Death();
		}

		return;
	}

	// 나의 스테이트가, 녹화진행중이 아니라면, 녹화 정보를 저장한다. 
	if (EffectState::RECORDING_PROGRESS != m_CurState)
	{
		if (true == m_RecordingFrame)
		{
			InfoSetting(m_Render.get());
		}
	}

	if (EffectState::DEATH == m_CurState)
	{
		return;
	}


	// 라이브타임으로 그냥 죽이자 
	if (RifleEffectType::CEILING_FIRE == m_Type)
	{
		if (true == m_Render->IsAnimationEnd())
		{
			

			EffectDeath();
		

			// this->Death();
			return;
		}

		if (1 == m_Render->GetCurrentFrame())
		{
			m_Render->GetTransform()->SetLocalScale(float4{ 1000.0f , 20.0f, 1.0f });
			if (EffectState::DEATH != m_CurState)
			{
				m_Collision->On();
			}
		}

		std::shared_ptr<GameEngineCollision> Col = m_Collision->Collision(ColOrder::PLAYER, ColType::OBBBOX2D, ColType::OBBBOX2D);
		if (nullptr != Col && false == Player::MainPlayer->IsInvincibility())
		{
			if (PlayerState::EXPLOSION_DEATH != Player::MainPlayer->GetCurState())
			{
				PlaySupporter::MainSupporter->CameraShakeOn();
				Player::MainPlayer->CreateExplosionEffect();
				Player::MainPlayer->ChangeState(PlayerState::EXPLOSION_DEATH);
			}
		}

		return;
	}


	if (RifleEffectType::SWEEP == m_Type)
	{
		// 만약, 라이플 타입이 sweep 이라면, 내가 애니메이션프레임이 1일 경우, 렌더러의 y축 크기를 증가시킨다. 
		if (1 == m_Render->GetCurrentFrame())
		{
			m_Render->GetTransform()->SetLocalScale(float4{ 1000.0f , 20.0f, 1.0f });

			if (EffectState::DEATH != m_CurState)
			{
				m_Collision->On();
			}
		}

		std::shared_ptr<GameEngineCollision> Col = m_Collision->Collision(ColOrder::PLAYER, ColType::OBBBOX2D, ColType::OBBBOX2D);
		if (nullptr != Col && false == Player::MainPlayer->IsInvincibility())
		{
			if (PlayerState::EXPLOSION_DEATH != Player::MainPlayer->GetCurState())
			{
				Player::MainPlayer->CreateExplosionEffect();
				Player::MainPlayer->ChangeState(PlayerState::EXPLOSION_DEATH);
			}
		}

		return;
	}

	// 노말이펙트일 경우 
	if (RifleEffectType::NORMAL == m_Type)
	{
		BossState CurState = m_Actor->GetCurState();
		HeadHunter_RifleEffect* Effect = this;
		if (true == m_Render->IsAnimationEnd() && EffectState::DEATH != m_CurState)
		{
			std::shared_ptr<GameEngineCollision> Col = m_Collision->Collision(ColOrder::PLAYER, ColType::OBBBOX2D, ColType::OBBBOX2D);
			if (nullptr != Col && false == Player::MainPlayer->IsInvincibility())
			{
				if (PlayerState::EXPLOSION_DEATH != Player::MainPlayer->GetCurState())
				{
					Player::MainPlayer->CreateExplosionEffect();
					Player::MainPlayer->ChangeState(PlayerState::EXPLOSION_DEATH);
				}
			}

			EffectDeath();
			/*this->Death();
			if (nullptr != m_Render)
			{
				m_Render = nullptr;
				m_Actor = nullptr;
			}*/
			return;
		}
	}

	size_t Frame = m_Render->GetCurrentFrame();
	switch (Frame)
	{
	case 0:
		m_Render->ColorOptionValue.MulColor.a -= _DeltaTime;
		break;
	case 1:
		m_Render->ColorOptionValue.MulColor.a = 0.0f;
		break;
	default:
		m_Render->ColorOptionValue.MulColor.a = 1.0f;
		m_Render->GetTransform()->SetLocalScale(float4{ 1000.0f , 20.0f });
		m_Collision->On();
		break;
	}
	
}

void HeadHunter_RifleEffect::Render(float _DeltaTime)
{
}

#include "PrecompileHeader.h"
#include "GunSpark_Effect.h"

#include <GameEngineCore/GameEngineSpriteRenderer.h>

GunSpark_Effect::GunSpark_Effect()
{
}

GunSpark_Effect::~GunSpark_Effect()
{
}

void GunSpark_Effect::Start()
{
	m_Render = CreateComponent<GameEngineSpriteRenderer>(RenderOrder::EFFECT);
	m_Render->GetTransform()->SetLocalPosition(float4{ 0.0f, 0.0f, -2.0f });


	if (nullptr == GameEngineSprite::Find("gunspark_effect"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToDirectory("katanazero_resources");
		Dir.Move("katanazero_resources");
		Dir.Move("Texture");
		Dir.Move("ClubLevel");
		Dir.Move("effect");

		GameEngineSprite::LoadFolder(Dir.GetPlusFileName("gunspark_effect").GetFullPath());
		std::vector<GameEngineFile> File = Dir.GetAllFile({ ".Png", });
	}
	
	m_Render->CreateAnimation({ .AnimationName = "gunspark_effect", .SpriteName = "gunspark_effect", .Start = 0, .End = 7 ,
								  .FrameInter = 0.1f , .Loop = false , .ScaleToTexture = true });
	m_Render->ChangeAnimation("gunspark_effect");

	Set_Recording_StartFrame();
}

void GunSpark_Effect::Update(float _DeltaTime)
{
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

	if (BaseLevel::LevelState::RECORDING_PROGRESS_FORWARD == GetReturnCastLevel()->GetCurState())
	{
		if (EffectState::RECORDING_PROGRESS_FORWARD != m_CurState)
		{
			SetMaxIndex();
			ChangeState(EffectState::RECORDING_PROGRESS_FORWARD);
			return;
		}
	}

	if (EffectState::RECORDING_PROGRESS_FORWARD == m_CurState)
	{
		// 만약 좌클릭 입력시 바로 death 
		if (true == GameEngineInput::IsDown("EngineMouseLeft"))
		{
			this->Death();
			return;
		}

		Play_RecordingForward(m_Render.get());
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

	if (true == m_Render->IsAnimationEnd())
	{
		ChangeState(EffectState::DEATH);
		m_Render->Off();
		m_IsRecording = false;
	}
}

void GunSpark_Effect::Render(float _DeltaTime)
{
}

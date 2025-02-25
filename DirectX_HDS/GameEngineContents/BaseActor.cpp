#include "PrecompileHeader.h"
#include "BaseActor.h"
#include "BaseLevel.h"

#include <GameEngineBase/GameEngineRandom.h>
#include "Player.h"
#include "BloodEffect.h"

BaseActor::BaseActor()
{
}

BaseActor::~BaseActor()
{
}

const int BaseActor::CreateRandomValue(const int _MAX)
{
	int RandomValue = GameEngineRandom::MainRandom.RandomInt(1, _MAX);
	return RandomValue;
}

BaseLevel* BaseActor::GetReturnCastLevel() const
{
	BaseLevel* Level = dynamic_cast<BaseLevel*>(GetLevel());
	
	if (nullptr == Level)
	{
		MsgAssert("Level 이 nullptr 입니다.");
		return nullptr;
	}

	return Level;
}

void BaseActor::ForcedReset()
{
	Infos.clear();
	m_Recording_Complete = true;
}

BaseLevel::LevelState BaseActor::GetLevelState()
{
	BaseLevel* CurLevel = GetReturnCastLevel();
	return CurLevel->GetCurState();
}

// 역재생함수 
void BaseActor::Reverse(GameEngineSpriteRenderer* _Renderer)
{
	if (nullptr == _Renderer)
	{
		MsgAssert("렌더러가 nullptr 입니다.");
		return;
	}
	BaseLevel::LevelState CurLevelState = GetLevelState();

	// 만약 인포가 비어있다면 return; 
	if (true == Infos.empty() || BaseLevel::LevelState::PLAY == CurLevelState)
	{
		// 여기서 bool 값 하나두고, 녹화완료 체크를 하고,
		// true로 바뀌었다면 외부 녹화 state 에서 상태를 종료시킨다. 
		m_Recording_Complete = true;
		return;
	}

	// 뒤쪽 데이터 부터 받아온다. 
	switch (CurLevelState)
	{
	case BaseLevel::LevelState::RECORDING_PROGRESS:
	{
		ReverseInfo& Info = *(Infos.rbegin());

		if (true == Info.IsRecording)
		{
			_Renderer->On();
		}

		else if (false == Info.IsRecording)
		{
			Infos.pop_back();
			return;
		}

		this->GetTransform()->SetTransformData(Info.ActorData);
		_Renderer->GetTransform()->SetTransformData(Info.RendererData);
		_Renderer->SetTexture(Info.InfoData.Texture->GetName());
		_Renderer->SetAtlasData(Info.InfoData.CutData);

		// 데이터 세팅 후 삭제 
		Infos.pop_back();
	}
		break;
	case BaseLevel::LevelState::RECORDING_PROGRESS_FORWARD:
	{
		// 만약 인덱스 값이 벡터의 사이즈보다 같거나 커졌다면 레코딩완료 true 
		if (m_CurrentIdx == m_MaxIdx)
		{
			m_Recording_Complete = true;
			m_CurrentIdx = 0;
			return;
		}

		// 벡터의 Idx번쨰 데이터를 받아온다. 
		ReverseInfo& Info = Infos[m_CurrentIdx];

		if (true == Info.IsRecording)
		{
			_Renderer->On();
		}

		else if (false == Info.IsRecording)
		{
			this->Death();
			return;
		}

		// 모든 데이터를 세팅해준다. 
		this->GetTransform()->SetTransformData(Info.ActorData);
		_Renderer->GetTransform()->SetTransformData(Info.RendererData);
		_Renderer->SetTexture(Info.InfoData.Texture->GetName());
		_Renderer->SetAtlasData(Info.InfoData.CutData);

		//// 데이터 세팅 후 삭제 
		//Infos.pop_front();

		// 원래는 삭제인데, 삭제하지 않고 인덱스값만 ++ 시켜준다.
		++m_CurrentIdx;

		
	}
		break;
	default:
		break;
	}
	
}

// 자료구조 벡터로 바꾸고
// 플레이어 프레임값저장하고
// 이펙트 액터들 정방향재생시 플레이어 프레임값 받아오기
// 인덱스값도 필요

void BaseActor::InfoSetting(GameEngineSpriteRenderer* _Renderer)
{
	if (40000 <= Infos.size())
	{
		return;
	}

	SpriteInfo Info;

	GameEngineTextureSetter* Tex = _Renderer->GetShaderResHelper().GetTextureSetter("DiffuseTex");
	Info.Texture = Tex->Res;
	Info.CutData = _Renderer->GetAtlasData();

	Infos.push_back({ _Renderer->GetActor()->GetTransform()->GetTransDataRef(), _Renderer->GetTransform()->GetTransDataRef(), Info , m_IsRecording });
}

void BaseActor::Update(float _DeltaTime)
{
	
}

void BaseActor::Set_Recording_StartFrame()
{
	// 감소시킬 프레임값 
	m_Recording_StartFrame = Player::MainPlayer->GetReverseFrameCount();

	// 맥스값
	m_Recording_StartFrameMax = m_Recording_StartFrame;
}

void BaseActor::Play_RecordingForward(GameEngineSpriteRenderer* _Render)
{
	if (nullptr == _Render)
	{
		MsgAssert("렌더러가 nullptr 입니다.");
		return;
	}


	// 정방향일때 저장되어있는 프레임값을 --
	if (0 < m_Recording_StartFrame)
	{
		--m_Recording_StartFrame;
	}

	if (0 == m_Recording_StartFrame)
	{
		++m_Recording_StartFrame_Check;
		Reverse(_Render);
	}
}

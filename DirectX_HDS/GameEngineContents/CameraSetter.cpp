#include "PrecompileHeader.h"
#include "CameraSetter.h"

#include <GameEnginePlatform/GameEngineWindow.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>

#include "Player.h"
#include "BaseLevel.h"
#include "Battery.h"

CameraSetter* CameraSetter::MainCamSetter = nullptr;
std::vector<float4> CameraSetter::m_CameraPivots = std::vector<float4>();

CameraSetter::CameraSetter()
{
	MainCamSetter = this;
}

CameraSetter::~CameraSetter()
{
}

void CameraSetter::Update(float _DeltaTime)
{
}

void CameraSetter::CameraSetting()
{
	GameEngineLevel* CurLevel = GameEngineCore::GetCurLevel().get();
	BaseLevel* CastLevel = dynamic_cast<BaseLevel*>(CurLevel);

	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다. ");
		return;
	}

	LevelType Type = CastLevel->GetLevelType();

	switch (Type)
	{
	case LevelType::NONE:
	{
		MsgAssert("현재 레벨타입이 NONE 입니다. Level 의 Start에서 LevelType를 세팅하세요.");
		return;
	}
		break;
	case LevelType::CLUBMAP0:
	{
		if (nullptr != Player::MainPlayer)
		{
			CastLevel->GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPivots[static_cast<int>(LevelType::CLUBMAP0)]);
		}
		
	}
		break;
	case LevelType::CLUBMAP1:
	{
		if (nullptr != Player::MainPlayer)
		{
			CastLevel->GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPivots[static_cast<int>(LevelType::CLUBMAP1)]);
		}
	}
		break;
	case LevelType::CLUBMAP2:
	{
		if (nullptr != Player::MainPlayer)
		{
			CastLevel->GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPivots[static_cast<int>(LevelType::CLUBMAP2)]);
		}
	}
		break;
	case LevelType::CLUBMAP3:
	{
		if (nullptr != Player::MainPlayer)
		{
			CastLevel->GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPivots[static_cast<int>(LevelType::CLUBMAP3)]);
		}
	}
		break;
	case LevelType::CLUBMAP4:
	{
		if (nullptr != Player::MainPlayer)
		{
			CastLevel->GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPivots[static_cast<int>(LevelType::CLUBMAP4)]);
		}
	}
		break;
	}
}

bool CameraSetter::PlayerDeathCheck()
{
	if (nullptr == Player::MainPlayer)
	{
		return false;
	}

	if (PlayerState::DEATH == Player::MainPlayer->GetCurState())
	{
		return true;
	}

	return false;
}

void CameraSetter::Reset()
{
	CameraSetting();
}

void CameraSetter::Start()
{
	// 맵별 카메라세팅
	if (0 == m_CameraPivots.size())
	{
		m_CameraPivots.reserve(8);

		m_CameraPivots.push_back(float4{ -360 , 34 , 0 });    // 0 
		m_CameraPivots.push_back(float4{ 0 , 0 , 0 });		  // 1 
		m_CameraPivots.push_back(float4{ -335 , -250 , 0 });  // 2
		m_CameraPivots.push_back(float4{ -72 , 356 , 0 });		  // 3		여기부터 설정
		m_CameraPivots.push_back(float4{ 0 , 0 , 0 });        // 4
		m_CameraPivots.push_back(float4{ 0 , 0 , 0 });		  // 5 
	}

}

#include "PrecompileHeader.h"
#include "ColEventObj.h"

#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineCore.h>

#include "BaseLevel.h"
#include "YouCanDoThis.h"

ColEventObj::ColEventObj()
{
}

ColEventObj::~ColEventObj()
{
}

void ColEventObj::Start()
{
	ComponentSetting();
}

void ColEventObj::Update(float _DeltaTime)
{
	DebugUpdate();
	CollisionUpdate(_DeltaTime);



}

void ColEventObj::Render(float _DeltaTime)
{
}

void ColEventObj::DebugUpdate()
{
}

void ColEventObj::CollisionUpdate(float _DeltaTime)
{
	std::shared_ptr<GameEngineCollision> PlayerCol = m_Collision->Collision(ColOrder::PLAYER, ColType::AABBBOX2D, ColType::AABBBOX2D);

	// nullptr이 아니라면 플레이어와 충돌한거고 
	if (nullptr != PlayerCol)
	{
		// 얘는 클리어 상태에서만 생성되니까
		BaseLevel* CurLevel = GetReturnCastLevel();
		LevelType CurLevelType = CurLevel->GetLevelType();

		// 바로 체인지레벨을 하는게 아니라 클리어 화면 액터를 먼저띄우고
		// 일정 시간 뒤에 레벨체인지를 하는걸로 여기서 액터를 만들어 
		GetLevel()->CreateActor<YouCanDoThis>();
		m_Collision->Off();
	}
}

void ColEventObj::ComponentSetting()
{
	m_Collision = CreateComponent<GameEngineCollision>(ColOrder::STAGE_COL_OBJ);
	m_Collision->GetTransform()->SetLocalScale({ 50.0f, 200.0f });
	m_Collision->GetTransform()->AddLocalPosition({ 0.0f, 75.0f });
	m_Collision->SetColType(ColType::OBBBOX3D);
	m_Collision->DebugOn();
}

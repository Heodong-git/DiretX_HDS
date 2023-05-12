#include "PrecompileHeader.h"
#include "ContentsCore.h"
#include <GameEngineCore\GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/GameEngineGUI.h>
#include "PlayLevel.h"
#include "CTestLevel.h"
#include "PrecompileHeader.h"
#include "TitleLevel.h"
#include "CTutorialLevel.h"
#include "ClubLevel.h"
#include "ClubBossLevel.h"
#include "TitleEditer.h"
#include "StageEditer.h"
#include "PlayManager.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::ContentsResourcesCreate()
{
	// 텍스쳐 로드만 각각 필요한 레벨에서. 레벨체인지하게 될 경우 로드한 리소스 삭제까지. 
}

// 게임이 실행되면 동작해야할 함수를 호출
void ContentsCore::GameStart()
{
	ContentsResourcesCreate();

	//GameEngineGUI::GUIWindowCreate<GameEngineCoreWindow>("CoreWindow");

	// 레벨생성 후 레벨체인지 , 생성시 이름을 넣어주지 않는다면
	// 클래스 이름과 동일한 이름으로 레벨 생성
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	// GameEngineCore::CreateLevel<CTutorialLevel>("TutorialLevel");
	GameEngineCore::CreateLevel<ClubLevel>("ClubLevel");
	GameEngineCore::CreateLevel<ClubBossLevel>("ClubBossLevel");

	//GameEngineCore::CreateLevel<CTestLevel>();
	GameEngineCore::ChangeLevel("ClubLevel");

	// gui
	{
		std::shared_ptr<GameEngineGUIWindow> Window = GameEngineGUI::GUIWindowCreate<TitleEditer>("Title_Editer");
		Window->Off();
	}
	{
		std::shared_ptr<GameEngineGUIWindow> Window = GameEngineGUI::GUIWindowCreate<StageEditer>("Debug_State");
		Window->Off();
	}
}

// 게임이 종료되면 동작해야할 함수를 호출
void ContentsCore::GameEnd()
{
	// 
	PlayManager::DestroyInst();
}
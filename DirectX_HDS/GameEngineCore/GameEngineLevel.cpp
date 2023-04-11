#include "PrecompileHeader.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineCamera.h"


GameEngineLevel::GameEngineLevel()
{
	// 레벨은 생성될 때 반드시 카메라를 생성
	MainCamera = CreateActor<GameEngineCamera>();
}

GameEngineLevel::~GameEngineLevel()
{
}

void GameEngineLevel::Start()
{


}


void GameEngineLevel::Update(float _DeltaTime)
{
	// 맵에 저장되어 있기 때문에 2중 for문으로 처리
	// 맵 탐색 후 리스트탐색 
	for (std::pair<int, std::list<std::shared_ptr<GameEngineActor>>> OrderGroup : Actors)
	{
		std::list<std::shared_ptr<GameEngineActor>>& ActorList = OrderGroup.second;

		for (std::shared_ptr<GameEngineActor> Actor : ActorList)
		{
			Actor->Update(_DeltaTime);
			Actor->ComponentsUpdate(_DeltaTime);
		}
	}	

	/*std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator StartIter = Actors.begin();
	std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator EndIter = Actors.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		std::list<std::shared_ptr<GameEngineActor>> Actorlist = (*StartIter).second;
		
		std::list<std::shared_ptr<GameEngineActor>>::iterator ActorStartIter = Actorlist.begin();
		std::list<std::shared_ptr<GameEngineActor>>::iterator ActorEndIter = Actorlist.end();

		for (; ActorStartIter != ActorEndIter; ++ActorStartIter)
		{
			if (nullptr != ActorStartIter->get())
			{
				ActorStartIter->get()->Update(_DeltaTime);
			}
		}
	}*/
}

void GameEngineLevel::Render(float _DeltaTime)
{
	// 이건 나중에 만들어질 랜더러의 랜더가 다 끝나고 되는 랜더가 될겁니다.
	for (std::pair<int, std::list<std::shared_ptr<GameEngineActor>>> OrderGroup : Actors)
	{
		std::list<std::shared_ptr<GameEngineActor>>& ActorList = OrderGroup.second;

		for (std::shared_ptr<GameEngineActor> Actor : ActorList)
		{
			Actor->Render(_DeltaTime);
			Actor->ComponentsRender(_DeltaTime);
		}
	}

	//std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator StartIter = Actors.begin();
	//std::map<int, std::list<std::shared_ptr<GameEngineActor>>>::iterator EndIter = Actors.end();

	//for (; StartIter != EndIter; ++StartIter)
	//{
	//	std::list<std::shared_ptr<GameEngineActor>> Actorlist = (*StartIter).second;
	//	std::list<std::shared_ptr<GameEngineActor>>::iterator ListStartIter = Actorlist.begin();
	//	std::list<std::shared_ptr<GameEngineActor>>::iterator ListEndIter = Actorlist.end();

	//	for (; ListStartIter != ListEndIter; ++ListStartIter)
	//	{
	//		if (nullptr != ListStartIter->get())
	//		{
	//			ListStartIter->get()->Render(_DeltaTime);
	//		}
	//	}
	//}
}

void GameEngineLevel::ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Parent)
{
	_Actor->Level = this;
	_Actor->SetOrder(_Order);
	_Actor->Start();

	// 리스트에 추가
	Actors[_Order].push_back(_Actor);
}
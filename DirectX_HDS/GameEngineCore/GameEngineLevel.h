#pragma once
#include "GameEngineObject.h"
#include <GameEngineBase\GameEngineTimeEvent.h>
#include <string_view>
#include <map>
#include <GameEngineCore/GameEngineRenderTarget.h>

class GameEngineActor;
class GameEngineCamera;
class GameEngineRenderer;
class GameEngineCollision;
class GameEngineLevel : public GameEngineObject
{
	friend class GameEngineRenderer;
	friend class GameEngineCollision;
	friend class GameEngineTransform;
	friend class GameEngineCore;
	friend class GameEngineActor;
	friend class GameEngineTexture;

public:
	static void IsDebugSwitch()
	{
		IsDebugRender = !IsDebugRender;
	}

	GameEngineTimeEvent TimeEvent;

	// constrcuter destructer
	GameEngineLevel();
	virtual ~GameEngineLevel();

	// delete Function
	GameEngineLevel(const GameEngineLevel& _Other) = delete;
	GameEngineLevel(GameEngineLevel&& _Other) noexcept = delete;
	GameEngineLevel& operator=(const GameEngineLevel& _Other) = delete;
	GameEngineLevel& operator=(GameEngineLevel&& _Other) noexcept = delete;

	template<typename ActorType>
	std::shared_ptr<ActorType> CreateActorToName(const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(0, _Name);
	}

	// 아 개빡침 내가 변환해서 넣는다. _ <-- 가 클래스명에 들어가있으면 이게 안댐ㅋㅋㅋ 
	/*template<typename ActorType, typename EnumType>
	std::shared_ptr<ActorType> CreateActor(EnumType  _Order, const std::string_view& _Name = "")
	{
		return CreateActor<ActorType>(static_cast<int>(_Order), _Name);
	}*/

	template<typename ActorType>
	std::shared_ptr<ActorType> CreateActor(int _Order = 0, const std::string_view& _Name = "")
	{
		std::shared_ptr<GameEngineActor> NewActor = std::make_shared<ActorType>();

		std::string Name = _Name.data();
	
		// 2번인자로 아무것도 들어오지 않았다면
		if (_Name == "")
		{
			// 생성한 클래스의 이름으로.. 엥 여기서 추가 안한거같은데 
			const type_info& Info = typeid(ActorType);
			Name = Info.name();
			Name.replace(0, 6, "");
		}
		
		// 생성한 액터 초기화
		ActorInit(NewActor, _Order, this);

		return std::dynamic_pointer_cast<ActorType>(NewActor);
	}

	std::shared_ptr<class GameEngineCamera> CreateNewCamera(int _Order);

	std::shared_ptr<class GameEngineCamera> GetMainCamera()
	{
		return MainCamera;
	}

	std::shared_ptr<GameEngineLevel> GetSharedThis()
	{
		// GameEngineLevel의 shared_ptr type를 반환하는데 
		// 너무길어서 조금더 간단하게 사용하기 위해 작성
		return DynamicThis<GameEngineLevel>();
	}

	std::shared_ptr<GameEngineCamera> GetCamera(int _CameraOrder);

	std::shared_ptr<GameEngineRenderTarget> GetLastTarget()
	{
		return LastTarget;
	}


protected:
	virtual void LevelChangeStart();
	virtual void LevelChangeEnd();
	// 로딩
	virtual void Start() = 0;
	void Update(float _DeltaTime);
	void Render(float _DeltaTime);

private:
	static bool IsDebugRender;
	// 모든 카메라의 내용이 다 종합된.
	std::shared_ptr<GameEngineRenderTarget> LastTarget;
	//      이름           경로
	// 리소스 최적화를 위해서 레벨이 시작하고, 필요한 리소스를 로드할 때의 이름을 키값으로 하여 경로를 저장한다. 
	std::map<std::string, std::string> TexturePath;
	std::map<std::string, std::string> LoadEndPath;

	std::map<int, std::shared_ptr<GameEngineCamera>> Cameras;

	// 메인카메라 , UI카메라 
	std::shared_ptr<GameEngineCamera> MainCamera;

	void PushCameraRenderer(std::shared_ptr<GameEngineRenderer> _Renderer, int _CameraOrder);

	std::map<int, std::list<std::shared_ptr<GameEngineActor>>> Actors;

	std::map<int, std::list<std::shared_ptr<GameEngineCollision>>> Collisions;

	void PushCollision(std::shared_ptr<GameEngineCollision> _Collision);

	void ActorInit(std::shared_ptr<GameEngineActor> _Actor, int _Order, GameEngineLevel* _Level);

	void ActorUpdate(float _DeltaTime);
	void ActorRender(float _DeltaTime);
	void ActorRelease();
	void ActorLevelChangeStart();
	void ActorLevelChangeEnd();

	void TextureUnLoad(GameEngineLevel* _NextLevel);
	void TextureReLoad(GameEngineLevel* _PrevLevel);
};


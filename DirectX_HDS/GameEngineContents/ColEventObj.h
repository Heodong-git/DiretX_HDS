#pragma once
#include "BaseActor.h"

// 설명 :
class ColEventObj : public BaseActor
{
public:
	// constrcuter destructer
	ColEventObj();
	~ColEventObj();

	// delete Function
	ColEventObj(const ColEventObj& _Other) = delete;
	ColEventObj(ColEventObj&& _Other) noexcept = delete;
	ColEventObj& operator=(const ColEventObj& _Other) = delete;
	ColEventObj& operator=(ColEventObj&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;
	
private:
	void DebugUpdate();
	void CollisionUpdate(float _DeltaTime);
	// 렌더러생성 및 세팅
	void ComponentSetting();

	std::shared_ptr<class GameEngineSpriteRenderer> m_MainRender = nullptr;
	std::shared_ptr<class GameEngineCollision> m_Collision = nullptr;
};

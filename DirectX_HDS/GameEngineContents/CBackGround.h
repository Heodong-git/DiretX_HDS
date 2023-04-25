#pragma once
#include <GameEngineContents/CKatanaZero_Actor.h>

// ���� :
class CBackGround : public CKatanaZero_Actor
{
public:
	// constrcuter destructer
	CBackGround();
	~CBackGround();

	// delete Function
	CBackGround(const CBackGround& _Other) = delete;
	CBackGround(CBackGround&& _Other) noexcept = delete;
	CBackGround& operator=(const CBackGround& _Other) = delete;
	CBackGround& operator=(CBackGround&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;


private:
	float4 m_RenderCurPos = {};
	float4 m_StartPoint = { 0 , 350 , 0};
	float  m_MoveSpeed = 150.0f;
	std::shared_ptr<class GameEngineSpriteRenderer> m_BackGroundRender = nullptr;
};

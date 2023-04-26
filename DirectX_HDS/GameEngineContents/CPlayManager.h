#pragma once
#include "CKatanaZero_Actor.h"

// 설명 :
class CPlayManager : public CKatanaZero_Actor
{
public:
	// constrcuter destructer
	CPlayManager();
	~CPlayManager();

	// delete Function
	CPlayManager(const CPlayManager& _Other) = delete;
	CPlayManager(CPlayManager&& _Other) noexcept = delete;
	CPlayManager& operator=(const CPlayManager& _Other) = delete;
	CPlayManager& operator=(CPlayManager&& _Other) noexcept = delete;

	void CreateRender();
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	// 몇개 안들고 있는데 전부 그냥 들고 있는게 맞는거같기도?? 
	std::shared_ptr<class GameEngineSpriteRenderer> m_BackGroundRender = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> m_HUDRender = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> m_LeftRender = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> m_MiddleRender = nullptr;
	std::shared_ptr<class GameEngineSpriteRenderer> m_RightRender = nullptr;

};

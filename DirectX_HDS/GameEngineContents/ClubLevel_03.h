#pragma once
#include "BaseLevel.h"

// 설명 :
class ClubLevel_03 : public BaseLevel
{
public:
	// constrcuter destructer
	ClubLevel_03();
	~ClubLevel_03();

	// delete Function
	ClubLevel_03(const ClubLevel_03& _Other) = delete;
	ClubLevel_03(ClubLevel_03&& _Other) noexcept = delete;
	ClubLevel_03& operator=(const ClubLevel_03& _Other) = delete;
	ClubLevel_03& operator=(ClubLevel_03&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime);

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

	virtual void ResourcesLoad() override;
	virtual void ActorLoad() override;

private:
	std::shared_ptr<class StageEditer> m_GUI = nullptr;

	// 디버그용 
	void DebugUpdate();
};

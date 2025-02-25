#pragma once
#include "BaseActor.h"

enum class RifleEffectType
{
	NORMAL,
	SWEEP,
	CEILING_FIRE,
};

// ���� :
class HeadHunter_RifleEffect : public BaseActor
{
public:
	// constrcuter destructer
	HeadHunter_RifleEffect();
	~HeadHunter_RifleEffect();

	// delete Function
	HeadHunter_RifleEffect(const HeadHunter_RifleEffect& _Other) = delete;
	HeadHunter_RifleEffect(HeadHunter_RifleEffect&& _Other) noexcept = delete;
	HeadHunter_RifleEffect& operator=(const HeadHunter_RifleEffect& _Other) = delete;
	HeadHunter_RifleEffect& operator=(HeadHunter_RifleEffect&& _Other) noexcept = delete;

	std::shared_ptr<class GameEngineSpriteRenderer> GetRender()
	{
		return m_Render;
	}

	void SetActor(std::shared_ptr<class Boss_HeadHunter> _Actor)
	{
		m_Actor = _Actor;
	}
	
	void SetType(RifleEffectType _Type);

	void ChangeLoopAnimation();
	
	void EffectDeath();

	void RifleShotSoundPlay();


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	GameEngineSoundPlayer m_SoundPlayer;

	std::shared_ptr<class GameEngineSpriteRenderer> m_Render = nullptr;
	std::shared_ptr<class GameEngineCollision> m_Collision = nullptr;
	std::shared_ptr<class Boss_HeadHunter> m_Actor = nullptr;

	RifleEffectType m_Type = RifleEffectType::NORMAL;
};

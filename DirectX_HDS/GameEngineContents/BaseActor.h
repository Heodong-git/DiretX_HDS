#pragma once
#include <GameEnginePlatform/GameEngineSound.h>
#include <GameEngineCore/GameEngineActor.h>

// BaseState ???? 

class BaseActor : public GameEngineActor
{
	// 레벨에서 초기상태로 리셋하게 될 경우
	// 액터들을 초기상태로 설정하기 위함 
	friend class BaseLevel;
public:
	// constrcuter destructer
	BaseActor();
	~BaseActor();

	// delete Function
	BaseActor(const BaseActor& _Other) = delete;
	BaseActor(BaseActor&& _Other) noexcept = delete;
	BaseActor& operator=(const BaseActor& _Other) = delete;
	BaseActor& operator=(BaseActor&& _Other) noexcept = delete;

	class BaseLevel* GetReturnCastLevel() const;

	inline void SetInitPos(float4& _Pos)
	{
		m_InitPos = _Pos;
	}

	inline const float4 GetInitPos() const
	{
		return m_InitPos;
	}

	virtual void BulletCollision() {};
	
	void SetDeath()
	{
		m_IsDeath = true;
	}

protected:
	void Update(float _DeltaTime) override;

	// 디버그 스위치
	virtual void DebugSwitch()
	{
		if (true == IsDebug())
		{
			DebugOff();
		}

		else if (false == IsDebug())
		{
			DebugOn();
		}
	}

	bool m_IsDeath = false;
	
private:

	// BaseActor들은 전부 얘를 오버라이드 
	virtual void Reset() {};
	float4 m_InitPos = {};
};

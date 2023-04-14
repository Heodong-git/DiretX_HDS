#pragma once
#include <GameEngineCore/GameEngineLevel.h>

class CKatanaZero_Level : public GameEngineLevel
{
public:
	// constrcuter destructer
	CKatanaZero_Level();
	~CKatanaZero_Level();

	// delete Function
	CKatanaZero_Level(const CKatanaZero_Level& _Other) = delete;
	CKatanaZero_Level(CKatanaZero_Level&& _Other) noexcept = delete;
	CKatanaZero_Level& operator=(const CKatanaZero_Level& _Other) = delete;
	CKatanaZero_Level& operator=(CKatanaZero_Level&& _Other) noexcept = delete;

	// 제한시간 
	inline void SetTimeLimit(float _Time)
	{
		m_TimeLimit = _Time;
	}

	// 제한시간 반환
	inline float GetTimeLimit()
	{
		return m_TimeLimit;
	}

	// 제한시간 감소 
	inline void TimeIncrease(float _DeltaTime)
	{
		m_TimeLimit += _DeltaTime;
	}

	// Level을 만들때 녹화가 필요없는 레벨은 이걸 호출해서 false로 만든다. 
	inline void RecodingOff()
	{
		m_Recording = false;
	}

	inline bool GetRecordUse()
	{
		return m_Recording;
	}

	inline bool IsTimeOver() const
	{
		return m_TimeOver;
	}
protected:
	// 녹화하고 역재생 해야하는 클래스만 재정의한 Start에서 얘 호출.
	void Start() override;
	void Update(float _DeltaTime);

private:
	// 타임오버
	void TimeOver();

	// 역재생
	void ReversePlay(float _DeltaTime);

	bool m_Recording = true;
	bool m_TimeOver = false;

	// 임시, 스테이지의 제한시간
	float m_TimeLimit = 0.0f;
	float m_MaxTimeLimit = 4.0f;
	float m_SaveInterval = 2.0f;
};
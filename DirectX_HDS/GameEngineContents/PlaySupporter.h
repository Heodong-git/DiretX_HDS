#pragma once
#include "BaseActor.h"

// 설명 :
class PlaySupporter : public BaseActor
{
	friend class Player;
public:
	static PlaySupporter* MainSupporter;
public:
	// constrcuter destructer
	PlaySupporter();
	~PlaySupporter();

	// delete Function
	PlaySupporter(const PlaySupporter& _Other) = delete;
	PlaySupporter(PlaySupporter&& _Other) noexcept = delete;
	PlaySupporter& operator=(const PlaySupporter& _Other) = delete;
	PlaySupporter& operator=(PlaySupporter&& _Other) noexcept = delete;

	// 스테이지 클리어 실패, 투명, 클리어 성공, 마우스 클릭 체크용 콜리전
	static std::shared_ptr<class GameEngineUIRenderer> g_FailRender;
	static std::shared_ptr<class GameEngineUIRenderer> g_BlackBoxRender;
	static std::shared_ptr<class GameEngineUIRenderer> g_ClearRender;

	void ResetButtonOn();
	void CameraZoomEffect(const float _Ratio);
	void CameraShake(float _DeltaTime);
	void CameraShakeOn();

	virtual void ForcedReset() override;

protected:
	virtual void LevelChangeStart() override;
	virtual void LevelChangeEnd() override;

	void Start() override;
	void Update(float _DeltaTime) override;
	void Render(float _DeltaTime) override;

private:
	// 사용할 배경사운드를 저장해두고, 내가 생성될때, 현재 레벨이 어떤 레벨인지 체크해서
	// 해당하는 사운드를 Play 시켜준다. 
	static GameEngineSoundPlayer m_BGMPlayer;

	bool m_CameraShake = false;
	
	float m_CameraShakeTime = 0.275f;
	float m_CameraShakeXValue = 0.0f;

	float4 m_CamOriginPos = float4::Zero;

	// 흔들림 세기 
	const float cameraShakeAmplitude = 2.0f;
	const float cameraShakeFrequency = 20.0f;
	// x축 흔들림 범위
	const float cameraXRange = 12.0f;


	void CameraMovement(float _DeltaTime);
	
	bool PlayerDeathCheck();
	void LevelResetCheck();

	std::shared_ptr<class GameEngineCamera> m_MainCamera = nullptr;
	const float m_MoveSpeed = 500.0f;
	
	void ComponentSetting();
	void SaveCameraRange();
	void LoadResources();

	// 좌상단, 우상단, 좌하단 , 우하단 순서로 
	std::map<int, std::vector<float4>> m_MapRanges = std::map<int, std::vector<float4>>();
	virtual void Reset() override;
};

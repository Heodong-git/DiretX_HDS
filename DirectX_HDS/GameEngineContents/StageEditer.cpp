#include "PrecompileHeader.h"
#include "StageEditer.h"

#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineCore/GameEngineTransform.h>
#include <GameEngineCore/GameEngineCamera.h>

#include "BaseLevel.h"
#include "Player.h"
#include "Cursor.h"

StageEditer::StageEditer()
{
}

StageEditer::~StageEditer()
{
}

void StageEditer::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	// 플레이어 디버그 
	if (nullptr != Player::MainPlayer)
	{
		float4 PlayerPos = Player::MainPlayer->GetTransform()->GetLocalPosition();
		PlayerState CurState = Player::MainPlayer->GetCurState();
        bool PlayerDir = Player::MainPlayer->GetDir();

		std::shared_ptr<GameEngineLevel> CurLevel = GameEngineCore::GetCurLevel();
        if (nullptr == CurLevel)
        {
            MsgAssert("레벨이 nullptr 입니다. ");
            return;
        }

		float4 CameraPos = CurLevel->GetMainCamera()->GetTransform()->GetLocalPosition();

        if (ImGui::Begin("Debug"))
        {
            // 플레이어 위치 정보
            ImGui::Text("Player Position:");
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.27f);
            ImGui::Text("Camera Position:");
            ImGui::Separator();

            ImGui::Columns(4, nullptr, true);
            ImGui::SetColumnWidth(0, 60);
            ImGui::SetColumnWidth(1, 80);
            ImGui::SetColumnWidth(2, 60);
            ImGui::SetColumnWidth(3, 80);

            ImGui::Text("X:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", PlayerPos.x);
            ImGui::NextColumn();
            ImGui::Text("X:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", CameraPos.x);
            ImGui::NextColumn();

            ImGui::Text("Y:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", PlayerPos.y);
            ImGui::NextColumn();
            ImGui::Text("Y:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", CameraPos.y);
            ImGui::NextColumn();

            ImGui::Text("Z:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", PlayerPos.z);
            ImGui::NextColumn();
            ImGui::Text("Z:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", CameraPos.z);
            ImGui::NextColumn();

            ImGui::Text("W:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", PlayerPos.w);
            ImGui::NextColumn();
            ImGui::Text("W:");
            ImGui::NextColumn();
            ImGui::Text("%.2f", CameraPos.w);
            ImGui::NextColumn();

            ImGui::Columns(1);
            ImGui::Separator();

            // 플레이어 상태
            ImGui::Text("Player State: ");
            switch (CurState)
            {
            case PlayerState::NONE:
                ImGui::SameLine();
                ImGui::Text("NONE ?");
                break;
            case PlayerState::IDLETORUN:
                ImGui::SameLine();
                ImGui::Text("Idle_to_run");
                break;
            case PlayerState::IDLE:
                ImGui::SameLine();
                ImGui::Text("Idle");
                break;
            case PlayerState::MOVE:
                ImGui::SameLine();
                ImGui::Text("Move");
                break;
            case PlayerState::JUMP:
                ImGui::SameLine();
                ImGui::Text("Jump");
                break;
            case PlayerState::SLASH:
                ImGui::SameLine();
                ImGui::Text("Slash");
                break;
            case PlayerState::CROUCH:
                ImGui::SameLine();
                ImGui::Text("Crouch");
                break;
            case PlayerState::FLIP:
                ImGui::SameLine();
                ImGui::Text("Flip");
                break;
            case PlayerState::FALL:
                ImGui::SameLine();
                ImGui::Text("Fall");
                break;
            case PlayerState::ROLL:
                ImGui::SameLine();
                ImGui::Text("Roll");
                break;
            case PlayerState::RIGHTWALL:
                ImGui::SameLine();
                ImGui::Text("RightWall");
                break;
            case PlayerState::LEFTWALL:
                ImGui::SameLine();
                ImGui::Text("LeftWall");
                break;
            }

            ImGui::Text("Dir : ");
            ImGui::SameLine();
            ImGui::Text(PlayerDir ? "Right" : "Left");

            if (nullptr != Cursor::MainCursor)
            {
                float4 CursorPos = Cursor::MainCursor->GetGameCursorPos();

                ImGui::Text("Mouse Position:");
                ImGui::SameLine();
                ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f);

                ImGui::Separator();

                ImGui::Columns(4, nullptr, true);
                ImGui::SetColumnWidth(0, 60);
                ImGui::SetColumnWidth(1, 80);
                ImGui::SetColumnWidth(2, 60);
                ImGui::SetColumnWidth(3, 80);

                ImGui::Text("X:");
                ImGui::NextColumn();
                ImGui::Text("%.2f", CursorPos.x);
                ImGui::NextColumn();

                ImGui::Text("Slow :");
                ImGui::NextColumn();
                ImGui::Text("%s", Player::MainPlayer->IsSlowSkill() ? "true" : "false");

              

                ImGui::NextColumn();
                ImGui::Text("Y:");
                ImGui::NextColumn();
                ImGui::Text("%.2f", CursorPos.y);
                ImGui::NextColumn();

                ImGui::Text("SkillTime :");
                ImGui::NextColumn();
                ImGui::Text("%.2f", Player::MainPlayer->GetSlowLimitTime());
                ImGui::NextColumn();


                ImGui::Columns(1);
                ImGui::Separator();
            }


            BaseLevel* CastLevel = dynamic_cast<BaseLevel*>(CurLevel.get());

            if (nullptr == CastLevel)
            {
                return;
            }

            {
                // 레벨 몬스터 카운트
                ImGui::Text("Monster Count :");
                ImGui::SameLine();
                ImGui::Text("%.2d", CastLevel->GetMonsterCount());

                ImGui::Text("Level LimitTime :");
                ImGui::SameLine();
                ImGui::Text("%.2f", CastLevel->GetLimitTime());
            }
            
            ImGui::End();
        }

	}

	

	
}

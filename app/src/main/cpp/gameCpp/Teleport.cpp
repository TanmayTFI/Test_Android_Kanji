//====================================
//DQFULP
// Teleport Handling
//====================================

#include "Teleport.h"
#include "GameFlow.h"
#include "HiddenObjectEnums.h"
#include "ProfileSystem.h"
#include "LangDefs.h"

const CRectangle  TeleThumbRect(1033.0f,376.0f,174.0f,245.0f);

TeleportSystem::TeleportSystem()
{
	TeleScene = NULL;
	VFX_BG = NULL;

	targetAreaID = eArea_L1_S1_Beach; //beach is default. If no task when coming to beach its a bug.

	TeleBG = TeleOverlay = TeleSceneThumb = TeleThumbGlow = NULL;
	
	waveTimer=0;
	waveOffset=5.0f;

	isActive = false;
	isBGTweening = false;
}

TeleportSystem::~TeleportSystem()
{
	isActive = false;
	isBGTweening = false;

	SAFE_DELETE(TeleScene);
	SAFE_DELETE(VFX_BG);

	TeleBG = TeleOverlay = TeleSceneThumb = TeleThumbGlow = NULL;
}

void TeleportSystem::Required()
{
	if(!isActive)
		return;
}

void TeleportSystem::Update()
{
	if(!isActive)
		return;

	Cursor::SetMode(CA_Normal);
	if(isBGTweening)
	{
		if(TeleBG->IsTweening && TeleBG->IsTweenFinished())
		{
			isBGTweening = false;
			TeleBG->IsTweening = false;
			
			TeleSceneThumb->SetEnable(true);			
			TeleSceneThumb->Show(false);
			TeleSceneThumb->FadeIn(4.0f);
			TeleOverlay->SetEnable(true);

			TeleScene->SaveXY();
		}
	}
	else
	{
		CPoint pos = Control::Input->Mpos();
		if(TeleThumbRect.Intersection(&pos))
		{
			Cursor::SetMode(CA_ExitUp, targetAreaID);
			TeleThumbGlow->SetEnable(true);
			if(Control::Input->LBclicked())
			{
				HideTeleport();
				Control::Input->ClearMouse();
				SendMessage(ActionProcessed, ID, HOSM_TeleportClosed);
				return;
			}
		}
		else
		{
			TeleThumbGlow->SetEnable(false);
			if(Control::Input->LBclicked())
			{
				HideTeleport();
				Control::Input->ClearMouse();
				return;
			}
		}

		waveTimer += 100*Control::LogicRate;
		TeleScene->ShiftXY(0,waveOffset*Elixir::Sin(waveTimer));
	}
}

void  TeleportSystem::ShowTeleport()
{
	isActive = true;

	TeleScene = new CHOScene();
	
	if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_1)
		TeleScene->Load("UI\\TELE\\L1\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_2)
		TeleScene->Load("UI\\TELE\\L2\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_3)
		TeleScene->Load("UI\\TELE\\L3\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_4)
		TeleScene->Load("UI\\TELE\\L4\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_5)
		TeleScene->Load("UI\\TELE\\L5\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_6)
		TeleScene->Load("UI\\TELE\\L6\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_7)
		TeleScene->Load("UI\\TELE\\L7\\TELE.SCN");
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_8)
		TeleScene->Load("UI\\TELE\\L8\\TELE.SCN");

	TeleScene->SetZOff(eZOrder_ComboInvScene+10);

	TeleBG = TeleScene->GetObjectByID("telebg");
	TeleOverlay = TeleScene->GetObjectByID("overlay");
	TeleSceneThumb = TeleScene->GetObjectByID("scnthumb");
	TeleThumbGlow = TeleScene->GetObjectByID("thumbglow");	

	TeleScene->SaveXY();

	CPoint targetPos = TeleBG->Pos;

	TeleOverlay->SetEnable(false);
	TeleSceneThumb->SetEnable(false);
	TeleThumbGlow->SetEnable(false);

	TeleBG->Pos.y += 200;
	TeleBG->SetAlpha(0);
	TeleBG->SetScale(0.25f);

	float valArr[5] = {1.0f, targetPos.x, targetPos.y, 1.0, 0};
	TeleBG->TweenTo(valArr, 1.25f, TW_EASEBACKOUT, false);
	isBGTweening = true;

	VFX_BG = new SuperFX("PRT\\UI\\tele.INI", PointSystem::CreateCPoint(1116.0f,490.0f,0), eZOrder_ComboInvScene);
	VFX_BG->Start();

	Control::Audio->QuickLoadAndPlaySFX(aSFXTeleShow);
	if (GFInstance->IsReplayingGame)
		SetupMorphTeleport();
	else
		SetupTeleport();
}

void TeleportSystem::SetupTeleport()
{
	std::vector<int> sceneIds;

	TeleSceneThumb->PlayAnimation(0);
	if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_1)
	{
		sceneIds.push_back(eArea_L1_S1_Beach);
		sceneIds.push_back(eArea_L1_S2_Frontyard);
		sceneIds.push_back(eArea_L1_S3_Foyer);
		sceneIds.push_back(eArea_L1_S4_Bedroom);
		sceneIds.push_back(eArea_L1_S5_TreasureRoom);
		sceneIds.push_back(eArea_L1_S6_BoatShed);
		sceneIds.push_back(eArea_L1_S7_BoatDock);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_2)
	{
		sceneIds.push_back(eArea_L2_S1_ForestEntrance);
		sceneIds.push_back(eArea_L2_S2_NagaTemple);
		sceneIds.push_back(eArea_L2_S3_CaveMouth);
		sceneIds.push_back(eArea_L2_S4_Cave);
		sceneIds.push_back(eArea_L2_S5_MoopansDen);
		sceneIds.push_back(eArea_L2_S6_TunnelEnd);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_3)
	{
		sceneIds.push_back(eArea_L3_S1_FortEntrance);
		sceneIds.push_back(eArea_L3_S2_FortInterior);
		sceneIds.push_back(eArea_L3_S2_P1Room1Start);
		sceneIds.push_back(eArea_L3_S3_Pool);
		sceneIds.push_back(eArea_L3_S4_FortFountain);
		sceneIds.push_back(eArea_L3_S5_Cliff);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_4)
	{
		sceneIds.push_back(eArea_L4_S1_SwampEntrance);
		sceneIds.push_back(eArea_L4_S2_RiverSide);
		sceneIds.push_back(eArea_L4_S3_Cave);
		sceneIds.push_back(eArea_L4_S4_HillTop);
		sceneIds.push_back(eArea_L4_S5_HillSide);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_5)
	{
		sceneIds.push_back(eArea_L5_S1_TigerDistant);
		sceneIds.push_back(eArea_L5_S2_TigerCloseup);
		sceneIds.push_back(eArea_L5_S3_Pond);
		sceneIds.push_back(eArea_L5_S4_Dungeon);
		sceneIds.push_back(eArea_L5_S5_MountainBelow);
		sceneIds.push_back(eArea_L5_S6_MountainTop);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_6)
	{
		sceneIds.push_back(eArea_L6_S1_FloatingIsland1);
		sceneIds.push_back(eArea_L6_S2_FloatingIsland2);
		sceneIds.push_back(eArea_L6_S3_FloatingIsland3);
		sceneIds.push_back(eArea_L6_S4_FloatingIsland4);
		sceneIds.push_back(eArea_L6_S5_MarysDream);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_7)
	{
		sceneIds.push_back(eArea_L7_S1_Volcano);
		sceneIds.push_back(eArea_L7_S2_LasyaPrison);
		sceneIds.push_back(eArea_L7_S3_Haven);
		sceneIds.push_back(eArea_L7_S4_Ameeha);
		sceneIds.push_back(eArea_L7_S5_SophiaEnd);
	}
	else if(ProfileSystem::Instance->ActiveProfile.CurrentLevel == Level_8)
	{
		sceneIds.push_back(eArea_L8_S1_ShukrasPlace);
		sceneIds.push_back(eArea_L8_S2_Garden);
		sceneIds.push_back(eArea_L8_S3_BanyanTree);
		sceneIds.push_back(eArea_L8_S4_Pool);
		sceneIds.push_back(eArea_L8_S5_VolcanoLand);
	}

	targetAreaID = sceneIds.at(0); //First scene is default.
	int numAreas = (int) sceneIds.size();  //Sac: converted Implicit to explicit

	int currentAreaID = ProfileSystem::Instance->ActiveProfile.CurrentArea;
	for (int i = 0; i < numAreas; i++)
	{
		if(sceneIds[i]!= currentAreaID)
		{
			if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[sceneIds[i]])
			{
				//task left in scene sceneIds[i]
			//	K_LOG("Found task in scene id %d", sceneIds[i]);

				TeleSceneThumb->PlayAnimation(i);

				//special case
				if(sceneIds[i] == eArea_L6_S3_FloatingIsland3)
				{
					if(!ProfileSystem::Instance->ActiveProfile.L6_S3_RoomLightened)
					{
						TeleSceneThumb->PlayAnimation(5);
					}
				}				
					
				targetAreaID = sceneIds[i];
				break;
			}
		}
	}
}

void TeleportSystem::HideTeleport()
{
	isActive = false;
	isBGTweening = false;

	SAFE_DELETE(TeleScene);
	SAFE_DELETE(VFX_BG);

	TeleBG = TeleOverlay = TeleSceneThumb = TeleThumbGlow = NULL;
}

void TeleportSystem::SetupMorphTeleport()
{
	if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 0)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L1_S1_Beach;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L1_S2_Frontyard;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected)
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L1_S3_Foyer;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected)
		{
			TeleSceneThumb->PlayAnimation(6);
			targetAreaID = eArea_L1_S7_BoatDock;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy])
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L1_S1_Beach;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb])
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L1_S3_PZShelf;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck])
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L1_S3_Foyer;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L1_S4_Bedroom;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll])
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L1_S6_BoatShed;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(6);
			targetAreaID = eArea_L1_S7_BoatDock;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 1)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L2_S1_ForestEntrance;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L2_S2_NagaTemple;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected)
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L2_S3_CaveMouth;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L2_S4_Cave;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L2_S5_MoopansDen;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L2_S5_MoopansDen;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L2_S5_MoopansDen;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L2_S4_Cave;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L2_S6_TunnelEnd;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L2_S7_PZSoldiers;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 2)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L3_S1_FortEntrance;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L3_S2_FortInterior;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L3_S3_Pool;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected)
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L3_S4_FortFountain;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L3_S1_FortEntrance;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L3_S2_FortInterior;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L3_S4_FortFountain;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg])
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L3_S5_Cliff;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L3_S5_Cliff;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 3)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L4_S1_SwampEntrance;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected)
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L4_S3_Cave;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L4_S4_HillTop;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected)
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L4_S5_HillSide;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L4_S1_SwampEntrance;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana])
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L4_S2_RiverSide;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L4_S3_Cave;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L4_S4_HillTop;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L4_S5_HillSide;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L4_S3_Cave;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 4)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L5_S2_TigerCloseup;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L5_S4_Dungeon;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected)
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L5_S5_MountainBelow;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected)
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L5_S6_MountainTop;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue])
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L5_S2_TigerCloseup;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L5_S2_TigerCloseup;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue])
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L5_S3_Pond;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L5_S4_Dungeon;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L5_S5_MountainBelow;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(5);
			targetAreaID = eArea_L5_S6_MountainTop;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 5)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L6_S1_FloatingIsland1;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L6_S2_FloatingIsland2;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected)
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L6_S3_FloatingIsland3;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L6_S4_FloatingIsland4;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L6_S1_FloatingIsland1;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L6_S2_FloatingIsland2;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush])
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L6_S3_FloatingIsland3;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L6_S4_FloatingIsland4;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal])
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L6_S5_MarysDream;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L6_S5_MarysDream;
		}
	}

	else if (ProfileSystem::Instance->ActiveProfile.CurrentLevel == 6)
	{
		if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected)
		{
			TeleSceneThumb->PlayAnimation(0);
			targetAreaID = eArea_L7_S1_Volcano;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected)
		{
			TeleSceneThumb->PlayAnimation(1);
			targetAreaID = eArea_L7_S2_LasyaPrison;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected)
		{
			TeleSceneThumb->PlayAnimation(2);
			targetAreaID = eArea_L7_S3_Haven;
		}
		else if (!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected)
		{
			TeleSceneThumb->PlayAnimation(3);
			targetAreaID = eArea_L7_S4_Ameeha;
		}
		else
		{
			TeleSceneThumb->PlayAnimation(4);
			targetAreaID = eArea_L7_S5_SophiaEnd;
		}
	}
}
//====================================
//DQFULP
// Hidden Object System
//====================================

#include "Map.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Util.h"
#include "GameFlow.h"
#include "LangDefs.h"
#include "Hud.h"
#include "AwardManager.h"

extern HudControl* GFHud;

const CRectangle  CloseBtnRect(1100.0f,19.0f,80.0f,72.0f);
const CRectangle  MapAreaRect(188.0f,4.0f,995.0f,723.0f);

const CRectangle  BtnRectJournalTab(199.0f,102.0f,55.0f,143.0f);
const CRectangle  BtnRectObjectivesTab(199.0f,254.0f,57.0f,141.0f);
const CRectangle  BtnRectMapTab(200.0f,400.0f,59.0f,141.0f);
const CRectangle  BtnRectCharactersTab(199.0f,547.0f,60.0f,140.0f);

Map::Map()
{
	for (int i = 0; i < 10; i++)
	{
		SceneThumbs[i] = NULL;
		MapIcons[i] = NULL;
		TaskIcons[i] = NULL;
		ThumbGlows[i] = NULL;

		MorphIcons[i] = NULL;
		MemoryIcons[i] = NULL;
	}

	CurrentLevel = ProfileSystem::Instance->ActiveProfile.CurrentLevel;

	MapScene = NULL;

	if(CurrentLevel == Level_1)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCN);		

		sceneRectArray.push_back(new CRectangle(761.0f,518.0f,87.0f,85.0f));
		sceneRectArray.push_back(new CRectangle(673.0f,318.0f,85.0f,90.0f));
		sceneRectArray.push_back(new CRectangle(748.0f,237.0f,76.0f,77.0f));
		sceneRectArray.push_back(new CRectangle(810.0f,161.0f,80.0f,75.0f));
		sceneRectArray.push_back(new CRectangle(895.0f,226.0f,77.0f,80.0f));
		sceneRectArray.push_back(new CRectangle(436.0f,286.0f,84.0f,80.0f));
		sceneRectArray.push_back(new CRectangle(531.0f,231.0f,91.0f,86.0f));

		sceneIDArray.push_back(eArea_L1_S1_Beach);
		sceneIDArray.push_back(eArea_L1_S2_Frontyard);
		sceneIDArray.push_back(eArea_L1_S3_Foyer);
		sceneIDArray.push_back(eArea_L1_S4_Bedroom);
		sceneIDArray.push_back(eArea_L1_S5_TreasureRoom);
		sceneIDArray.push_back(eArea_L1_S6_BoatShed);
		sceneIDArray.push_back(eArea_L1_S7_BoatDock);
	}
	else if(CurrentLevel == Level_2)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL2);

		sceneRectArray.push_back(new CRectangle(471.0f,517.0f,97.0f,99.0f));
		sceneRectArray.push_back(new CRectangle(379.0f,296.0f,96.0f,92.0f));
		sceneRectArray.push_back(new CRectangle(561.0f,391.0f,98.0f,98.0f));
		sceneRectArray.push_back(new CRectangle(579.0f,232.0f,106.0f,101.0f));
		sceneRectArray.push_back(new CRectangle(485.0f,95.0f,108.0f,104.0f));
		sceneRectArray.push_back(new CRectangle(742.0f,186.0f,107.0f,102.0f));
		sceneRectArray.push_back(new CRectangle(864.0f,291.0f,96.0f,95.0f));

		sceneIDArray.push_back(eArea_L2_S1_ForestEntrance);
		sceneIDArray.push_back(eArea_L2_S2_NagaTemple);
		sceneIDArray.push_back(eArea_L2_S3_CaveMouth);
		sceneIDArray.push_back(eArea_L2_S4_Cave);
		sceneIDArray.push_back(eArea_L2_S5_MoopansDen);	
		sceneIDArray.push_back(eArea_L2_S6_TunnelEnd);
		sceneIDArray.push_back(eArea_L2_S7_PZSoldiers);
	}
	else if(CurrentLevel == Level_3)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL3);

		sceneRectArray.push_back(new CRectangle(536.0f,505.0f,107.0f,103.0f));
		sceneRectArray.push_back(new CRectangle(563.0f,319.0f,105.0f,105.0f));
		sceneRectArray.push_back(new CRectangle(739.0f,352.0f,126.0f,124.0f));
		sceneRectArray.push_back(new CRectangle(367.0f,264.0f,126.0f,124.0f));
		sceneRectArray.push_back(new CRectangle(537.0f,138.0f,126.0f,123.0f));
		sceneRectArray.push_back(new CRectangle(770.0f,101.0f,124.0f,119.0f));

		sceneIDArray.push_back(eArea_L3_S1_FortEntrance);
		sceneIDArray.push_back(eArea_L3_S2_FortInterior);
		sceneIDArray.push_back(eArea_L3_S2_P1Room1Start);
		sceneIDArray.push_back(eArea_L3_S3_Pool);
		sceneIDArray.push_back(eArea_L3_S4_FortFountain);	
		sceneIDArray.push_back(eArea_L3_S5_Cliff);
	}
	else if(CurrentLevel == Level_4)
	{
		CurrentLevel = Level_4;

		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL4);

		sceneRectArray.push_back(new CRectangle(509.0f,449.0f,151.0f,135.0f));
		sceneRectArray.push_back(new CRectangle(408.0f,274.0f,166.0f,130.0f));
		sceneRectArray.push_back(new CRectangle(282.0f,82.0f,171.0f,150.0f));
		sceneRectArray.push_back(new CRectangle(743.0f,69.0f,201.0f,168.0f));
		sceneRectArray.push_back(new CRectangle(814.0f,285.0f,195.0f,164.0f));

		sceneIDArray.push_back(eArea_L4_S1_SwampEntrance);
		sceneIDArray.push_back(eArea_L4_S2_RiverSide);
		sceneIDArray.push_back(eArea_L4_S3_Cave);
		sceneIDArray.push_back(eArea_L4_S4_HillTop);
		sceneIDArray.push_back(eArea_L4_S5_HillSide);
	}
	else if(CurrentLevel == Level_5)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL5);

		sceneRectArray.push_back(new CRectangle(494.0f,497.0f,124.0f,108.0f));
		sceneRectArray.push_back(new CRectangle(339.0f,353.0f,146.0f,123.0f));
		sceneRectArray.push_back(new CRectangle(482.0f,221.0f,152.0f,122.0f));
		sceneRectArray.push_back(new CRectangle(338.0f,82.0f,158.0f,124.0f));
		sceneRectArray.push_back(new CRectangle(737.0f,111.0f,151.0f,140.0f));
		sceneRectArray.push_back(new CRectangle(915.0f,259.0f,158.0f,136.0f));

		sceneIDArray.push_back(eArea_L5_S1_TigerDistant);
		sceneIDArray.push_back(eArea_L5_S2_TigerCloseup);
		sceneIDArray.push_back(eArea_L5_S3_Pond);
		sceneIDArray.push_back(eArea_L5_S4_Dungeon);
		sceneIDArray.push_back(eArea_L5_S5_MountainBelow);
		sceneIDArray.push_back(eArea_L5_S6_MountainTop);
	}
	else if(CurrentLevel == Level_6)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL6);

		sceneRectArray.push_back(new CRectangle(493.0f,506.0f,147.0f,121.0f));
		sceneRectArray.push_back(new CRectangle(472.0f,188.0f,135.0f,111.0f));
		sceneRectArray.push_back(new CRectangle(355.0f,320.0f,142.0f,115.0f));
		sceneRectArray.push_back(new CRectangle(781.0f,196.0f,159.0f,124.0f));
		sceneRectArray.push_back(new CRectangle(871.0f,368.0f,159.0f,132.0f));


		sceneIDArray.push_back(eArea_L6_S1_FloatingIsland1);
		sceneIDArray.push_back(eArea_L6_S2_FloatingIsland2);
		sceneIDArray.push_back(eArea_L6_S3_FloatingIsland3);
		sceneIDArray.push_back(eArea_L6_S4_FloatingIsland4);
		sceneIDArray.push_back(eArea_L6_S5_MarysDream);
	}
	else if(CurrentLevel == Level_7)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL7);

		sceneRectArray.push_back(new CRectangle(424.0f,417.0f,164.0f,132.0f));
		sceneRectArray.push_back(new CRectangle(298.0f,219.0f,148.0f,121.0f));
		sceneRectArray.push_back(new CRectangle(722.0f,268.0f,149.0f,122.0f));
		sceneRectArray.push_back(new CRectangle(833.0f,118.0f,166.0f,131.0f));
		sceneRectArray.push_back(new CRectangle(901.0f,369.0f,147.0f,153.0f));

		sceneIDArray.push_back(eArea_L7_S1_Volcano);
		sceneIDArray.push_back(eArea_L7_S2_LasyaPrison);
		sceneIDArray.push_back(eArea_L7_S3_Haven);
		sceneIDArray.push_back(eArea_L7_S4_Ameeha);
		sceneIDArray.push_back(eArea_L7_S5_SophiaEnd);
	}
	else if(CurrentLevel == Level_8)
	{
		MapScene =  GFInstance->LoadBufferScene(MAP_SCNL8);

		sceneRectArray.push_back(new CRectangle(497.0f,494.0f,145.0f,114.0f));
		sceneRectArray.push_back(new CRectangle(492.0f,319.0f,150.0f,123.0f));
		sceneRectArray.push_back(new CRectangle(348.0f,174.0f,152.0f,124.0f));
		sceneRectArray.push_back(new CRectangle(681.0f,203.0f,155.0f,137.0f));
		sceneRectArray.push_back(new CRectangle(885.0f,388.0f,171.0f,126.0f));

		sceneIDArray.push_back(eArea_L8_S1_ShukrasPlace);
		sceneIDArray.push_back(eArea_L8_S2_Garden);
		sceneIDArray.push_back(eArea_L8_S3_BanyanTree);
		sceneIDArray.push_back(eArea_L8_S4_Pool);
		sceneIDArray.push_back(eArea_L8_S5_VolcanoLand);
	}
	else
	{
		#ifdef K_WIN32
			WARN(StringTable::getString("ERR_MapUndef"));////Removed Wide string conversion for Windows	
		#else
			WARN(StringTable::getString("ERR_MapUndef"));
		#endif
	}

	CObject* temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\LEGEND.SPR", MapScene->GetObjectByID("legend")->Pos.x, MapScene->GetObjectByID("legend")->Pos.y, MapScene->GetObjectByID("legend")->ZOrder + 1);
	temp->SetUID("legendText");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	MaxCurrentLevelScenes = (int)sceneRectArray.size();  //Sac: converted Implicit to explicit

	if(MapScene->GetObjectByID("diarybg")!=NULL && MapScene->GetObjectByID("diarybg")->ZOrder<=0)
	{
		MapScene->SetZOff(eZOrder_MapdiaryPopup);
	}

	BtnTabJournal = MapScene->GetObjectByID("btnjournal");
	BtnTabObjective = MapScene->GetObjectByID("btnobjective");
	BtnTabCharacter = MapScene->GetObjectByID("btncharacter");
	MapScene->GetObjectByID("btnmap")->PlayAnimation(2);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\JOURNAL.SPR", BtnTabJournal->Pos.x, BtnTabJournal->Pos.y, BtnTabJournal->ZOrder + 1);
	temp->SetUID("btnjournaltext");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\MAP.SPR", MapScene->GetObjectByID("btnmap")->Pos.x, MapScene->GetObjectByID("btnmap")->Pos.y, MapScene->GetObjectByID("btnmap")->ZOrder + 1);
	temp->SetUID("btnmaptext");
	temp->PlayAnimation(1);
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\OBJECTIVE.SPR", BtnTabObjective->Pos.x, BtnTabObjective->Pos.y, BtnTabObjective->ZOrder + 1);
	temp->SetUID("btnobjectivetext");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\CHARACTERS.SPR", BtnTabCharacter->Pos.x, BtnTabCharacter->Pos.y, BtnTabCharacter->ZOrder + 1);
	temp->SetUID("btncharactertext");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\MEMTEXT.SPR", MapScene->GetObjectByID("extraslegend")->Pos.x, MapScene->GetObjectByID("extraslegend")->Pos.y, MapScene->GetObjectByID("extraslegend")->ZOrder + 1);
	temp->SetUID("MEMTEXT");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	temp = new CObject();
	temp->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\MAPDIA\\MORPHTEXT.SPR", MapScene->GetObjectByID("extraslegend")->Pos.x, MapScene->GetObjectByID("extraslegend")->Pos.y, MapScene->GetObjectByID("extraslegend")->ZOrder + 1);
	temp->SetUID("MORPHTEXT");
	//temp->SetEnable(false);
	MapScene->PushObject(temp);

	MapScene->Show(false);

#ifndef _CEBUILD
	MapScene->GetObjectByID("extraslegend")->SetEnable(false); 
	MapScene->GetObjectByID("MEMTEXT")->SetEnable(false);
	MapScene->GetObjectByID("MORPHTEXT")->SetEnable(false);
#endif // 

	IsTransitionActive = false;
	IsMapActive = false;
	IsMapClosed = false;
	IsOtherSceneClicked = false;
	isPlayerInMainScene = false;

	MapState = JRN_IDLE;
	MapPopupAlpha = 0.0f;

	SetZ(eZOrder_MapdiaryPopup);

	SetYouAreHereMapArea();

	Visible = true;
}

void Map::HideMap(bool isQuick)
{
	IsMapClosed = true;
	IsMapActive = false;
	MapState = JRN_IDLE;
	MapScene->Show(false);
	IsOtherSceneClicked = false;
}

void  Map::SetYouAreHereMapArea()
{
	//Only specific locations are shown in the map.
	//In case of locations which are inside/part of this main locations
	//we use the previous active location for the player
	isPlayerInMainScene = false;

	for (int i = 0; i < MaxCurrentLevelScenes; i++)
	{
		int areaID = sceneIDArray[i];

					
		if(areaID == ProfileSystem::Instance->ActiveProfile.CurrentArea)
		{
			//player is here
			isPlayerInMainScene = true;
			ProfileSystem::Instance->ActiveProfile.PrevMapActiveArea = areaID;
			break;
		}
	}
}

void Map::SetUpAndShowMap(bool isQuick)
{
	IsMapClosed = false;
	IsMapActive = false;
	IsOtherSceneClicked = false;

	bool isCollectablesOnMapEnabled = true;

	if(CurrentLevel==Level_8 || ProfileSystem::Instance->ActiveProfile.IsExpertMode)
	{
		if( !(ProfileSystem::Instance->ActiveProfile.IsExpertMode && GFInstance->IsReplayingGame) )			
			isCollectablesOnMapEnabled = false;

		if (MapScene->GetObjectByID("extraslegend") != NULL)
		{
			MapScene->GetObjectByID("extraslegend")->SetEnable(false);
			MapScene->GetObjectByID("MEMTEXT")->SetEnable(false);
			MapScene->GetObjectByID("MORPHTEXT")->SetEnable(false);
		}
	}

	for (int i = 0; i < MaxCurrentLevelScenes; i++)
	{
		SceneThumbs[i] = MapScene->GetObjectByID(std::string( "scn"+MKSTR((i+1)) ).c_str() ); //s1-sx
		MapIcons[i] = MapScene->GetObjectByID(std::string( "mapicons"+MKSTR((i+1)) ).c_str() ); //icons1-iconsx
		TaskIcons[i] = MapScene->GetObjectByID(std::string( "taskicons"+MKSTR((i+1)) ).c_str() ); //taskicon1-taskiconx
		ThumbGlows[i] = MapScene->GetObjectByID(std::string( "thumbglow"+MKSTR((i+1)) ).c_str() ); //thumbglow1-thumbglow7
		ThumbGlows[i]->SetBlendMode(eBlend_AlphaAdditive);
		ThumbGlows[i]->SetEnable(false);

		MorphIcons[i] = MapScene->GetObjectByID(std::string( "MorphIcon"+MKSTR((i+1)) ).c_str() ); //MorphIcons
		MemoryIcons[i] = MapScene->GetObjectByID(std::string( "MemoryIcon"+MKSTR((i+1)) ).c_str() ); //MemoryIcons

		MorphIcons[i]->SetEnable(false);
		MemoryIcons[i]->SetEnable(false);
		MorphIcons[i]->SetZ(eZOrder_MapdiaryPopup+500);
		MemoryIcons[i]->SetZ(eZOrder_MapdiaryPopup+501);

		if(SceneThumbs[i] == NULL || MapIcons[i] == NULL || TaskIcons[i] == NULL || ThumbGlows[i] == NULL)
		{
			#ifdef K_WIN32
				FATAL(StringTable::getString("ERR_MapNull"));////Removed Wide string conversion for Windows	
			#else
				FATAL(StringTable::getString("ERR_MapNull"));
			#endif

			return;
		}
	}

	for (int i = 0; i < MaxCurrentLevelScenes; i++)
	{
		TaskIcons[i]->SetEnable(false);
		MapIcons[i]->SetEnable(false);

		int areaID = sceneIDArray[i];

		if(!ProfileSystem::Instance->ActiveProfile.IsAreaVisited[areaID])
		{
			K_LOG("Scene %d NOT VISITED", areaID);
			SceneThumbs[i]->SetEnable(false);
		}
		else
		{
#ifdef _CEBUILD
			//Check for Morph and Memory Object in current Area
			if(isCollectablesOnMapEnabled)
			{
				if(IsMemoryItemAvaialbleToCollect(areaID))
				{
					K_LOG("Scene %d Mem %d - YES", areaID, i);
					if(MemoryIcons[i]!=NULL)
						MemoryIcons[i]->SetEnable(true);
				}
				else
				{
					K_LOG("Scene %d Mem %d - ___", areaID, i);
					if(MemoryIcons[i]!=NULL)
						MemoryIcons[i]->SetEnable(false);
				}

				if(IsMorphItemAvaialbleToCollect(areaID))
				{
					K_LOG("Scene %d Morph %d - YES", areaID, i);
					if(MorphIcons[i]!=NULL)
						MorphIcons[i]->SetEnable(true);
				}
				else
				{
					K_LOG("Scene %d Morph %d", areaID, i);
					if(MorphIcons[i]!=NULL)
						MorphIcons[i]->SetEnable(false);
				}
			}  
#endif // _CEBUILD


			SceneThumbs[i]->SetEnable(true);
			
			
			if(ProfileSystem::Instance->ActiveProfile.IsAreaLocked[areaID] )
			{				
				MapIcons[i]->SetEnable(true);								
				MapIcons[i]->PlayAnimation(MAP_ICON_LOCKED);				
			}
			else if(ProfileSystem::Instance->ActiveProfile.IsTaskLeft[areaID])
			{
				TaskIcons[i]->SetEnable(true);
				//hack to avoid wrong task icons "set" during replay - ideally no task must be set
				if( GFInstance->IsReplayingGame && ProfileSystem::Instance->ActiveProfile.CurrentLevel <= Level_7 )
					TaskIcons[i]->PlayAnimation(MAP_ICON_NIL);
				else
					TaskIcons[i]->PlayAnimation(MAP_ICON_TASKPENDING);

				if((isPlayerInMainScene && areaID == ProfileSystem::Instance->ActiveProfile.CurrentArea) || (!isPlayerInMainScene && areaID == ProfileSystem::Instance->ActiveProfile.PrevMapActiveArea))
				{
					MapIcons[i]->SetEnable(true);
					MapIcons[i]->PlayAnimation(MAP_ICON_PLAYERHERE);						
				}
			}
			else if((isPlayerInMainScene && areaID == ProfileSystem::Instance->ActiveProfile.CurrentArea) || (!isPlayerInMainScene && areaID == ProfileSystem::Instance->ActiveProfile.PrevMapActiveArea))
			{
				//player is here
				MapIcons[i]->SetEnable(true);
				MapIcons[i]->PlayAnimation(MAP_ICON_PLAYERHERE);
			}
		}
	}

	//New map task pending indicators during replaying game!!
	if( GFInstance->IsReplayingGame )
	{
		//Morphing objects/Memory collectables left to pick in current level map regions!!
		switch( ProfileSystem::Instance->ActiveProfile.CurrentLevel )
		{
			case Level_1://7 worlds
			//eArea_L1_S1_Beach
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected )
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L1_S2_Frontyard
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy] )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L1_S3_Foyer - eArea_L1_S3_PZShelf - eArea_L1_S3_Foyer
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck] )
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L1_S4_Bedroom
			if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves] )
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);			
			//eArea_L1_S5_TreasureRoom
			//MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);			
			//eArea_L1_S6_BoatShed
			if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll] )
				MapIcons[5]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L1_S7_BoatDock
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected )
				MapIcons[6]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_2://7 worlds
			//eArea_L2_S1_ForestEntrance
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected )
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L2_S2_NagaTemple
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L2_S3_CaveMouth				
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue] )
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L2_S4_Cave
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L2_S5_MoopansDen
			if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope] ) 
				MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);	
			//eArea_L2_S6_TunnelEnd
			if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles] )
				MapIcons[5]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L2_S7_PZSoldiers
			//MapIcons[6]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_3://6 worlds
			//eArea_L3_S1_FortEntrance
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror] )
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L3_S2_FortInterior
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing] )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L3_S2_P1Room1Start
			//MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L3_S3_Pool -eArea_L3_S3_PZBlocks
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S3Shoes])
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			////eArea_L3_S4_FortFountain
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
				MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L3_S5_Cliff
			if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg] )
				MapIcons[5]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_4://5 worlds
			//eArea_L4_S1_SwampEntrance
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin] )
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L4_S2_RiverSide
			if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana] )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L4_S3_Cave
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L4_S4_HillTop 
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L4_S5_HillSide
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
				MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_5://6 worlds
			//eArea_L5_S1_TigerDistant
			//MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L5_S2_TigerCloseup
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone] )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L5_S3_Pond
			if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue] )
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L5_S4_Dungeon
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L5_S5_MountainBelow 
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
				MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L5_S6_MountainTop
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected )
				MapIcons[5]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_6://5 worlds
			//eArea_L6_S1_FloatingIsland1 
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L6_S2_FloatingIsland2  
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L6_S3_FloatingIsland3  
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush]) 
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L6_S4_FloatingIsland4  
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue]) 
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L6_S5_MarysDream
			if( !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal])
				MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
			case Level_7://5 worlds
			//eArea_L7_S1_Volcano
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected )
				MapIcons[0]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L7_S2_LasyaPrison
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected )
				MapIcons[1]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L7_S3_Haven 
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected )
				MapIcons[2]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L7_S4_Ameeha 
			if( !ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected )
				MapIcons[3]->PlayAnimation(MAP_ICON_TASKPENDING);
			//eArea_L7_S5_SophiaEnd
			//MapIcons[4]->PlayAnimation(MAP_ICON_TASKPENDING);
			break;
		}			
	}
	
	if(isQuick)
	{
		MapPopupAlpha = 1.0f;

		MapScene->Show(true);
		MapState = JRN_POPPED;

		IsMapActive = true;
	}
	else
	{
		//Start MAP Transition
		MapPopupAlpha = 0.13f;

		MapState = JRN_POPUP;
		InitRenderTarget(MapScene);
		GFInstance->SetPopupTween( -500,320,171,0);
		GFInstance->CreateGreyscaleScene();

		IsTransitionActive = true;
	}
}

bool Map::IsMemoryItemAvaialbleToCollect(int areaID)
{
	bool isMemoryActive = false;

	if(CurrentLevel == Level_1)
	{
		switch (areaID)
		{
			case eArea_L1_S1_Beach: 
				if(ProfileSystem::Instance->ActiveProfile.IsAreaVisited[eArea_L1_S2_Frontyard] && !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S1Teddy])
					isMemoryActive = true;
				break;
			case eArea_L1_S3_Foyer: 
				if(ProfileSystem::Instance->ActiveProfile.L1_IsMansionPowerOn && 
					(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3MaryLamb] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S3RibbonNeck]))
					isMemoryActive = true;
				break;
			case eArea_L1_S4_Bedroom: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S4BoxingGloves])
					isMemoryActive = true;
				break;
			case eArea_L1_S7_BoatDock: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L1S6ChappathiRoll])
					isMemoryActive = true;
				break; 
		}

	}
	else if(CurrentLevel == Level_2)
	{
		switch (areaID)
		{
			case eArea_L2_S4_Cave: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S4TigerMask])
					isMemoryActive = true;
				break;
			case eArea_L2_S5_MoopansDen: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Statue] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5Stethoscope]
					|| !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S5HoneyBottle])
					isMemoryActive = true;
				break;	
			case eArea_L2_S6_TunnelEnd: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L2S6Spectacles])
					isMemoryActive = true;
				break;
		}
	}
	else if(CurrentLevel == Level_3)
	{
		switch (areaID)
		{
			case eArea_L3_S1_FortEntrance: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S1MemMirror])
					isMemoryActive = true;
				break;
			case eArea_L3_S2_FortInterior: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S2SunDrawing])
					isMemoryActive = true;
				break;
			case eArea_L3_S2_P1Room1Start: break;
			case eArea_L3_S3_Pool: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S3Shoes])
					isMemoryActive = true;
				break;
			case eArea_L3_S4_FortFountain:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S4CatStatue])
					isMemoryActive = true;
				break;	
			case eArea_L3_S5_Cliff:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L3S5EasterEgg])
					isMemoryActive = true;
				break; 
		}
	}
	else if(CurrentLevel == Level_4)
	{
		switch (areaID)
		{
			case eArea_L4_S1_SwampEntrance:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S1Pumpkin])
					isMemoryActive = true;
				break;
			case eArea_L4_S2_RiverSide: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S2Banana])
					isMemoryActive = true;
				break;
			case eArea_L4_S3_Cave:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S3Honey])
					isMemoryActive = true;
				break;
			case eArea_L4_S4_HillTop: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S4Colorleaf])
					isMemoryActive = true;
				break;
			case eArea_L4_S5_HillSide:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L4S5StarLocket])
					isMemoryActive = true;
				break; 
		}
	}
	else if(CurrentLevel == Level_5)
	{
		switch (areaID)
		{
			case eArea_L5_S2_TigerCloseup: 
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2DogStatue] || !ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S2FootStone])
					isMemoryActive = true;
				break;
			case eArea_L5_S3_Pond:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S3AngelStatue])
					isMemoryActive = true;
				break;
			case eArea_L5_S4_Dungeon:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S4Watermelon])
					isMemoryActive = true;
				break;
			case eArea_L5_S5_MountainBelow:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
					isMemoryActive = true;
				break;
		}
	}
	else if(CurrentLevel == Level_6)
	{
		switch (areaID)
		{
			case eArea_L6_S1_FloatingIsland1:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S1LocketChain])
					isMemoryActive = true;
				break;
			case eArea_L6_S2_FloatingIsland2:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S2MotherkidStatue])
					isMemoryActive = true;
				break;
			case eArea_L6_S3_FloatingIsland3:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S3PaintBrush])
					isMemoryActive = true;
				break;
			case eArea_L6_S4_FloatingIsland4:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S4MotherStatue])
					isMemoryActive = true;
				break;
			case eArea_L6_S5_MarysDream:
				if(!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L6S5KidSandal])
					isMemoryActive = true;
				break; 
		}
	}
	/*
	else if(CurrentLevel == Level_7)
	{
		switch (areaID)
		{
			case eArea_L7_S1_Volcano: break;
			case eArea_L7_S2_LasyaPrison: break;
			case eArea_L7_S3_Haven: break;
			case eArea_L7_S4_Ameeha: break;
			case eArea_L7_S5_SophiaEnd: break; 
		}
	}
	else if(CurrentLevel == Level_8)
	{

	}
	*/
	return isMemoryActive;
}

bool Map::IsMorphItemAvaialbleToCollect(int areaID)
{
	bool isMorphActive = false;

	if(CurrentLevel == Level_1)
	{
		switch(areaID)
		{
			case eArea_L1_S1_Beach:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S1Collected) isMorphActive = true;	break;
			case eArea_L1_S2_Frontyard:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S2Collected) isMorphActive  = true;	break;
			case eArea_L1_S3_Foyer:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S3Collected) isMorphActive  = true;	break;
			case eArea_L1_S6_BoatShed:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL1S7Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_2)
	{
		switch(areaID)
		{
			case eArea_L2_S1_ForestEntrance:	if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S1Collected) isMorphActive  = true;	break;
			case eArea_L2_S2_NagaTemple:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S2Collected) isMorphActive  = true;	break;
			case eArea_L2_S3_CaveMouth:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S3Collected) isMorphActive  = true;  break;
			case eArea_L2_S4_Cave:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL2S4Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_3)
	{
		switch(areaID)
		{
			case eArea_L3_S1_FortEntrance:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S1Collected) isMorphActive  = true;	break;
			case eArea_L3_S2_FortInterior:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S2Collected) isMorphActive  = true;	break;
			case eArea_L3_S3_Pool:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S3Collected) isMorphActive  = true;	break;
			case eArea_L3_S4_FortFountain:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL3S4Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_4)
	{
		switch(areaID)
		{
			case eArea_L4_S1_SwampEntrance:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S1Collected) isMorphActive  = true;	break;
			case eArea_L4_S3_Cave:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S3Collected) isMorphActive  = true;	break;	
			case eArea_L4_S4_HillTop:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S4Collected) isMorphActive  = true;	break;
			case eArea_L4_S5_HillSide:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL4S5Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_5)
	{
		switch(areaID)
		{
			case eArea_L5_S2_TigerCloseup:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S2Collected) isMorphActive  = true;	break;
			case eArea_L5_S4_Dungeon:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S4Collected) isMorphActive  = true;	break;	
			case eArea_L5_S5_MountainBelow:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S5Collected) isMorphActive  = true;	break;
			case eArea_L5_S6_MountainTop:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL5S6Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_6)
	{
		switch(areaID)
		{
			case eArea_L6_S1_FloatingIsland1:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S1Collected) isMorphActive  = true;	break;
			case eArea_L6_S2_FloatingIsland2:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S2Collected) isMorphActive  = true;	break;	
			case eArea_L6_S3_FloatingIsland3:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S3Collected) isMorphActive  = true;	break;
			case eArea_L6_S4_FloatingIsland4:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL6S4Collected) isMorphActive  = true;	break;
		}
	}
	else if(CurrentLevel == Level_7)
	{
		switch(areaID)
		{
			case eArea_L7_S1_Volcano:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S1Collected) isMorphActive  = true;	break;
			case eArea_L7_S2_LasyaPrison:		if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S2Collected) isMorphActive  = true;	break;	
			case eArea_L7_S3_Haven:				if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S3Collected) isMorphActive  = true;	break;
			case eArea_L7_S4_Ameeha:			if(!ProfileSystem::Instance->ActiveProfile.IsMorphObjL7S4Collected) isMorphActive  = true;	break; 
		}
	}
	/*
	else if(CurrentLevel == Level_8)
	{
	}
	*/
	return isMorphActive;
}

Map::~Map()
{
	int numScenes = (int)sceneRectArray.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < numScenes; i++)
	{
		SAFE_DELETE(sceneRectArray[i]);
	}
	sceneRectArray.clear();
	sceneIDArray.clear();

	for (int i = 0; i < 10; i++){
		SceneThumbs[i] = NULL;
		MapIcons[i] = NULL;
		TaskIcons[i] = NULL;
		ThumbGlows[i] = NULL;

		MorphIcons[i] = NULL;
		MemoryIcons[i] = NULL;
	}

	if( MapScene!=NULL)
		MapScene->Show(false);
	MapScene = NULL;
	
	if( GFInstance != NULL ){
		if( !GFInstance->IsPartialCacheMem )
		{
			GFInstance->UnloadBufferScene(MAP_SCN);
			GFInstance->UnloadBufferScene(MAP_SCNL2);
		}

		if (CurrentLevel == Level_1)
		{
			GFInstance->UnloadBufferScene(MAP_SCN);
		}
		else if (CurrentLevel == Level_2)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL2);
		}
		else if (CurrentLevel == Level_3)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL3);
		}
		else if (CurrentLevel == Level_4)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL4);
		}
		else if (CurrentLevel == Level_5)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL5);
		}
		else if (CurrentLevel == Level_6)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL6);
		}
		else if (CurrentLevel == Level_7)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL7);
		}
		else if (CurrentLevel == Level_8)
		{
			GFInstance->UnloadBufferScene(MAP_SCNL8);
		}
		else
		{
#ifdef K_WIN32
			WARN(StringTable::getString("ERR_MapUndef"));////Removed Wide string conversion for Windows	
#else
			WARN(StringTable::getString("ERR_MapUndef"));
#endif
		}
	}
}

void Map::Required()
{
	if(IsTransitionActive)
	{
		switch (MapState)
		{
			case JRN_POPUP:
				MapPopupAlpha += POPUP_SCALE_FACTOR *  Control::LogicRate;
				if( MapPopupAlpha >= 1.0f )
				{
					MapPopupAlpha = 1.0f;
					MapScene->Show(true);
					MapState = JRN_POPPED;

					IsTransitionActive = false;

					if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && !ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT15_MapDetailsTutorial] && !ProfileSystem::Instance->ActiveProfile.TutorialSpecialFeaturesOnly)
					{						
						GFHud->Tutor->ShowTutorial(TUT15_MapDetailsTutorial);
					}

					IsMapActive = true;
				}
				if( MapPopupAlpha < 0.5f )
					GFInstance->UpdateBlurScene(2*MapPopupAlpha);
				else
					GFInstance->UpdateBlurScene(1.0f);				
			break;

			case JRN_POPDOWN:
				MapPopupAlpha -= POPUP_SCALE_FACTOR * Control::LogicRate;
				GFInstance->UpdateBlurScene(MapPopupAlpha);
				if( MapPopupAlpha <= 0.13f )
				{				
					MapPopupAlpha = 0.13f;			
					GFInstance->TurnOffBlurScene();
					MapState = JRN_IDLE;

					IsTransitionActive = false;

					IsMapClosed = true;
				}
			break;

		default:
			break;
		}
	}
}



void Map::Render(const CPoint* pCam)
{
	if(!IsTransitionActive)
		return;
	//Draw render target object in Render as last object!!!
	if( MapState == JRN_POPUP || MapState == JRN_POPDOWN )
	{

		//Init again if render target lost due to fullscreen-window switches or similar
		if( GFInstance->RTGraphic != NULL )
			GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
			short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*MapPopupAlpha),
			short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*MapPopupAlpha),
			-5*(1.0f-MapPopupAlpha),
			MapPopupAlpha,MapPopupAlpha);
    }
}

void Map::Update()
{
	if(IsMapActive)
	{
		Cursor::SetMode(CA_Normal);

		CPoint mosPos = Control::Input->Mpos();
		bool clicked = Control::Input->LBclicked();

		if(GFHud->Tutor != NULL && GFHud->Tutor->IsActive && GFHud->Tutor->IsVisible)
		{
			if(clicked)
			{
				ProfileSystem::Instance->ActiveProfile.IsTutorialShown[TUT15_MapDetailsTutorial] = true;
				GFHud->Tutor->HideTutorial();
			}
			return;
		}
		/*
		MapScene->GetObjectByID("btnjournaltext")->SetEnable(true);
		MapScene->GetObjectByID("btnmaptext")->SetEnable(true);
		MapScene->GetObjectByID("btnobjectivetext")->SetEnable(true);
		MapScene->GetObjectByID("btncharactertext")->SetEnable(true);
		*/

		//Objective or Journal. Call the Diary Popup instantly.
		if( BtnRectJournalTab.Intersection(&mosPos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabJournal->PlayAnimation(1);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				BtnTabJournal->PlayAnimation(0);
				ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = false;

				GFHud->ShowJournalFromMap();
				return;
			}
		}
		else
			BtnTabJournal->PlayAnimation(0);

		if( BtnRectObjectivesTab.Intersection(&mosPos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabObjective->PlayAnimation(1);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				BtnTabObjective->PlayAnimation(0);
				ProfileSystem::Instance->ActiveProfile.IsJObjBtnActive = true;

				GFHud->ShowJournalFromMap();

				return;
			}
		}
		else
			BtnTabObjective->PlayAnimation(0);

		//Character Page
		if( BtnRectCharactersTab.Intersection(&mosPos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnTabCharacter->PlayAnimation(1);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaTabClick);

				BtnTabCharacter->PlayAnimation(0);

				GFHud->ShowJournalFromMap(true);
				return;
			}
		}
		else
			BtnTabCharacter->PlayAnimation(0);


	#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			Control::Audio->QuickLoadAndPlaySFX(aSFXDiaClose);
			MapScene->GetObjectByID("closebtn")->PlayAnimation(0);
			Control::Input->ClearMouse();

			InitRenderTarget(MapScene);

			MapScene->Show(false);
			MapScene->GetObjectByID("closebtn")->Show(false);
			MapPopupAlpha = 1.0f;
			MapState = JRN_POPDOWN;
			IsMapActive = false;

			IsTransitionActive = true;
		}
	#endif
		MapScene->GetObjectByID("closebtn")->PlayAnimation(0);
		if(CloseBtnRect.Intersection(&mosPos) || !MapAreaRect.Intersection(&mosPos))
		{
			MapScene->GetObjectByID("closebtn")->PlayAnimation(1);
			Cursor::SetMode(CA_HandPointy);
			if(clicked)
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXDiaClose);

				MapScene->GetObjectByID("closebtn")->PlayAnimation(0);
				Control::Input->ClearMouse();

				InitRenderTarget(MapScene);

				MapScene->Show(false);
				MapScene->GetObjectByID("closebtn")->Show(false);

				MapPopupAlpha = 1.0f;
				MapState = JRN_POPDOWN;
				IsMapActive = false;

				IsTransitionActive = true;
			}
		}
		else
		{
			for (int i = 0; i < MaxCurrentLevelScenes; i++)
			{
				if( SceneThumbs[i]->IsActive && sceneRectArray[i] != NULL && sceneRectArray[i]->Intersection(&mosPos) )
				{
					if(!ThumbGlows[i]->IsActive)
						Control::Audio->QuickLoadAndPlaySFX(aSFXMapMouseOver);
					ThumbGlows[i]->SetEnable(true);
					int areaID = sceneIDArray[i];

					Cursor::SetMode(CA_ExitUp, areaID);
					if(clicked)
					{
						if(MapIcons[i]->IsActive && MapIcons[i]->GetAnimation() == MAP_ICON_LOCKED)
						{
							MapIcons[i]->Shake(45,3,0,0.25f);
						}
						else
						{
							Control::Input->ClearMouse();

							if( ProfileSystem::Instance->ActiveProfile.CurrentArea == areaID )
							{
								//Just close it. Same scene clicked
								InitRenderTarget(MapScene);

								MapScene->Show(false);
								MapScene->GetObjectByID("closebtn")->Show(false);

								MapPopupAlpha = 1.0f;
								MapState = JRN_POPDOWN;

								IsTransitionActive = true;

								IsMapActive = false;
								IsMapClosed = true;
								return;
							}
							else
							{
								Control::Audio->QuickLoadAndPlaySFX(aSFXMapClick);
								Control::Audio->PlayFootStepsFX();
								IsMapActive = false;
								AwardManager::GetInstance()->CollectAward(kAward_FastTravel);
								ProfileSystem::Instance->ActiveProfile.CurrentArea = areaID;
								IsMapClosed = true;
								IsOtherSceneClicked = true;
								return;
							}
						}
					}
					break;
				}
				else
					ThumbGlows[i]->SetEnable(false);
			}
		}
	}
}

void  Map::InitRenderTarget(CHOScene *Popup)
{
	if( GFInstance->IsRTCreated )
	{		
		GFInstance->RTGraphic->clearRenderTarget();   
		GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS	
		GFInstance->IsRenderingToRT = true;
    }
		Popup->RenderToTarget(-171.0f);
    if( GFInstance->IsRTCreated )
    {
		GFInstance->IsRenderingToRT = false;
		GFInstance->RTGraphic->endRenderToTarget();		
	}
}

//====================================
//  created by : Faisal
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S5_Pop1BambooLadder.h"
#include "Hud.h"

const CRectangle  ActionRectStone1(484.0f,300.0f,289.0f,227.0f);
const CRectangle  ActionRectStone2(566.0f,239.0f,294.0f,255.0f);
const CRectangle  ActionRectMemoryAmeeha(609.0f,220.0f,234.0f,145.0f);
const CRectangle  InvItemRectStoneEmblem(683.0f,230.0f,127.0f,111.0f);
const CRectangle  ActionRectPillar(427.0f,97.0f,485.0f,385.0f);
const CRectangle  InvItemRectBamboo(361.0f,232.0f,649.0f,256.0f);

const CRectangle  MemoryRectFlower(490.0f,166.0f,93.0f,82.0f);

const CRectangle  PaptusFlyRect(658.0f,289.0f,61.0f,68.0f);

L5S5_Pop1BambooLadder::L5S5_Pop1BambooLadder(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S5\\POP1\\P1.SCN");
	SetFrameAndCloseButton();

	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);

	Show(false);

	clickTimer = -1;

	int stone1pts[] = {529,319,739,292,797,365,701,509,543,529,469,430};
	stone1poly=  new CPolygon(6);
	stone1poly->Init(stone1pts);
	int stone2pts[] = {594,238,699,233,844,359,877,445,828,489,703,497,557,382,562,271};
	stone2poly=  new CPolygon(8);
	stone2poly->Init(stone2pts);
	int pillarpts[] = {512,90,552,93,912,289,759,493,647,390,427,197};
	pillarpoly=  new CPolygon(6);
	pillarpoly->Init(pillarpts);
	int bamboopts[] = {360,250,363,364,1006,484,1007,364};
	bamboopoly=  new CPolygon(4);
	bamboopoly->Init(bamboopts);

	GetObjectByID("paptus")->SetBlendMode(eBlend_AlphaAdditive);
	if(ProfileSystem::Instance->ActiveProfile.IsPaptusL5S5Collected)
	{
		GetObjectByID("paptus")->SetEnable(false);
	}


	GetObjectByID("violetglow")->Animate(0.25f,0.95f,3.0f,0,eBlend_AlphaAdditive);

	_waterMesh = new MeshObject(GetObjectByID("meshwater"), 18, 5);
	_waterMesh->meshFX->SetSpeedAndDisplacement(5, 2, 5, 3);
	_waterMesh->meshFX->setWaterwaves(true);
	_waterMesh->Show(false);
	InsertObject(_waterMesh, 2);

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone1Removed)
	{
		GetObjectByID("stone1")->SetEnable(false);
		GetObjectByID("stone1water")->SetEnable(false);
	}
	else
		GetObjectByID("stone1water")->Animate(0, 2, 0, 0, 3.0f);

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone2Removed)
	{
		GetObjectByID("stone2")->SetEnable(false);
		GetObjectByID("stone2water")->SetEnable(false);
	}
	else
		GetObjectByID("stone2water")->Animate(0, 2, 0, 0, 4.0f, 1.25f);

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved)
	{
		GetObjectByID("piller")->SetEnable(false);
		GetObjectByID("pillerwater")->SetEnable(false);
	}
	else
		GetObjectByID("pillerwater")->Animate(0, 2, 0, 0, 4.25f, 1.25f);

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed)
	{
		GetObjectByID("diarynote")->SetEnable(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
	{
		GetObjectByID("bamboo")->SetEnable(false);
		GetObjectByID("bamboowater")->SetEnable(false);
	}

	GetObjectByID("bamboowater")->Animate(0, 1, 0, 0, 4.75f);

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken)
		GetObjectByID("stoneemblem")->SetEnable(false);

	cv_AmeehaCutscn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 17;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary6"));
	subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary7"));
	subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary8"));
	subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary9"));
	subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary10"));
	subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary11"));
	subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary12"));
	subtitles[12] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary13"));
	subtitles[13] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary14"));
	subtitles[14] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary15"));
	subtitles[15] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary16"));
	subtitles[16] = AssetsStringTable::getSub(AssetsStringTable::getString("lv5ameehadiary17"));

	vidTimer = 0;

	isAmeehaCutscnPlaying = false;
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed)
	{
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		cv_AmeehaCutscn = new CVideo("OGV\\L5\\S5AMEEHADIARY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//cv_AmeehaCutscn->SetScale(1.334f);
		cv_AmeehaCutscn->SetScale(GFApp->video_aspectRatio);
		cv_AmeehaCutscn->SetSkipActive(); 
	}

	if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
	{
		GetObjectByID("memflower")->SetEnable(false);
	}
}

void L5S5_Pop1BambooLadder::Required()
{

}

void L5S5_Pop1BambooLadder::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if (isAmeehaCutscnPlaying)
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = cv_AmeehaCutscn->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");

		if (time > subtitles[subIndex].end)
		{
			if (subMax - 1 > subIndex)
				subIndex++;
		}
#ifdef FORCE_CLOSE_VIDEO
		vidTimer += Control::LogicRate;
#endif
		if(
#ifndef FORCE_CLOSE_VID_END
			cv_AmeehaCutscn->isEndReached() ||
#endif
			vidTimer > 96)
		{
			isAnimationPlaying = false;
			subtitleText->SetText("");
			ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed = true;

			isAmeehaCutscnPlaying = false;
			cv_AmeehaCutscn->StopAndFadeOutVideo(2.0f);
			Hud->AllowInput = true;
			Hud->ShowHud();

			Hud->ShowBannerText("L5anantha");

			//GetObjectByID("diarynote")->SetEnable(false);

			isAnimationPlaying = true;

			float valArr[5] = {0, 220, 667, 0.45f, 15.0f};
			GetObjectByID("diarynote")->TweenTo(valArr, 1.0f, TW_EASEQUADIN, true, CC_CALLBACK_1(L5S5_Pop1BambooLadder::OnDiaryNoteTweenComplete, this));
		} 
		return;
	}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	if(clickTimer>0)
	{
		clickTimer -= Control::LogicRate;
		return;
	}

	if(ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed && !ProfileSystem::Instance->ActiveProfile.IsPaptusL5S5Collected && PaptusFlyRect.Intersection(&pos))
	{
		if(Control::Input->LBclicked())
		{
			Control::Audio->PlaySample(aSFXMagic2);
			GetObjectByID("paptus")->FadeOut(1.25f);
			Hud->CollectMysteryFlower(eArea_L5_S5_MountainBelow, GetObjectByID("paptus")->Pos);
			return;
		}
	}

	//handle interactions here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone1Removed)
	{
		if(stone1poly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s5p1_rockmove1");

				ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone1Removed = true;
				//GetObjectByID("stone1")->TweenPosTo(-70, -80, -1, 1.0f, TW_EASEBACKIN, true);

				float valArr[5] = {0, GetObjectByID("stone1")->Pos.x-70, GetObjectByID("stone1")->Pos.y-30, 1.05f, 45.0f};
				GetObjectByID("stone1")->TweenTo(valArr, 0.75f, TW_EASEQUADIN, true);
				GetObjectByID("stone1water")->FadeOut(2.0f);

				clickTimer = 0.25f;
			}
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone2Removed)
	{
		if(stone2poly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s5p1_rockmove2");

				ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone2Removed = true;
				//GetObjectByID("stone2")->TweenPosTo(-70, -80, -1, 1.0f, TW_EASEBACKIN, true);
				float valArr[5] = {0, GetObjectByID("stone2")->Pos.x-80, GetObjectByID("stone2")->Pos.y-30, 1.05f, 25.0f};
				GetObjectByID("stone2")->TweenTo(valArr, 1.15f, TW_EASEQUADIN, true);

				GetObjectByID("stone2water")->FadeOut(1.0f);

				clickTimer = 0.25f;
			}
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed)
	{
		if(ActionRectMemoryAmeeha.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Hud->AllowInput = false;
				Hud->HideHud();		

				cv_AmeehaCutscn->PlayFadeInVideo(2.0f);

				isAmeehaCutscnPlaying = true;
				isAnimationPlaying = true;
			}
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken)
	{
		if(InvItemRectStoneEmblem.Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken = true;
				GetObjectByID("stoneemblem")->SetEnable(false);
				Hud->TakeInventoryItem(eInv_L5_S5_StoneEmblem);
				ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S3_Pond] = true;
				clickTimer = 0.45f;
			}
			return;
		}
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved)
	{
		if(pillarpoly->Intersection(&pos))
		{
			Cursor::SetMode(CA_HandTake);
			if(Control::Input->LBclicked())
			{
				Control::Audio->QuickLoadAndPlaySFX("s5p1_pillarmove");

				ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved = true;
				//GetObjectByID("piller")->TweenPosTo(-120, -180, -1, 1.5f, TW_EASEBACKIN, true);
				float valArr[5] = {0, GetObjectByID("piller")->Pos.x-170, GetObjectByID("piller")->Pos.y+100, 0.85f, 35.0f};
				GetObjectByID("piller")->TweenTo(valArr, 1.25f, TW_EASEQUADIN, true);

				GetObjectByID("pillerwater")->FadeOut(1.0f);

				clickTimer = 0.65f;
			}
			return;
		}
	}
	
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
	{
		if(bamboopoly->Intersection(&pos))
		{
			if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved)
			{
				Cursor::SetMode(CA_Gear);
				if(Control::Input->LBclicked())
				{
					GetObjectByID("bamboo")->Shake(90, 3, 0, 0.35f);
					Hud->ShowBannerText("L5oof");
				}
			}
			else
			{
				Cursor::SetMode(CA_HandTake);
				if(Control::Input->LBclicked())
				{
					ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken = true;
					GetObjectByID("bamboo")->SetEnable(false);
					Hud->TakeInventoryItem(eInv_L5_S5_BambooLadder);

#ifdef _CEBUILD
					if (ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower])
					{
						ClosePopup(1.25f);
					}
#else
					ClosePopup(1.25f);
#endif 

				} 
			}
		}
	}

#ifdef _CEBUILD
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower] && ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved && MemoryRectFlower.Intersection(&pos))
	{
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memflower")->FadeOut();
			Hud->ShowMemoryObjectCollected_CE(eMem_L5S5Flower, &GetObjectByID("memflower")->Pos);

			if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
			{
				GetObjectByID("memflower")->SetEnable(false);
				ClosePopup(1.25f);
			}
		}
	}
#else
	if (!ProfileSystem::Instance->ActiveProfile.IsMemoryCollected[eMem_L5S5Flower] && ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved && MemoryRectFlower.Intersection(&pos))
	{
		Cursor::SetMode(CA_Help);
		if( Control::Input->LBclicked() )
		{
			GetObjectByID("memflower")->FadeOut();
			Hud->ShowMemoryObjectCollected_SE(eMem_L5S5Flower, &(GetObjectByID("memflower")->Pos));
		}
	}
#endif 
}

void L5S5_Pop1BambooLadder::OnDiaryNoteTweenComplete(CObject* obj)
{
	GetObjectByID("diarynote")->SetEnable(false);
	isAnimationPlaying = false;

	//Journal Entry Add
	if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P5_AmeehaDiaryNote])
	{
		//Flag Set
		ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P5_AmeehaDiaryNote] = true;

		//Add to Note Array
		ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P5_AmeehaDiaryNote;

		ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

		//Increment Notes
		ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
	}

	//Show New Info added
	Hud->ShowNewInfoTextBanner();
}

void L5S5_Pop1BambooLadder::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	GFInstance->WrongInvDropMsg(Hud);
}

void L5S5_Pop1BambooLadder::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	// hint code goes here
	//handle interactions here
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone1Removed)
	{
		Hud->InitHintTrailEmit(&ActionRectStone1);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1Stone2Removed)
	{
		Hud->InitHintTrailEmit(&ActionRectStone2);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsAmeehaDiaryCutscenePlayed)
	{
		Hud->InitHintTrailEmit(&ActionRectMemoryAmeeha);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectStoneEmblem);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_IsP1PillerRemoved)
	{
		Hud->InitHintTrailEmit(&ActionRectPillar);
	}
	else if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
	{
		Hud->InitHintTrailEmit(&InvItemRectBamboo);
	}
	else
	{
		//Shows Close Button
		Popup::ProcessHint();
	}
}

bool L5S5_Pop1BambooLadder::isTaskLeft()
{
	bool isAnyTaskLeft = false;
	
	if(!ProfileSystem::Instance->ActiveProfile.L5_S5_InvBambooLadderTaken)
		isAnyTaskLeft = true;

	return isAnyTaskLeft;
}

L5S5_Pop1BambooLadder::~L5S5_Pop1BambooLadder()
{
	SAFE_DELETE(stone1poly);
	SAFE_DELETE(stone2poly);
	SAFE_DELETE(pillarpoly);
	SAFE_DELETE(bamboopoly);
	SAFE_DELETE(cv_AmeehaCutscn);
	SAFE_DELETE(subtitleText);
}

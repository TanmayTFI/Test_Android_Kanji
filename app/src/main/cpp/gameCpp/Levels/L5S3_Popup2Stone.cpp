
//====================================
//  created by : Jithin M
//  copyright  : Tuttifrutti Games
//====================================

#include "L5S3_Popup2Stone.h"
#include "Hud.h"
#include "MeshObject.h"

const CRectangle  PopupClickArea(626.0f,165.0f,309.0f,249.0f);
const CRectangle  PaperRect(681.0f,223.0f,211.0f,221.0f);
const CRectangle  PopupItemDropArea(780.0f,352.0f,118.0f,107.0f);

L5S3_PopStone::L5S3_PopStone(int popupId, HudControl *hud, CHOScene * scene,  int zOrder, CPoint zoomOrigin, std::function<void (Popup*)> popupCloseCallback) : Popup( popupId, hud, scene, zOrder,  zoomOrigin, popupCloseCallback)
{
	//Popup load
	Load("LV5\\S3\\POP2\\P2.SCN");
	SetFrameAndCloseButton();
	SetZOff(zOrder);
	SetPosOff(683.0f, 334.0f);
	SetPopupBoundingBox(GetObjectByID("bg"));
	Control::Audio->LoadSample(aVOL5MaryRefS3_1,	AudioVO);
	MeshObject* waterObj = new MeshObject(GetObjectByID("waterMesh"), 20, 20);
	waterObj->meshFX->SetSpeedAndDisplacement(4, 9, 5, 2);
	InsertObject(waterObj, 1);

	_paper = GetObjectByID("paper_1");
	_paper->SetEnable(false);
	GetObjectByID("star")->SetEnable(false);
	GetObjectByID("effect")->SetEnable(false);
	GetObjectByID("stone_3")->SetEnable(false);

	_janim_StoneBreak = NULL;
	_janim_PaperOpen = NULL;

	_clothMemoryVdo = NULL;

	time = 0;
	subIndex = 0;
	subMax = 6;
	vidTimer = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory1"));
	subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory2"));
	subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory3"));
	subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory4"));
	subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory5"));
	subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("clothmemory6"));

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
	{			
		//CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY,  GFApp->video_width,  GFApp->video_height);
		_clothMemoryVdo = new CVideo("OGV\\L5\\S3CLOTHMEMORY.OGV", RectVidFull, false, eZOrder_CutsceneFull);
		//_clothMemoryVdo->SetScale(1.334f);
		_clothMemoryVdo->SetScale(GFApp->video_aspectRatio);
		_clothMemoryVdo->SetSkipActive();
	}
	else
	{
		GetObjectByID("cloth")->SetEnable(false);
	}

	_state = L5S3_Pop2_EmblemPlaced;

	if (!ProfileSystem::Instance->ActiveProfile.L5_S3_EmblemPlacedOnStone)
	{
		_state = L5S3_Pop2_Initial;

		_janim_StoneBreak = new JSONAnimator(1, CC_CALLBACK_1(L5S3_PopStone::OnAnimationComplete, this));
		_janim_StoneBreak->parseAndLoadJSON("ANIM\\L5\\S3_STONESTAROPEN.JSON",this);

		GetObjectByID("paper_2")->SetEnable(false);
		GetObjectByID("stone_1")->SetEnable(false);
		GetObjectByID("stone_2")->SetEnable(false);
		GetObjectByID("stone_3")->SetEnable(true);
	}

	if (ProfileSystem::Instance->ActiveProfile.L5_S3_PaperCollectedFromStone)
	{
		_state = L5S3_Pop2_PaperTaken;

		_paper->SetEnable(true);
		GetObjectByID("paper_2")->SetEnable(false);
	}
	else
	{
		_janim_PaperOpen = new JSONAnimator(2, CC_CALLBACK_1(L5S3_PopStone::OnAnimationComplete, this));
		_janim_PaperOpen->parseAndLoadJSON("ANIM\\L5\\S3_PAPEROPEN.JSON",this);
	}

	Show(false);
}

void L5S3_PopStone::StartMemoryCutscene()
{
	_state = L5S3_Pop2_PlayingMemVdo;
	_clothMemoryVdo->PlayFadeInVideo(2);
	Hud->EnableInput(false);
	Hud->HideHud(false);
	ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken = true;
}

void L5S3_PopStone::Required()
{

}

void L5S3_PopStone::Update(float ds)
{
	Popup::Update(ds);
	if(GetPopupState()!= ePopupState_Open || GFInstance->IsActivePopup || Hud->IsDraggingItem() || Hud->IsComboInvPopupShowing)
		return;

	if (_state == L5S3_Pop2_PlayingMemVdo)
	{
		Cursor::SetMode(CA_Normal);
		//Subtitles Code
		time = _clothMemoryVdo->GetTime();

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
		if (
#ifndef FORCE_CLOSE_VID_END
			_clothMemoryVdo->isEndReached() ||
#endif
			vidTimer > 36)
		{
			subtitleText->SetText("");
			_clothMemoryVdo->StopAndFadeOutVideo(2);
			GetObjectByID("cloth")->FadeOut();
			_state = L5S3_Pop2_Initial;
			isAnimationPlaying = false;
			Hud->AllowInput = true;
			Hud->ShowHud(false);
			Hud->ShowBannerText(Banner_L5_S3_SophiaCutSceneComplete);
			Control::Audio->PlaySample(aVOL5MaryRefS3_1);

			//Journal Entry Add
			if(!ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P3_CrystalVideo])
			{
				//Flag Set
				ProfileSystem::Instance->ActiveProfile.IsJrnNotesFound[eJrn_L5P3_CrystalVideo] = true;

				//Add to Note Array
				ProfileSystem::Instance->ActiveProfile.JrnNotesCollected[ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected] = eJrn_L5P3_CrystalVideo;

				ProfileSystem::Instance->ActiveProfile.JrnPageLastUnlocked = ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected;

				//Increment Notes
				ProfileSystem::Instance->ActiveProfile.NumJrnNotesCollected += 1;
				Hud->ShowJrnEntryFX();
			}
		}
	}

	if(isAnimationPlaying)
	{
		Cursor::SetMode(CA_Normal);
		return;
	}

	CPoint pos = Control::Input->Mpos();
	if(!isMouseOverCloseButton)
		Cursor::SetMode(CA_Normal);

	switch (_state)
	{
	case L5S3_Pop2_Initial:
		{
			if (PopupClickArea.Intersection(&pos))
			{
				if (!ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
				{
					Cursor::SetMode(CA_Normal);

					if (Control::Input->LBclicked())
					{
						Hud->HideOrbGlowing();
						Hud->SendOrbFXToObjectPos(GetObjectByID("cloth")->Pos);
						isAnimationPlaying = true;
					}
				}
				else
				{
					Cursor::SetMode(CA_Gear);
					if (Control::Input->LBclicked())
					{
						Hud->ShowBannerText(Banner_L5_S3_ClickStoneOnMiddle);
					}
				}
			}
		}
		break;

	case L5S3_Pop2_PlayingMemVdo:
		
		break;

	case L5S3_Pop2_EmblemPlaced:
		{
			if(PaperRect.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					Control::Audio->QuickLoadAndPlaySFX("s3p2_scrooltake");
					ProfileSystem::Instance->ActiveProfile.L5_S3_PaperCollectedFromStone = true;
					_janim_PaperOpen->PlayAnim();
					isAnimationPlaying = true;
				}
			}
		}
		break;

	case L5S3_Pop2_PaperTaken:
		{
			if (PopupClickArea.Intersection(&pos))
			{
				Cursor::SetMode(CA_HandTake);
				if (Control::Input->LBclicked())
				{
					_paper->FadeOut(1);
					Hud->TakeInventoryItem(eInv_L5_S3_MoonSolution);
					ProfileSystem::Instance->ActiveProfile.IsTaskLeft[eArea_L5_S5_MountainBelow] = true;
					ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken = true;
					_state = L5S3_Pop2_PaperCollcetd;
				}
			}
		}
		break;

	case L5S3_Pop2_PaperCollcetd:
		{

		}
		break;

	default:
		break;
	}
}

void L5S3_PopStone::HandleInventoryDrop(CPoint dropPos, int itemVal)
{
	if (_janim_StoneBreak && 
		ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken &&
		PopupClickArea.Intersection(&dropPos) && 
		itemVal ==  eInv_L5_S5_StoneEmblem)
	{
		Control::Audio->QuickLoadAndPlaySFX("s3p2_staremblemfix");

		_janim_StoneBreak->PlayAnim();
		isAnimationPlaying = true;

		Hud->ReleaseDrag();
		Hud->CloseInventory(); 

		ProfileSystem::Instance->ActiveProfile.Inventory[eInv_L5_S5_StoneEmblem] = false;
		Hud->InvBoxCtrl->ClearItemFromInvBox(eInv_L5_S5_StoneEmblem);
	}
	else
	{
		Popup::HandleInventoryDrop(dropPos, itemVal);
	}
}

void L5S3_PopStone::ProcessHint()
{
	if(GetPopupState()!= ePopupState_Open )
		return;

	if(!ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
	{
		Hud->InitHintTrailEmit(&PopupItemDropArea);
		return;
	}
	
	if (ProfileSystem::Instance->ActiveProfile.L5_S3_EmblemPlacedOnStone && 
		(!ProfileSystem::Instance->ActiveProfile.L5_S3_PaperCollectedFromStone || !ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken))
	{
		Hud->InitHintTrailEmit(&PaperRect);
		return;
	}
	
	if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken &&
		ProfileSystem::Instance->ActiveProfile.L5_S3_memClothTaken)
	{
		Hud->ShowHintForInvItem(eInv_L5_S5_StoneEmblem, &PopupItemDropArea);
		return;
	}

		//Shows Close Button
		Popup::ProcessHint();
}

bool L5S3_PopStone::isTaskLeft()
{
	bool isAnyTaskLeft = false;

	if (ProfileSystem::Instance->ActiveProfile.L5_S5_InvStoneEmblemTaken &&
		!ProfileSystem::Instance->ActiveProfile.L5_S3_InvMoonSolutionTaken)
	{
		isAnyTaskLeft = true;
	}

	return isAnyTaskLeft;
}

L5S3_PopStone::~L5S3_PopStone()
{
	Control::Audio->UnloadSample(aVOL5MaryRefS3_1);
	SAFE_DELETE(_janim_StoneBreak);
	SAFE_DELETE(_janim_PaperOpen);
	SAFE_DELETE(_clothMemoryVdo);
	SAFE_DELETE(subtitleText);
}

void L5S3_PopStone::OnAnimationComplete(int animId)
{
	switch (animId)
	{
	case 1:
		{
			ProfileSystem::Instance->ActiveProfile.L5_S3_EmblemPlacedOnStone = true;
			_state = L5S3_Pop2_EmblemPlaced;
			_interface->UpdateHOSystem(this);
		}
		break;
		
	case 2:
		{
			ProfileSystem::Instance->ActiveProfile.L5_S3_PaperCollectedFromStone = true;
			_state = L5S3_Pop2_PaperTaken;
		}
		break;

	default:
		break;
	}

	isAnimationPlaying = false;
}

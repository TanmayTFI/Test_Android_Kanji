//====================================
//	DQFYLH
//	HO System
//	ExtrasRoom
//====================================

#include "AchieveRoom.h"
#include "Hud.h"
#include "Fonts.h"
#include "HOScene.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "Rect.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "TextureFactory.h"
#include "AssetsStringTable.h"

const CPoint	CenterPoint(683,334,0);

const CRectangle  MemoryRect(233.0f,431.0f,195.0f,295.0f);
const CRectangle  AwardRect(943.0f,430.0f,202.0f,297.0f);
const CRectangle  MorphRect(480.0f,104.0f,416.0f,518.0f);


ExtrasRoom::ExtrasRoom()
{
	MessageText = NULL;
	MessageElement = NULL;
	LoadScene("EXTRAS\\ACHIEVE\\ACH.SCN");
	

	HOScene->SetZOff(eZOrder_INVBOXITEMS+10);

	BackBtn = HOScene->GetObjectByID("btnback");

	PopCloseBtn = NULL;

	PopMaskBg = NULL;

	PopMaskBg = new CObject();
	PopMaskBg->LoadSprite("UI\\MASK.SPR");
	PopMaskBg->SetXY(683,384);
	PopMaskBg->SetScale(2.0);
	PopMaskBg->SetZ(eZOrder_INVBOXITEMS+30);
	PopMaskBg->Show(false);

	InitMessageBanner();

	Popup_State = POPUP_CLOSED;

	IsPopupActive = false;
	IsPopupTransitionActive = false;
	PopupAlpha = 0;
	PopVal = 0;

	KidMemOverFX = new SuperFX("PRT\\KID\\kidtele.INI", PointSystem::CreateCPoint(320,580,0), eZOrder_INVBOXITEMS+100);
	EagleGlitterFX = new SuperFX("PRT\\KID\\eagleglint.INI", PointSystem::CreateCPoint(666,316,0), eZOrder_INVBOXITEMS+100);
	EagleGlitterFX->Start();
	AwardFX = new SuperFX("PRT\\KID\\awardfx.INI", PointSystem::CreateCPoint(1037,563,0), eZOrder_INVBOXITEMS+100);
	AwardFX->Start();
	CrystalFX = new SuperFX("PRT\\KID\\crystalfx.INI", PointSystem::CreateCPoint(585,637,0), eZOrder_INVBOXITEMS+100);
	CrystalFX->Start();

	HOScene->GetObjectByID("kidOverlay")->SetAlpha(0.75f);

	memoryMesh = new MeshObject("EXTRAS\\ACHIEVE\\LEFTMESH.JPA", 5, 5);
	memoryMesh->SetPosAndParams(231, 451, 1.5f, 1.25f, 6, 4);
	memoryMesh->SetZ(eZOrder_INVBOXITEMS+33);

	awardMesh = new MeshObject("EXTRAS\\ACHIEVE\\RIGHTMESH.JPA", 5, 5);
	awardMesh->SetPosAndParams(960, 450, 1.5f, 1.5f, 4, 6);
	awardMesh->SetZ(eZOrder_INVBOXITEMS+33);

	MemoryOver = HOScene->GetObjectByID("memover");
	MemoryOver->Show(false);

	AwardsOver = HOScene->GetObjectByID("awardover");
	AwardsOver->Show(false);

	MorphOver = HOScene->GetObjectByID("morphover");	
	MorphOver->Show(false);

	MorphText = new CBitmapText();
	MorphText->SetPos(PointSystem::CreateCPoint(725, 590, 0));
	MorphText->LoadFont(eFont_Small);
	MorphText->SetAlignment(Align_Center);
	MorphText->SetText(AssetsStringTable::getString("morphText"));
	MorphText->SetZ(MorphOver->ZOrder);

	MemoryText = new CBitmapText();
	MemoryText->SetPos(PointSystem::CreateCPoint(320, 702, 0));
	MemoryText->LoadFont(eFont_Small);
	MemoryText->SetAlignment(Align_Center);
	MemoryText->SetText(AssetsStringTable::getString("memoryText"));
	MemoryText->SetZ(MemoryOver->ZOrder);

	AwardsText = new CBitmapText();
	AwardsText->SetPos(PointSystem::CreateCPoint(1045, 705, 0));
	AwardsText->LoadFont(eFont_Small);
	AwardsText->SetAlignment(Align_Center);
	AwardsText->SetText(AssetsStringTable::getString("awardsText"));
	AwardsText->SetZ(AwardsOver->ZOrder);

	BackBtnText = new CObject();
	BackBtnText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 710, 700, AwardsOver->ZOrder);
	BackBtnText->SetUID("BackBtnText");
	HOScene->PushObject(BackBtnText);

	HOScene->GetObjectByID("kidOverlay")->Animate(0,5,2,0.05f,6.0f);
	HOScene->GetObjectByID("MedalSymbol")->Animate(0,5,0,0,5.0f);

	HOScene->GetObjectByID("lightray1")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray1")->Animate(0.15f, 0.75f, 5.0f);

	HOScene->GetObjectByID("lightray2")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray2")->Animate(0.15f, 0.75f, 7.0f);

	HOScene->GetObjectByID("lightray3")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray3")->Animate(0.15f, 0.75f, 6.0f);

	HOScene->GetObjectByID("lightray4")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray4")->Animate(0.15f, 0.75f, 5.0f, 1.0f);

	HOScene->GetObjectByID("ligthray5")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ligthray5")->Animate(0.15f, 0.75f, 6.0f);

	HOScene->GetObjectByID("lightray6")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray6")->Animate(0.15f, 0.75f, 5.0f, 1.0f);

	HOScene->GetObjectByID("ligthray7")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ligthray7")->Animate(0.15f, 0.75f, 7.0f);

	HOScene->GetObjectByID("lightray8")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("lightray8")->Animate(0.15f, 0.75f, 4.0f);

	HOScene->GetObjectByID("ligthray9")->SetBlendMode(eBlend_AlphaAdditive);
	HOScene->GetObjectByID("ligthray9")->Animate(0.15f, 0.75f, 6.0f, 1.0f);

	int maxMorphObjects = 28;
	int numMorphCollected = ProfileSystem::Instance->ActiveProfile.NumMorphObjectsCollected;
	for (int i = 1; i <= maxMorphObjects; i++)
	{
		if(i>numMorphCollected)
			HOScene->GetObjectByID(std::string("morphobj")+MKSTR(i))->SetEnable(false);
	}

	if(numMorphCollected>9)
	{
		HOScene->GetObjectByID("MorphNum1")->PlayAnimation(numMorphCollected/10);
		HOScene->GetObjectByID("MorphNum2")->PlayAnimation(numMorphCollected%10);
	}
	else
	{
		HOScene->GetObjectByID("MorphNum1")->SetEnable(false);
		HOScene->GetObjectByID("MorphNum2")->PlayAnimation(numMorphCollected%10);
	}

	Control::Audio->TurnOffLvlMscTracks();//turns off music-snd carryover....

	SetZ(eZOrder_INVBOXITEMS+200);

	
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_ExtrasRoom] = true;

	Control::Audio->LoadSample(aTrackKidRoom, AudioTrack,true);
	Control::Audio->PlaySample(aTrackKidRoom,true);


	ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras = false;
	ProfileSystem::Instance->ActiveProfile.IsAwardsRoomFromExtrasScene = false;

	//Setting the Current Area, to prevent if user Force exit while here
	ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;

	Visible = true;	//To render anything with renderlistentity especially Video
}

void ExtrasRoom::Update()
{
#ifdef K_ANDROID
//handle BACK key gracefully
	if (Control::Input->isKeyBackSensed()) 
	{
		//Back to Scene or to Main Menu
			if(ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu)
				SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			else
			{
				SendMessage(ControlExit, ID, HOSM_NewArea);
				ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
			}
	}
#endif
	Cursor::SetMode(CA_Normal);	
	CPoint pos = Control::Input->Mpos();

	UpdateBanner();

	if(IsPopupTransitionActive)
		UpdatePopupTransition();

	if(MorphRect.Intersection(&pos))
	{
		if(!MorphOver->Visible)
		{
			//MorphOver->FadeIn(4.0f);
		}
	}
	else{
		if(MorphOver->Visible)
		{
			//MorphOver->FadeOut(4.0f);
		}
	}

	if(MemoryRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_ExitUp);
		if(!MemoryOver->Visible)
			{
				//MemoryOver->FadeIn(4.0f);
			}

		if(!KidMemOverFX->Active)
			KidMemOverFX->Start();

		if(Control::Input->LBclicked())
		{
			//Load Kids Room
			ProfileSystem::Instance->ActiveProfile.IsKidsRoomFromExtras = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_KidsRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
		}
	}
	else{
		if(KidMemOverFX->Active)
			KidMemOverFX->StopEmit();

		if(MemoryOver->Visible)
			{
				//MemoryOver->FadeOut(4.0f);
			}
	}

	if(AwardRect.Intersection(&pos))
	{
		Cursor::SetMode(CA_ExitUp);
		if(!AwardsOver->Visible)
		{
			//AwardsOver->FadeIn(4.0f);
		}
		if(Control::Input->LBclicked())
		{
			//Load Awards Room
			ProfileSystem::Instance->ActiveProfile.IsAwardsRoomFromExtrasScene = true;
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_AwardsRoom;
			SendMessage(ControlExit, ID, HOSM_NewArea);
			return;
			
		}
	}
	else{
		if(AwardsOver->Visible)
		{
			//AwardsOver->FadeOut(4.0f);
		}
	}

	if(BackBtn->Collide(&pos))
	{
		Cursor::SetMode(CA_HandPointy);
		BackBtn->PlayAnimation(1);
		if(Control::Input->LBclicked())
		{
			BackBtn->PlayAnimation(2);
			//Back to Scene or to Main Menu
			if(ProfileSystem::Instance->ActiveProfile.IsExtrasRoomFromMainMenu)
				SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			else
			{
				SendMessage(ControlExit, ID, HOSM_NewArea);
				ProfileSystem::Instance->ActiveProfile.IsStartMusic = true;
			}
			return;
		}
	}
	else
	{
		BackBtn->PlayAnimation(0);
	}
}

float ExtrasRoom::easingQuadOut(float t,float b,float c,float d)
{
	return (c * -t*(t-2) / d + b);
}

void ExtrasRoom::UpdatePopupTransition()
{
	if( IsPopupActive )
	{
		switch( Popup_State )
		{
		case POPUP_SCALEUP:
			PopVal += POPUP_SCALE_FACTOR * Control::LogicRate;
			PopupAlpha = easingQuadOut(PopVal,0,1,1.0f);

			if( PopVal >= 1.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 1.0f;
				//MemPopScene->Show(true);
				Popup_State = POPUP_OPEN;
			}
			PopMaskBg->SetAlpha(PopupAlpha);
			break;
		case POPUP_SCALEDOWN:
			PopupAlpha -= POPUP_SCALEDOWN_FACTOR * Control::LogicRate;
			if( PopupAlpha <= 0.0f )
			{
				IsPopupTransitionActive = false;

				PopupAlpha = 0.0f;
				PopMaskBg->Show(false);
				Popup_State = POPUP_CLOSED;
				
				IsPopupActive = false;
			}		
			else
				PopMaskBg->SetAlpha(PopupAlpha);
			break;
		}
	}
}

void  ExtrasRoom::SetupPopupTransition(CHOScene *Popup, int popx, int popy)
{

    
        IsPopupTransitionActive = true;
    if( GFInstance->IsRTCreated )
    {
        GFInstance->RTGraphic->clearRenderTarget();
        GFInstance->RTGraphic->beginRenderToTarget();//RENDER TO TARGET STARTS
        GFInstance->IsRenderingToRT = true;
    }
        Popup->RenderToTarget(-171,0);
    if( GFInstance->IsRTCreated )
    {
        GFInstance->IsRenderingToRT = false;
        GFInstance->RTGraphic->endRenderToTarget();
    }
        GFInstance->SetPopupTween( (popx) ,(popy),171,0);
        
        PopMaskBg->SetAlpha(0);
        PopMaskBg->Show(true);
    
}

void ExtrasRoom::Render(const CPoint* pCam)
{  
	static int FrameCnt = 0;
   FrameCnt = (FrameCnt+1)%2;

   if(IsPopupActive && IsPopupTransitionActive)
	{
		//Init again if render target lost due to fullscreen-window switches or similar
            if( GFInstance->RTGraphic != NULL )
                GFInstance->RTGraphic->blitAlphaRectFx(0,0,1366,768,
                short(GFInstance->PStartX+float(GFInstance->PEndX-GFInstance->PStartX)*PopupAlpha),
                short(GFInstance->PStartY+float(GFInstance->PEndY-GFInstance->PStartY)*PopupAlpha),
                -5*(1.0f-PopupAlpha),
                //0.2f,0.6f); //test gap between frames
            PopupAlpha,PopupAlpha);
    }
}

/////////////////////////////////////////////////////
void ExtrasRoom::InitMessageBanner()
{	
	IsBannerStandStill = false;
	TransOutFactor = 1.0f;
	BannerTimer = 0.0f;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;

	MessageText = new CBitmapText();
	MessageText->SetZ(eZOrder_Banner+1);
	MessageText->SetAlignment(Align_Center);
	MessageText->SetPos(&GFApp->MessageTextPos);
	MessageText->LoadFont(eFont_26);
	MessageText->SetText(StringTable::getStr("ExMessage_Text"));

	const CPoint MessageBannerPos(0.0f, 0.0f, 0.0f);
	MessageElement = new CObject();
	MessageElement->LoadSprite("UI\\BNR.SPR");
	MessageElement->SetZ(eZOrder_Banner);//above back button
	MessageElement->SetPos(&MessageBannerPos);

	HideMessageBanner();
}

bool ExtrasRoom::HideMessageBanner(void)
{
	bool WasActiveMessage = MessageElement->Visible;
	BannerAlpha = 0.0f;
	BannerState = eHMS_Idle;
	MessageElement->Show(false);
	MessageText->Show(false);

	return WasActiveMessage;
}


void ExtrasRoom::TransOutBanner()
{
	if( BannerState == eHMS_TransIn )
	{
		BannerTimer = 0.0f;
		BannerState = eHMS_TransOut;
	}
	else
		TransOutFactor = 2.0f;
}


void ExtrasRoom::ShowMessageBanner(const std::string& message, int SFXId)
{
	IsBannerStandStill = false;
    TransOutFactor = 1.0f;
	Control::Input->ClearMouse();
	
	BannerState = eHMS_TransIn;
	BannerTimer = 0.0f;	
	
	MessageText->Show(true);
	MessageText->SetText(message);
	MessageElement->Show(true);

	MessageElement->SetAlpha(BannerAlpha);
	MessageText->SetAlpha(BannerAlpha);
	switch( SFXId )
	{
		case SFX_SOFT_NOTIFY: Control::Audio->PlaySample(aSFXNotifySoft);break;
		case SFX_WRONG_NOTIFY: Control::Audio->PlaySample(aSFXNotifyHard);break;
		case SFX_EMPTY_NOTIFY: Control::Audio->PlaySample(aSFXGlassBtn);break;
	}
}

void ExtrasRoom::UpdateBanner()
{
	if( BannerState == eHMS_Idle )
		return;

	if( BannerState == eHMS_TransIn )
	{
		BannerAlpha += 2.0f * Control::LogicRate;

		if( BannerAlpha >= MAX_BANNER_ALPHA )
		{
			BannerAlpha = MAX_BANNER_ALPHA;
		}

		BannerTimer += Control::LogicRate;
		if( BannerTimer >= 3.0f  || (BannerAlpha == MAX_BANNER_ALPHA && Control::Input->LBclicked()) )
		{
			BannerTimer = 0.0f;
			BannerState = eHMS_TransOut;
		}
	}
	else if( !IsBannerStandStill )
	{
		BannerAlpha -= TransOutFactor * Control::LogicRate;

		if( BannerAlpha <= 0.0f )
		{
			BannerAlpha = 0.0f;
			BannerState = eHMS_Idle;
			MessageElement->Show(false);
			MessageText->Show(false);
			BannerCloseNotify();
		}
	}

	MessageElement->SetAlpha(BannerAlpha);
	float MTAlpha = ( BannerAlpha > MAX_BANNERTEXT_ALPHA )? MAX_BANNERTEXT_ALPHA: BannerAlpha;
	MessageText->SetAlpha(MTAlpha);
}

ExtrasRoom::~ExtrasRoom()
{
	SAFE_DELETE(PopMaskBg);

	SAFE_DELETE(memoryMesh);
	SAFE_DELETE(awardMesh);

	if( Control::Audio->IsPlaying(aTrackKidRoom))
		Control::Audio->StopSample(aTrackKidRoom);

	Control::Audio->UnloadSample(aTrackKidRoom);

	SAFE_DELETE(KidMemOverFX);
	SAFE_DELETE(EagleGlitterFX);
	SAFE_DELETE(AwardFX);
	SAFE_DELETE(CrystalFX);

	delete MessageElement;
	delete MessageText;

	delete MorphText;
	delete AwardsText;
	delete MemoryText;
}

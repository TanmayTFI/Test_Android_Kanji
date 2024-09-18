//====================================
//	DQFYLH
//	HO System
//	Level 4 - Scene 01 : SWAMP ENTRANCE HO
//====================================

#include "L4_S1_ShoeHO.h"
#include "Hud.h"
#include "HOScene.h"
#include "Banners.h"
#include "ProfileSystem.h"
#include "HiddenObjectEnums.h"
#include "TextureFactory.h"
#include "ParticleEffect.h"
#include "Rect.h"
#include "Polygon.h"
#include "Resources.h"
#include "Application.h"
#include "SDrop.h"
#include "GameFlow.h"
#include "Util.h"
#include "exMath.h"
#include "Circle.h"




const CRectangle  Match3btnRect(543.0f,3.0f,279.0f,51.0f);

#pragma region L4_S1_HOObject

L4_S1_HOObject::L4_S1_HOObject()
{
	_hiddenObject = NULL;
	_mouseOverObject = NULL;
	_id = -1;
	_nameText = "";
	_descText = "";
	//_polygonArea = NULL;
	_isFound = false;
	_hintText = NULL;
	_hintText = new CBitmapText();

	
}

void L4_S1_HOObject::InitObject()
{
	if(_hiddenObject)
	{
	_hiddenObject->SetMouseOverCallback(CC_CALLBACK_1(L4_S1_HOObject::OnMouseOverHOObject, this));
	_hiddenObject->SetMouseOutCallback(CC_CALLBACK_1(L4_S1_HOObject::OnMouseOutHOObject, this));
	}
	if(_mouseOverObject)
	_mouseOverObject->SetEnable(false);
}

void L4_S1_HOObject::ShowHintText()
{
	CRectangle InvAreaRect(368.0f,664.0f,624.0f,96.0f);
	float vertGap = 26, horizontalGap = InvAreaRect.w/4.0f;
	CPoint pos(0,0,0);

	pos.x = InvAreaRect.Left() + horizontalGap*2.1f;
	pos.y = InvAreaRect.Top() + vertGap*2;

	ColorRGB gold = {(233/255.0f),(204/255.0f),(131/255.0f)};
	_hintText->SetTextProps(_descText, pos.x, pos.y, eZOrder_INVBOXITEMS, gold, eFont_25, Align_Center);
	_hintText->SetZ(eZOrder_INVBOXITEMS);
	_hintText->SetScale(1.0f);
}

void L4_S1_HOObject::HideHintText()
{
	_hintText->SetText("");
}

void L4_S1_HOObject::OnMouseOverHOObject(CObject *obj)
{
	//int id = ((L4_S1_HOObject*)obj)->getID();
	if (getIsFound())
		return;
	_mouseOverObject->SetEnable(true);
}

void L4_S1_HOObject::OnMouseOutHOObject(CObject *obj)
{
	if (getIsFound())
		return;
	_mouseOverObject->SetEnable(false);
}

bool L4_S1_HOObject::IsMouseClicked(CPoint pos)
{
	if (getIsFound())
		return false;

	if (_hiddenObject->GetWorldRect().Intersection(&pos))
		return true;
	else
		return false;
}

L4_S1_HOObject::~L4_S1_HOObject()
{
	SAFE_DELETE(_hintText);
	SAFE_DELETE(_hiddenObject);
	SAFE_DELETE(_mouseOverObject);	
}

#pragma endregion

#pragma region L4_S1_ShoeHO

L4_S1_ShoeHO::L4_S1_ShoeHO()
{
	AwardManager::GetInstance()->StartHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
	Visible = false;

	//Main Scene
	LoadHud(INVENTORY_V1);
	LoadScene("LV4\\S1\\HO\\HO.SCN");

	GFInstance->PostUpdate(2);

	Control::Audio->ClearUnusedTracks();

	/*if( !Control::Audio->IsPlaying(aTrackLevel2) )
	Control::Audio->PlaySample(aTrackLevel2,true);*/

	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_L4_S1_ShoeHO] = true;

	Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);//lock in HO anim
	Hud->IsHOScene = true;
	Hud->HOState = HOActive;

	mo_Water = NULL;
	mo_Water = new MeshObject(HOScene->GetObjectByID("water"), 5, 5);
	mo_Water->meshFX->SetSpeedAndDisplacement(5, 5, 5, 5);
	HOScene->PushObject(mo_Water);

	/*HOScene->GetObjectByID("b1")->SetAnimSpeed(6);*/
	HOScene->GetObjectByID("b3")->SetAnimSpeed(7);/*
												  HOScene->GetObjectByID("b5")->SetAnimSpeed(8);
												  HOScene->GetObjectByID("b7")->SetAnimSpeed(8);*/

	Visible = true;	//To render anything with renderlistentity


	//Match 3
	match3Ctrl  = NULL;
	ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
	match3Back = new CObject();
	match3Back->LoadInit("MNU\\EXTRAS\\BACKBUTTON.SPR", 1266, 30, eZOrder_Popup + 300);
	match3Back->SetUID("match3Back");
	match3Back->Show(false);
	HOScene->PushObject(match3Back);

	match3BackText = new CObject();
	match3BackText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BUTTONS\\BACK.SPR", 1250, 27, eZOrder_Popup + 301);
	match3BackText->SetUID("match3BackText");
	match3BackText->Show(false);
	HOScene->PushObject(match3BackText);
#ifndef M3_BUTTON
	match3Back->SetEnable(false);
    match3BackText->SetEnable(false);
#endif
	if (ProfileSystem::Instance->_isExtrasGamePlay)
	{
		match3Back->SetEnable(false);
		match3BackText->SetEnable(false);
	}
	BtnMatch3 = new CObject();
	BtnMatch3->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNMATCH3.SPR", 682, 26, 48);
	BtnMatch3->SetUID("btnMatch3");
	HOScene->PushObject(BtnMatch3);

	BtnSwitchToHO = new CObject();
	BtnSwitchToHO->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\BTNSWITCHTOHO.SPR", 682, 26, 49);
	BtnSwitchToHO->SetUID("btnSwitchToHO");
	HOScene->PushObject(BtnSwitchToHO);

	BtnMatch3->SetZ(100);
	BtnSwitchToHO->SetZ(eZOrder_PopupOverPopup + 300);
	BtnSwitchToHO->Show(false);

	for (int i = 0; i < NUMBER_OF_HO_SHOE; i++)
	{
		HiddenObjects[i] = HOScene->GetObjectByID(std::string("h_o_") + MKSTR(i));
		HO_MouseOver[i] = HOScene->GetObjectByID(std::string("m_o_") + MKSTR(i));
		//HO_MouseOver[i]->SetPos(HiddenObjects[i]->GetPosPointer());

		L4_S1_HOObject *obj = new L4_S1_HOObject();
		obj->setHiddenObject(HiddenObjects[i]);
		obj->setMouseOverObject(HO_MouseOver[i]);
		obj->setID(i);
		//obj->setNameText(shoeHOItemNames[i]);
		////obj->setPolygonArea(HOPolygonArray[i]);
		//obj->setDescriptionText(shoeHOItemDescriptions[i]);
		obj->InitObject();

		if (ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeFound[i])
		{
			obj->getHiddenObject()->SetEnable(false);
			obj->setIsFound(true);
			_foundObjs.push_back(obj);
		}
		_hiddenObjs.push_back(obj);
	}

	isHandleInHand = false;
	//HOScene->GetObjectByID("Chellam_hand_1")->SetEnable(false);
	ObjToFind = (int)_foundObjs.size();  //Sac: converted Implicit to explicit

	//if (ObjToFind == 10)
	//{
	//	if (!ProfileSystem::Instance->ActiveProfile.L4_S1_BetelBoxOPened)
	//	{
	//		isHandleInHand = true;
	//		HOScene->GetObjectByID("Chellam_hand_1")->SetEnable(true);
	//	}
	//}
	//if (ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeFound[6])
	//{
	//	HOScene->GetObjectByID("Chameleon")->SetEnable(false);
	//}

	//HOScene->GetObjectByID("Chameleon")->Animate(0,1,8,3);

	//MUSIC_SFX
	musicFadeOutTimer = 0;
	Control::Audio->LoadSample(aAmbL4s1ShoeHOAmb,AudioAmb);
	if( !Control::Audio->IsPlaying(aAmbL4s1ShoeHOAmb) )		
	{
		Control::Audio->PlaySample(aAmbL4s1ShoeHOAmb,true);
	}
	
	//MUSIC_SFX
}

L4_S1_ShoeHO::~L4_S1_ShoeHO()
{
	//SAFE_DELETE(mo_Water);
	Hud->InvBoxCtrl->ResetToInvBox();
	
	if (_foundObjs.size())
		_foundObjs.clear();
	//MUSIC_SFX
	
	Control::Audio->StopSample(aAmbL4s1ShoeHOAmb);
	Control::Audio->UnloadSample(aAmbL4s1ShoeHOAmb);
	//MUSIC_SFX
}


void L4_S1_ShoeHO::Update()
{
	NavigateToScene();
	if( Hud->PopupState != Popup_Inactive )
		return;

	if( Hud->PrevCursorMode != -1 || Hud->InvBoxCtrl->IsHoverItem  || GFInstance->IsActivePopup || Hud->IsComboInvPopupShowing || Hud->teleportSystem->isActive)
		return;

	const CPoint currentPos = Control::Input->Mpos();

	Cursor::SetMode(CA_Normal);
	if(!BtnMatch3->Visible && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken)
	{
		match3Back->Show(true);
		match3BackText->Show(true);
	}
	else
	{
		match3Back->Show(false);
		match3BackText->Show(false);
	}

	if(ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken)
	{
		match3Back->Show(false);
		match3BackText->Show(false);
		BtnMatch3->Show(false);
		BtnSwitchToHO->Show(false);
	}

	if( !ProfileSystem::Instance->ActiveProfile._IsMatch3Playing )
	{
		#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed())
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
			//_hiddenObjs[ObjToFind]->HideHintText();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);						
		}	
		#endif

		if( IsMouseOverGoBackArea(&currentPos)
			#ifdef M3_BUTTON
			&& BtnMatch3->Visible
            #endif
            )
		{
			Cursor::Show(true);
#ifdef TOUCH_UI
	//	if(!GFHud->InvBoxCtrl->navTapped)
		//	Cursor::SetMode(CA_ExitDown,eArea_L4_S1_SwampEntrance);
#else
		Cursor::SetMode(CA_ExitDown,eArea_L4_S1_SwampEntrance);
#endif

		if( Control::Input->LBclicked() )
		{
#ifdef TOUCH_UI
			if(!GFHud->InvBoxCtrl->navTapped)
			{
				GFHud->InvBoxCtrl->GoToScene(eArea_L4_S1_SwampEntrance, 2);
			}
#else			
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
			//_hiddenObjs[ObjToFind]->HideHintText();
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);		
#endif
								
			}
		}

		/*if (isHandleInHand)
		{
			HOScene->GetObjectByID("Chellam_hand_1")->SetPos(new CPoint(currentPos));
			HOScene->GetObjectByID("Chellam_hand_1")->SetZ(HOScene->GetObjectByID("Chellam_hand_1")->ZOrder + 200);

			if (Control::Input->LBclicked() && HOScene->GetObjectByID("Vettila_chellam_01")->GetWorldRect().Intersection(&currentPos))
			{
				HOScene->GetObjectByID("Chellam_hand_1")->SetEnable(false);
				ProfileSystem::Instance->ActiveProfile.L4_S1_BetelBoxOPened = true;
				HOScene->GetObjectByID("Chellam_hand_1")->SetZ(HOScene->GetObjectByID("Chellam_hand_1")->ZOrder - 200);
			}
		}*/

		if (Control::Input->LBclicked())
		{
			if (ObjToFind < NUMBER_OF_HO_SHOE)
			{
				if (_hiddenObjs[ObjToFind]->IsMouseClicked(currentPos))
				{
					_hiddenObjs[ObjToFind]->getHiddenObject()->SetScale(1.1f);
					_hiddenObjs[ObjToFind]->getHiddenObject()->FadeOut();
					_hiddenObjs[ObjToFind]->getMouseOverObject()->SetScale(1.1f);
					_hiddenObjs[ObjToFind]->getMouseOverObject()->FadeOut();
					_hiddenObjs[ObjToFind]->setIsFound(true);
					ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeFound[ObjToFind] = true;
					_foundObjs.push_back(_hiddenObjs[ObjToFind]);
					//_hiddenObjs[ObjToFind]->HideHintText();
					ObjToFind++;
					Control::Audio->QuickLoadAndPlaySFX("s1ho_objectsClicked");
					if (ObjToFind == NUMBER_OF_HO_SHOE)
					{
						AwardManager::GetInstance()->CompleteHO(ProfileSystem::Instance->ActiveProfile.CurrentArea);
						if(_handleExtrasGameComplete(false))
							return;
						ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken = true;
						Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
						Hud->InvBoxCtrl->ResetToInvBox();
						Hud->IsHOScene = false;
						Hud->TakeInventoryItem(eInv_L4_S1_Shoe1);
					}
					else
					{
						//_hiddenObjs[ObjToFind]->ShowHintText();
						Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
						/*if (ObjToFind == 10)
						{
							isHandleInHand = true;
							HOScene->GetObjectByID("Chellam_hand_1")->SetEnable(true);
						}*/
						/*else if (ObjToFind == 2)
						{
							HOScene->GetObjectByID("pot")->SetEnable(false);
						}*/
						///*else */if (ObjToFind == 6)
						//{
						//	HOScene->GetObjectByID("Chameleon")->SetEnable(false);
						//}
					}
				}
			}

		}
	}

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
	{
		if(match3Back->IsIntersect(currentPos))
		{
			if( Control::Input->LBclicked() )
			{
				Hud->InvBoxCtrl->ResetToInvBox();
				Hud->IsHOScene = false;
				//_hiddenObjs[ObjToFind]->HideHintText();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
		}
		if( match3Ctrl->isPuzzleSolved && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken )
		{
			match3Ctrl->clearBoard();
			BtnMatch3->Show(false); 
			BtnSwitchToHO->Show(false);
			Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
			Hud->IsHOScene = false;
			Hud->HOState = HOInactive;
			Hud->InvBoxCtrl->ResetToInvBox();
			ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken = true;
			Hud->TakeInventoryItem(eInv_L4_S1_Shoe1);
			ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
			return;
		}


		if(Match3btnRect.Intersection(&currentPos) && !match3Ctrl->isGemAnimPlaying())
		{
			Cursor::SetMode(CA_HandPointy);
			BtnSwitchToHO->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				match3Ctrl->clearBoard();
				BtnMatch3->Show(true);
				BtnSwitchToHO->Show(false);
				BtnSwitchToHO->PlayAnimation(0);

				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = false;
				
				//_hiddenObjs[ObjToFind]->ShowHintText();
				Hud->InvBoxCtrl->SetLockBase(HOBOX_FRAME);
				for(int i=0; i < HELP_TEXT_LINES; ++i)
					if( Hud->InvBoxCtrl->HelpTexts[i] != NULL )
						Hud->InvBoxCtrl->HelpTexts[i]->Show(true);//turn on HO text
				
				return;
			}
		}
		else
		{
			BtnSwitchToHO->PlayAnimation(0);
		}
		return;
	}
	else
	{
		if(Match3btnRect.Intersection(&currentPos) )
		{
			Cursor::SetMode(CA_HandPointy);
			BtnMatch3->PlayAnimation(1);
			if( Control::Input->LBclicked() )
			{
				Control::Audio->QuickLoadAndPlaySFX(aSFXClick);

				if(match3Ctrl == NULL)
				{
					match3Ctrl = new Match3(11);
					AddControl(match3Ctrl);
				}
				else
				{
					match3Ctrl->ShowBoard();
				}

				BtnMatch3->Show(false);
				BtnMatch3->PlayAnimation(0);
				BtnSwitchToHO->Show(true);
				
				ProfileSystem::Instance->ActiveProfile._IsMatch3Playing = true;
				
				for (int i = 0; i < NUMBER_OF_HO_SHOE; i++)
				{
					//_hiddenObjs[i]->HideHintText(); 
				}
				Hud->InvBoxCtrl->SetLockBase(MATCH3_FRAME);
				for(int i=0; i < HELP_TEXT_LINES; ++i)
					if( Hud->InvBoxCtrl->HelpTexts[i] != NULL )
						Hud->InvBoxCtrl->HelpTexts[i]->Show(false);//turn off HO text
				return;
			}
		}
		else
		{
			BtnMatch3->PlayAnimation(0);
		}
	}

}


void L4_S1_ShoeHO::Required()
{
	HiddenObjectSystem::Required();	//forced update

	if( Control::Audio->IsPlaying(aTrackL4theme) )
	{
		Control::Audio->Samples[aTrackL4theme]->SetVolume(Control::Audio->GlobalMusicVol - musicFadeOutTimer);
		if((Control::Audio->GlobalMusicVol - musicFadeOutTimer)<=0)
		{
			 Control::Audio->Samples[aTrackL4theme]->SetVolume((float)Control::Audio->GlobalMusicVol);
			 Control::Audio->StopSample(aTrackL4theme);
			 musicFadeOutTimer = 0;
			 if( !Control::Audio->IsPlaying(aTrackL4ho) )
				  Control::Audio->PlaySample(aTrackL4ho, true);
		}
		musicFadeOutTimer += Control::LogicRate * 24;
	}
	else
	{
		if( !Control::Audio->IsPlaying(aTrackL4ho))
		{
				Control::Audio->TurnOffLvlMscTracks();
			Control::Audio->PlaySample(aTrackL4ho, true);
		}
	}
}

float L4_S1_ShoeHO::easing(float t,float b,float c,float d)
{
	t /= d/2;
	if (t < 1) return c/2*t*t + b;
	t--;
	return -c/2 * (t*(t-2) - 1) + b;
}

void L4_S1_ShoeHO::Render(const CPoint* pCam)
{  
	if( !Visible ) 
		return;
}

void L4_S1_ShoeHO::ReceiveMessage(int val)
{	
	if( val == MAP_NEW_AREA ) 
	{
		SendMessage(ControlExit, ID, HOSM_NewArea);	
	}
	else if( val == MAP_HUD_CLICK ) 
	{
		//TaskPendingUpdate();
		Hud->ShowMap();
	}
	else if( val == INVBOX_DROP ) 
	{		
		CPoint dropPos;
		int itemVal;
		if( Hud->GetDropInfo( dropPos, itemVal ) )//No drop items in this scene and hence!!
		{
			GFInstance->WrongInvDropMsg(Hud);
		}
	}
	else if( val == INV_CLOSE )
	{		
		if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvLeftShoeTaken && !ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected)
		{
			ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected = true;
			Hud->TakeInventoryItem(eInv_L4_S1_Knife);
			ProfileSystem::Instance->ActiveProfile.L4_S1_HOShoeCompleted = true;
		}
		else if (ProfileSystem::Instance->ActiveProfile.L4_S1_InvKnifeCollected)
		{
			Hud->InvBoxCtrl->ResetToInvBox();
			Hud->IsHOScene = false;
			
			ProfileSystem::Instance->ActiveProfile.CurrentArea = eArea_L4_S1_SwampEntrance;
			SendMessage(ControlExit, ID, HOSM_NewArea);	
		}
	}	
}

void L4_S1_ShoeHO::ProcessNonHOHint()
{

	if(ProfileSystem::Instance->ActiveProfile._IsMatch3Playing)
		return;

	bool haveHint = false;

	int size = (int)_hiddenObjs.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		if (!_hiddenObjs[i]->getIsFound())
		{

			haveHint = true;
			InitHintTrailEmit(_hiddenObjs[i]->getHiddenObject()->Pos.x, _hiddenObjs[i]->getHiddenObject()->Pos.y);
			AwardManager::GetInstance()->UseHoHint(ProfileSystem::Instance->ActiveProfile.CurrentArea);
			break;
		}
	}

	if (!haveHint)
		Hud->ShowNothingToDoMsgBanner();
}

void L4_S1_ShoeHO::NavigateToScene()
{
	if(GFHud->InvBoxCtrl->navTapped)
	{
		if( Control::Input->LBclicked() )
		{
			if(Control::Input->Mpos().x > GFHud->InvBoxCtrl->navRectX2 && Control::Input->Mpos().x < GFHud->InvBoxCtrl->navRectX1 && Control::Input->Mpos().y > GFHud->InvBoxCtrl->navRectY2 && Control::Input->Mpos().y < GFHud->InvBoxCtrl->navRectY1)
			{	
				//Cursor::SetMode(CA_ExitUp);
				Hud->InvBoxCtrl->ResetToInvBox();
				Hud->IsHOScene = false;
				//_hiddenObjs[ObjToFind]->HideHintText();

				//GFHud->InvBoxCtrl->NavigateText->Show(false);
				Control::Audio->PlayFootStepsFX();
				ProfileSystem::Instance->ActiveProfile.CurrentArea = GFHud->InvBoxCtrl->travelArea;
				SendMessage(ControlExit, ID, HOSM_NewArea);
			}
				
			GFHud->InvBoxCtrl->NavigateText->Text = "";
			GFHud->InvBoxCtrl->travelArea = -1;
			GFHud->InvBoxCtrl->navTapped = false;
			GFHud->InvBoxCtrl->Navigate->Show(false);					
			GFHud->InvBoxCtrl->NavArrow->Show(false);
		}
	}
}
#pragma endregion

//====================================
//DQFULP
// DialogSystem Handling
//====================================

#include "DialogSystem.h"
#include "GameFlow.h"
#include "ProfileSystem.h"
#include "LangDefs.h"
#include "HiddenObjectEnums.h"
#include "AwardManager.h"
#include "GameTextManager.h"


const CRectangle  BtnRectSkip(971.0f,612.0f,183.0f,70.0f);

DialogSystem::DialogSystem()
{
	DialogScene = NULL;

	_isActive = false;	

	_dialogId = -1;
	_numDialogLines = 0;
	_activeDialogIndex = 0;

	IsMoopanTalking = false;
	SophiaPosition = 0;
	IsSophiaTalking = false;

	BtnSkip = NULL;
	btnContText = NULL;
	btnSkipText = NULL;
	S1RajaHead = NULL;	
	CharScene = NULL;
	DialogText = NULL;
	S6TreeMouth = NULL;

	isMaskEnabled = true;


	for (int i = 0; i < 10; i++)
	{
		_maryTalksArray[i] = false;
	}

	isCreekPortalShowFXPlaying = false;
	cv_CreekPortalShowFX = NULL;
	cv_CreekPortalLoopFX = NULL;
	
	clickWaitTimer = 0.5f;

	_isFadingIn = _isFadingOut = false;
	_fadeAlpha = 0;

	_isTextFadingOut = _isTextFadingIn = false;
	_nextDialogId=0;

	startRockTimer = false;
	rockTimer = 0;
}

DialogSystem::~DialogSystem()
{
	SAFE_DELETE(DialogScene);
	SAFE_DELETE(CharScene);
	SAFE_DELETE(DialogText);

	SAFE_DELETE(cv_CreekPortalShowFX);
	SAFE_DELETE(cv_CreekPortalLoopFX);	 
}

void DialogSystem::Required()
{

}

void DialogSystem::Update()
{
	if(!_isActive)
		return;

	//Special case hack for rockman not speaking
	if(startRockTimer)
	{
		if(rockTimer < 8)
			rockTimer += Control::LogicRate;
		else
		{
			startRockTimer = false;
			rockTimer = 0;
			if(L6S4Mouth)
			{
				L6S4Mouth->PlayAnimation(0);
			}
		}
	}

	if(!_isFadingOut && _activeDialogIndex<_numDialogLines)
	{
		if( _dialogId == eDialog_S4Creek)
		{
			if(!_isFadingIn && S4CreekHead->GetAnimation()==1) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					S4CreekHead->PlayAnimation(0);
				}
			}

			if(isCreekPortalShowFXPlaying)
			{
				if(cv_CreekPortalShowFX!=NULL && cv_CreekPortalShowFX->isEndReached())
				{
					isCreekPortalShowFXPlaying = false;
					cv_CreekPortalShowFX->StopVideo();

					cv_CreekPortalLoopFX->PlayVideo();
				}
				return;
			}
		}
		else if(_dialogId == eDialog_L7S4Ameha || _dialogId == eDialog_L3S5_AnnMary || _dialogId == eDialog_L5S4AnasooyaFree || _dialogId == eDialog_L5S4Anasooya || _dialogId == eDialog_S1RajaBeach || _dialogId == eDialog_S3RajaFoyerDark || _dialogId == eDialog_S3RajaFoyerAskForLetter || _dialogId == eDialog_S3RajaFoyer || _dialogId == eDialog_S3RajaFoyerShowLetter)
		{
			if(!_isFadingIn && S1RajaHead->GetAnimation()==1) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					S1RajaHead->PlayAnimation(0);
				}
			}
		}
		else if(_dialogId == eDialog_L5S6TreeGuardian || _dialogId == eDialog_L5S6TreeGuardianAfterBadgePlaced)
		{
			if(!_isFadingIn && S6TreeMouth->GetAnimation()==1) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					S6TreeMouth->PlayAnimation(0);
				}
			}
		}
		else if( _dialogId == eDialog_L2S5SMoopanTired )
		{
			if(!_isFadingIn && IsMoopanTalking ) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					IsMoopanTalking = false;				
				}
			}
		}
		else if( _dialogId == eDialog_L2S5SMoopanDrinking )
		{
			if(!_isFadingIn && IsMoopanTalking ) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					IsMoopanTalking = false;				
				}
			}
		}
		else if( _dialogId == eDialog_L6S3FakeJames )
		{
			if(!_isFadingIn && L6S3JamesFace->GetAnimation() == 1 ) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					L6S3JamesFace->PlayAnimation(0);		
				}
			}
		}
		else if( _dialogId == eDialog_L6S4RockMan)
		{
			if(!_isFadingIn && L6S4Mouth->GetAnimation() == 1 ) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					if(_activeDialogIndex != 0)
						L6S4Mouth->PlayAnimation(0);
				}
			}
		}
		else if( _dialogId == eDialog_L6S5Sophia)
		{
			if(!_isFadingIn && IsSophiaTalking ) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					//SophiaPosition++;
					IsSophiaTalking = false;
					//L6S5Mouth->PlayAnimation(0);		
				}
			}
		}
		else if( _dialogId == eDialog_L6S5SophiaPlayingWithTablet)
		{
			if(!_isFadingIn && IsSophiaTalking) //is talking anim
			{
				if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
				{
					IsSophiaTalking = false;
					//L6S5Mouth->PlayAnimation(0);		
				}
			}
		}
		else if( _dialogId == eDialog_L7S3Anantha)
		{
			if(!_isFadingIn)
			{
				if (L7AnanthaHead->GetAnimation() == 1 ) //is talking anim
				{
					if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
					{
						L7AnanthaHead->PlayAnimation(0);
						L7BlinkTimer = 5.0f;
					}
				}
				else
				{
					L7BlinkTimer -= Control::LogicRate;
					if(L7BlinkTimer<0)
					{
						L7BlinkTimer = 5.0f;
						L7AnanthaEyeBlink->ForcePlayAnimation(1);
					}
				}
			}
		}
	}

	if(_isFadingIn)
	{
		_fadeAlpha += 1.5f*Control::LogicRate;
		if(_fadeAlpha>1.0f)
		{
			_fadeAlpha = 1.0f;
			_isFadingIn = false;
			Control::Audio->PlaySample((eAudio)_dialogVOArray.at(_activeDialogIndex));
		}
		DialogText->setAlpha(_fadeAlpha);
		CharScene->SetSceneAlpha(_fadeAlpha);
		DialogScene->SetSceneAlpha(_fadeAlpha);
		if(isMaskEnabled)
			GFInstance->UpdateBlurScene(_fadeAlpha);
		return;
	}
	else if(_isFadingOut)
	{
		_fadeAlpha -= 1.5f*Control::LogicRate;
		if(_fadeAlpha<0)
		{
			_fadeAlpha = 0;
			_isFadingOut = false;

			if(isMaskEnabled)
				GFInstance->TurnOffBlurScene();

			CloseDialogSystem();
		}
		else
		{
			DialogText->setAlpha(_fadeAlpha);
			CharScene->SetSceneAlpha(_fadeAlpha);
			DialogScene->SetSceneAlpha(_fadeAlpha);
			if(isMaskEnabled)
				GFInstance->UpdateBlurScene(_fadeAlpha);
		}
		return;
	}

	if(_isTextFadingOut)
	{
		_fadeAlpha -= 1.5f*Control::LogicRate;
		if(_fadeAlpha<0)
		{
			_fadeAlpha = 0;
			_isTextFadingOut = false;

			//Color it
			ColorRGB defColor = {1.0f, 0.90f, 0.80f};
			ColorRGB maryColor = {0.18f, 0.96f, 0.32f};

			float _lineWidth = 750;
			int _nLines = 4;
			EFont _font = eFont_Italic24;
			eTextAlignment _align = Align_Center;
			std::string temp = _dialogArray.at(_nextDialogId);
			if(StringTable::IsKeyFound(temp))
			{
				_lineWidth = StringTable::GetLineWidth(temp);
				_nLines = StringTable::GetNoOfLines(temp);
				_font = (EFont)StringTable::GetFont(temp);
			}
			SAFE_DELETE(DialogText);
			
			if(_maryTalksArray[_nextDialogId])
				DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, maryColor, 30, _lineWidth, _align, _font);
			else
				DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, defColor, 30, _lineWidth, _align, _font);
			DialogText->SetTextMultiline(StringTable::getStr(temp));

			_isTextFadingIn = true;
		}
		DialogText->setAlpha(_fadeAlpha);
		return;
	}
	else if(_isTextFadingIn)
	{
		_fadeAlpha += 1.5f*Control::LogicRate;
		if(_fadeAlpha>1.0f)
		{
			_fadeAlpha = 1.0f;
			_isTextFadingIn = false;
			if(_dialogId == eDialog_L7S4Ameha || _dialogId == eDialog_L3S5_AnnMary|| _dialogId == eDialog_L5S4AnasooyaFree ||_dialogId == eDialog_L5S4Anasooya || _dialogId == eDialog_S1RajaBeach || _dialogId == eDialog_S3RajaFoyer || _dialogId == eDialog_S3RajaFoyerDark || _dialogId == eDialog_S3RajaFoyerAskForLetter || _dialogId == eDialog_S3RajaFoyerShowLetter)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					S1RajaHead->PlayAnimation(1);	//TALK ANIM
				}
				else
				{
					S1RajaHead->PlayAnimation(0);	//BLINK ANIM
				}
			}
			else if(_dialogId == eDialog_L5S6TreeGuardian && _dialogId == eDialog_L5S6TreeGuardianAfterBadgePlaced)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					S6TreeMouth->PlayAnimation(1);	//TALK ANIM
				}
				else
				{
					S6TreeMouth->PlayAnimation(0);	//BLINK ANIM
				}
			}
			else if( _dialogId == eDialog_S4Creek)
			{
				S4CreekHead->PlayAnimation(1); //Talk Anim
				if(_activeDialogIndex == 3)
				{
					cv_CreekPortalShowFX->PlayVideo();
					isCreekPortalShowFXPlaying = true;
				}
			}
			else if( _dialogId == eDialog_L2S5SMoopanTired)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					IsMoopanTalking = true;	//TALK ANIM
				}
			}
			else if( _dialogId == eDialog_L2S5SMoopanDrinking)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					IsMoopanTalking = true;	//TALK ANIM
				}
			}
			else if(_dialogId == eDialog_L6S3FakeJames)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					L6S3JamesFace->PlayAnimation(1);	//TALK ANIM
				}
				else
				{
					L6S3JamesFace->PlayAnimation(0);	//BLINK ANIM
				}
			}
			else if(_dialogId == eDialog_L6S4RockMan)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					L6S4Mouth->PlayAnimation(1);	//TALK ANIM
				}
				else
				{
					L6S4Mouth->PlayAnimation(0);	//BLINK ANIM
				}
			}
			else if(_dialogId == eDialog_L6S5Sophia)
			{
				IsSophiaTalking = true;
				SophiaPosition++;
			}
			else if(_dialogId == eDialog_L6S5SophiaPlayingWithTablet)
			{
				IsSophiaTalking = true;
				//if(!_maryTalksArray[_nextDialogId])
				//{
				//	L6S5Mouth->PlayAnimation(1);	//TALK ANIM
				//}
				//else
				//{
				//	L6S5Mouth->PlayAnimation(0);	//BLINK ANIM
				//}
			}
			else if(_dialogId == eDialog_L7S3Anantha)
			{
				if(!_maryTalksArray[_nextDialogId])
				{
					L7AnanthaHead->PlayAnimation(1);	//TALK ANIM
				}
				else
				{
					L7AnanthaHead->PlayAnimation(0);	//BLINK ANIM
				}
			}
			Control::Audio->PlaySample((eAudio)_dialogVOArray.at(_activeDialogIndex));
		}
		DialogText->setAlpha(_fadeAlpha);
		return;
	}


	if(clickWaitTimer>-1.0f)
		clickWaitTimer -= Control::LogicRate;

	Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();
	bool clicked = Control::Input->LBclicked();

	if(!Control::Audio->IsPlaying((eAudio)_dialogVOArray.at(_activeDialogIndex)))
	{
		if( _dialogId == eDialog_S1RajaBeach || _dialogId == eDialog_L5S4AnasooyaFree || _dialogId == eDialog_L5S4Anasooya || _dialogId == eDialog_L7S4Ameha || _dialogId == eDialog_L3S5_AnnMary)
		{
			if(S1RajaHead->GetAnimation()==2)
				S1RajaHead->PlayAnimation(1); //idle animation
		}
	}

	BtnSkip->PlayAnimation(0);
	btnSkipText->PlayAnimation(0);
	if(BtnRectSkip.Intersection(&pos))
	{
		BtnSkip->PlayAnimation(1);
		btnSkipText->PlayAnimation(1);
		Cursor::SetMode(CA_HandPointy);
		if(clicked)
		{
			AwardManager::GetInstance()->SkipDialogueOrCutScene();
			if( _dialogId == eDialog_S4Creek)
				CloseDialogSystem();
			else
				HideDialog();
			return;
		}
	}
	else
	{
		if(clicked && clickWaitTimer<0)
		{
			Control::Audio->StopSample((eAudio)_dialogVOArray.at(_activeDialogIndex));
			if(_dialogId == eDialog_L6S5Sophia)
				clickWaitTimer = 1.5f;			
			else
				clickWaitTimer = 0.5f;			
			_activeDialogIndex += 1;
			if(_activeDialogIndex<_numDialogLines)
			{
				_nextDialogId = _activeDialogIndex;
				_fadeAlpha = 1.0f;
				_isTextFadingOut = true;
				//DialogText->SetTextMultiline(_dialogArray.at(_activeDialogIndex));
			}
			else
			{
				if( _dialogId == eDialog_S4Creek)
					CloseDialogSystem();
				else
					HideDialog();
				return;
			}
		}
	}
}

void DialogSystem::HideDialog()
{
	_isFadingOut = true;
	if(_dialogId == eDialog_L7S4Ameha)
	{
		hairObj->SetEnable(false);
	}
		
	if(_activeDialogIndex<_numDialogLines) //if user skips when dialog VO audio is playing
		Control::Audio->StopSample((eAudio)_dialogVOArray.at(_activeDialogIndex));

}

void DialogSystem::ShowDialog(int dialogId, bool quickShow)
{	
	_isActive = true;

	_dialogId = dialogId;

	DialogScene = new CHOScene();
	DialogScene->Load("DIALOG\\DIALOG.SCN");
	DialogScene->SetZOff(eZOrder_CutsceneFull+10);

	BtnSkip = DialogScene->GetObjectByID("diaskip");
	TxtContinue = DialogScene->GetObjectByID("diatxtcontinue");

	btnContText = new CObject();
	btnContText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\DIALOG\\CONTINUE.SPR", TxtContinue->Pos.x, TxtContinue->Pos.y, TxtContinue->ZOrder + 1);
	btnContText->SetUID("btnContText");
	DialogScene->PushObject(btnContText);

	btnSkipText = new CObject();
	btnSkipText->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\DIALOG\\SKIP.SPR", BtnSkip->Pos.x, BtnSkip->Pos.y, BtnSkip->ZOrder + 1);
	btnSkipText->SetUID("btnSkipText");
	DialogScene->PushObject(btnSkipText);

	ColorRGB red = {1.0f, 0.90f, 0.80f};
	float __lineWidth = 750;
	if(StringTable::IsLanguageMultiByte())
		__lineWidth = 1300;
	if( dialogId == eDialog_S3RajaFoyer)
		DialogText = new MultilineText(4, PointSystem::CreateCPoint(683,675,0), eZOrder_CutsceneFull+12, red, 30, __lineWidth, Align_Center, eFont_Italic24);  //Sac: Initially fontId was default to eFont_26, changing it to eFont_Italic24 as the new font is big
	else if( dialogId == eDialog_L2S5SMoopanDrinking)
		DialogText = new MultilineText(4, PointSystem::CreateCPoint(683,668,0), eZOrder_CutsceneFull+12, red, 30, __lineWidth, Align_Center,eFont_Italic24);
	else
	{
		//DialogText = new MultilineText(4, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, Align_Center,eFont_Italic24);
	}

	for (int i = 0; i < 10; i++)
	{
		_maryTalksArray[i] = false;
	}

	_activeDialogIndex = 0;

	if( dialogId == eDialog_S1RajaBeach)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S1RAJA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("RajaTalkBeachNew");
		S1RajaHead->PlayAnimation(1);//talk loop

		CharScene->GetObjectByID("s1rajashawl")->SetPivot(0,-200);
		CharScene->GetObjectByID("s1rajashawl")->Animate(0,2,0,0.02f,8.0f);

		/*CharScene->GetObjectByID("s1rajahead")->SetPivot(10,116);
		CharScene->GetObjectByID("s1rajahead")->Animate(0,3,2.0f,0,6.0f);*/

		CharScene->GetObjectByID("s1rajahandL")->SetPivot(-60,-171);
		CharScene->GetObjectByID("s1rajahandL")->Animate(0,0,2.0f,0,5.0f);

		CharScene->GetObjectByID("s1rajahandR")->SetPivot(90,-35);
		CharScene->GetObjectByID("s1rajahandR")->Animate(0,0,4.0f,0,5.6f,1.25f);
		
		
		std::string dia_1 = "L1witness";
		std::string dia_2 = "L1buffalo";
		std::string dia_3 = "L1mam";
		std::string dia_4 = "L1me";

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);

		Control::Audio->LoadSample(aVOL1S1_1Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S1_2Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S1_3Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S1_4Raja,	AudioVO);

		_dialogVOArray.push_back(aVOL1S1_1Raja);
		_dialogVOArray.push_back(aVOL1S1_2Raja);
		_dialogVOArray.push_back(aVOL1S1_3Raja);
		_dialogVOArray.push_back(aVOL1S1_4Raja);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		//S1RajaHead->PlayAnimation(2);	//TALK ANIM

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));
	}
	else if( dialogId == eDialog_S3RajaFoyerAskForLetter)
	{
		
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S3RAJA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("RajaFoyerLitTalknew");
		S1RajaHead->PlayAnimation(1);

		CharScene->GetObjectByID("s3rajashawl")->SetPivot(0,-200);
		CharScene->GetObjectByID("s3rajashawl")->Animate(0,1,0,0.02f,8.0f);

		/*CharScene->GetObjectByID("s3Rajahead")->SetPivot(10,116);
		CharScene->GetObjectByID("s3Rajahead")->Animate(0,1,2.0f,0,6.0f);*/

		CharScene->GetObjectByID("s3rajahandL")->PlayAnimation(1);
		CharScene->GetObjectByID("s3rajahandL")->SetPivot(-60,-171);
		CharScene->GetObjectByID("s3rajahandL")->Animate(0,0,1.0f,0,5.0f);

		CharScene->GetObjectByID("s3rajahandR")->SetPivot(90,-35);
		CharScene->GetObjectByID("s3rajahandR")->Animate(0,0,1.0f,0,5.6f,1.25f);

		std::string dia_1 = "L1isle";
		std::string dia_2 = "L1faint";
		std::string dia_3 = "L1us";
		std::string dia_4 = "L1week";
		std::string dia_5 = "L1received";
		std::string dia_6 = "L1dead";

		_maryTalksArray[1] = true;
		_maryTalksArray[2] = true;
		_maryTalksArray[3] = true;
		_maryTalksArray[5] = true;

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);
		_dialogArray.push_back(dia_6);

		Control::Audio->LoadSample(aVOL1S3Talk1_1Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk1_2Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk1_3Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk1_4Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk1_5Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk1_6Mary,	AudioVO);


		_dialogVOArray.push_back(aVOL1S3Talk1_1Raja);
		_dialogVOArray.push_back(aVOL1S3Talk1_2Mary);
		_dialogVOArray.push_back(aVOL1S3Talk1_3Mary);
		_dialogVOArray.push_back(aVOL1S3Talk1_4Mary);
		_dialogVOArray.push_back(aVOL1S3Talk1_5Raja);
		_dialogVOArray.push_back(aVOL1S3Talk1_6Mary);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));
	}
	else if( dialogId == eDialog_S3RajaFoyer)
	{
		
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S3RAJA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("RajaFoyerLitTalknew");
		S1RajaHead->PlayAnimation(1);

		CharScene->GetObjectByID("s3rajashawl")->SetPivot(0,-200);
		CharScene->GetObjectByID("s3rajashawl")->Animate(0,1,0,0.02f,8.0f);

		/*CharScene->GetObjectByID("s3Rajahead")->SetPivot(10,116);
		CharScene->GetObjectByID("s3Rajahead")->Animate(0,1,2.0f,0,6.0f);*/

		CharScene->GetObjectByID("s3rajahandL")->SetPivot(-60,-171);
		CharScene->GetObjectByID("s3rajahandL")->Animate(0,0,1.0f,0,5.0f);

		CharScene->GetObjectByID("s3rajahandR")->SetPivot(90,-35);
		CharScene->GetObjectByID("s3rajahandR")->Animate(0,0,1.0f,0,5.6f,1.25f);

		std::string dia_1 = "L1recognize";
		std::string dia_2 = "L1indeed";
		std::string dia_3 = "L1elixir";
		std::string dia_4 = "L1scripture";
		std::string dia_5 = "L1great";
		std::string dia_6 = "L1diesel";
		std::string dia_7 = "L1husband";

		_maryTalksArray[2] = true;
		_maryTalksArray[4] = true;

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);
		_dialogArray.push_back(dia_6);
		_dialogArray.push_back(dia_7);

		Control::Audio->LoadSample(aVOL1S3Talk2_1Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_2Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_3Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_4Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_5Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_6Raja,	AudioVO);
		Control::Audio->LoadSample(aVOL1S3Talk2_7Raja,	AudioVO);

		
		_dialogVOArray.push_back(aVOL1S3Talk2_1Raja);
		_dialogVOArray.push_back(aVOL1S3Talk2_2Raja);
		_dialogVOArray.push_back(aVOL1S3Talk2_3Mary);
		_dialogVOArray.push_back(aVOL1S3Talk2_4Raja);
		_dialogVOArray.push_back(aVOL1S3Talk2_5Mary);
		_dialogVOArray.push_back(aVOL1S3Talk2_6Raja);
		_dialogVOArray.push_back(aVOL1S3Talk2_7Raja);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_S3RajaFoyerShowLetter)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S3RAJA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("RajaFoyerLitTalknew");
		S1RajaHead->PlayAnimation(1);

		CharScene->GetObjectByID("s3rajashawl")->SetPivot(0,-200);
		CharScene->GetObjectByID("s3rajashawl")->Animate(0,1,0,0.02f,8.0f);

		/*CharScene->GetObjectByID("s3Rajahead")->SetPivot(10,116);
		CharScene->GetObjectByID("s3Rajahead")->Animate(0,1,2.0f,0,6.0f);*/

		CharScene->GetObjectByID("s3rajahandL")->PlayAnimation(1);
		CharScene->GetObjectByID("s3rajahandL")->SetPivot(-60,-171);
		CharScene->GetObjectByID("s3rajahandL")->Animate(0,0,1.0f,0,5.0f);

		CharScene->GetObjectByID("s3rajahandR")->SetPivot(90,-35);
		CharScene->GetObjectByID("s3rajahandR")->Animate(0,0,1.0f,0,5.6f,1.25f);

		std::string dia_1 = "L1lett";

		Control::Audio->LoadSample(aVOL1S3Talk3_Raja,	AudioVO);

		_dialogArray.push_back(dia_1);

		_dialogVOArray.push_back(aVOL1S3Talk3_Raja);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_S3RajaFoyerDark)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S1RAJA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("RajaTalkBeachNew");
		S1RajaHead->PlayAnimation(1);

		CharScene->GetObjectByID("s1rajashawl")->SetPivot(0,-200);
		CharScene->GetObjectByID("s1rajashawl")->Animate(0,2,0,0.02f,8.0f);

		/*CharScene->GetObjectByID("s1rajahead")->SetPivot(10,116);
		CharScene->GetObjectByID("s1rajahead")->Animate(0,3,2.0f,0,6.0f);*/

		CharScene->GetObjectByID("s1rajahandL")->SetPivot(-60,-171);
		CharScene->GetObjectByID("s1rajahandL")->Animate(0,0,2.0f,0,5.0f);

		CharScene->GetObjectByID("s1rajahandR")->SetPivot(90,-35);
		CharScene->GetObjectByID("s1rajahandR")->Animate(0,0,4.0f,0,5.6f,1.25f);

		std::string dia_1 = "L1restore";

		Control::Audio->LoadSample(aVOL1S3Talk0_Raja,	AudioVO);

		_dialogArray.push_back(dia_1);

		_dialogVOArray.push_back(aVOL1S3Talk0_Raja);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_S4Creek)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S4CREEK.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S4CreekHead = CharScene->GetObjectByID("CreekTalkHead");
		S4CreekHead->PlayAnimation(1);//talk loop

		S4CreekHead->Animate(0,3,0,0,6.0f);

		CRectangle RectVid(300, 368, 462, 400);
		cv_CreekPortalShowFX = new CVideo("OGV\\L1S4\\CREEKPORTALSHOW.OGV", RectVid, false, eZOrder_Popup+1);
		cv_CreekPortalShowFX->SetBlendMode(eBlend_AlphaAdditive);
		//cv_CreekPortalShowFX->PlayFadeInVideo(2.0f);
		// Changed the Z order for this video to be of that scene's popup zorder, so that it doesnt draw on top of Creek's Body.
		cv_CreekPortalLoopFX = new CVideo("OGV\\L1S4\\CREEKPORTALLOOP.OGV", RectVid, true, eZOrder_Popup+1);
		cv_CreekPortalLoopFX->SetBlendMode(eBlend_AlphaAdditive);		

		//isCreekPortalShowFXPlaying = true;
		
		CharScene->GetObjectByID("creektalk")->Animate(0,3,0,0,6.0f);

		std::string dia_1 = "L1mary";
		std::string dia_2 = "L1danger";
		std::string dia_3 = "L1letter";
		std::string dia_4 = "L1well";

		Control::Audio->LoadSample(aVOL1S4Talk1_1Creek,	AudioVO);
		Control::Audio->LoadSample(aVOL1S4Talk1_2Creek,	AudioVO);
		Control::Audio->LoadSample(aVOL1S4Talk1_3Creek,	AudioVO);
		Control::Audio->LoadSample(aVOL1S4Talk1_4Creek,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);

		_dialogVOArray.push_back(aVOL1S4Talk1_1Creek);
		_dialogVOArray.push_back(aVOL1S4Talk1_2Creek);
		_dialogVOArray.push_back(aVOL1S4Talk1_3Creek);
		_dialogVOArray.push_back(aVOL1S4Talk1_4Creek);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L2S5SMoopanTired )
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L2\\MOOPANTIREDNEW.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);
		CharScene->SetPosOff(683.0f, 334.0f);

		IsMoopanTalking = true;

		//MoopanHead = CharScene->GetObjectByID("MooppanTalk1");
		//MoopanHead->PlayAnimation(1);//talk loop
		

		//S4CreekHead->Animate(0,3,0,0,6.0f);

		/*CRectangle RectVid(300, 368, 462, 400);
		cv_CreekPortalShowFX = new CVideo("OGV\\L1S4\\CREEKPORTALSHOW.OGV", RectVid, false, eZOrder_CutsceneFull);
		cv_CreekPortalShowFX->SetBlendMode(eBlend_AlphaAdditive);*/
		//cv_CreekPortalShowFX->PlayFadeInVideo(2.0f);

		/*cv_CreekPortalLoopFX = new CVideo("OGV\\L1S4\\CREEKPORTALLOOP.OGV", RectVid, true, eZOrder_CutsceneFull);
		cv_CreekPortalLoopFX->SetBlendMode(eBlend_AlphaAdditive);	*/	

		//isCreekPortalShowFXPlaying = true;
		
		//CharScene->GetObjectByID("creektalk")->Animate(0,3,0,0,6.0f);

		std::string dia_1 = "L2weak";
		std::string dia_2 = "L2who";
		std::string dia_3 = "L2youre";
		std::string dia_4 = "L2guide";
		_maryTalksArray[1] = true;
		_maryTalksArray[3] = true;

		Control::Audio->LoadSample(aVOL2S5Talk1_1Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk1_2Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk1_3Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk1_4Mary,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);

		_dialogVOArray.push_back(aVOL2S5Talk1_1Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk1_2Mary);
		_dialogVOArray.push_back(aVOL2S5Talk1_3Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk1_4Mary);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L2S5SMoopanDrinking )
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L2\\MOOPANTIREDNEW.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);
		CharScene->SetPosOff(683.0f, 334.0f);

		IsMoopanTalking = true;

		std::string dia_1 = "L2info";
		std::string dia_2 = "L2sacred";
		std::string dia_3 = "L2lord";
		std::string dia_4 = "L2spell";
		std::string dia_5 = "L2elixir";
		std::string dia_6 = "L2let";

		_maryTalksArray[5] = true;

		Control::Audio->LoadSample(aVOL2S5Talk2_1Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk2_2Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk2_3Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk2_4Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk2_5Moopan,	AudioVO);
		Control::Audio->LoadSample(aVOL2S5Talk2_6Mary,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);
		_dialogArray.push_back(dia_6);


		_dialogVOArray.push_back(aVOL2S5Talk2_1Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk2_2Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk2_3Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk2_4Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk2_5Moopan);
		_dialogVOArray.push_back(aVOL2S5Talk2_6Mary);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L5S4Anasooya)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S4ANASUYA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("AnasuyaTalk");
		S1RajaHead->PlayAnimation(1);//talk loop

		
		std::string dia_1 = "L5faithful";
		std::string dia_2 = "L5escape";
		std::string dia_3 = "L5century";
		std::string dia_4 = "L5help";
		std::string dia_5 = "L5next";

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);

		Control::Audio->LoadSample(aVOL5S5Talk1_1Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk1_2Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk1_3Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk1_4Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk1_5Latika,	AudioVO);

		_dialogVOArray.push_back(aVOL5S5Talk1_1Latika);
		_dialogVOArray.push_back(aVOL5S5Talk1_2Latika);
		_dialogVOArray.push_back(aVOL5S5Talk1_3Latika);
		_dialogVOArray.push_back(aVOL5S5Talk1_4Latika);
		_dialogVOArray.push_back(aVOL5S5Talk1_5Latika);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		//S1RajaHead->PlayAnimation(2);	//TALK ANIM

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L5S4AnasooyaFree)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\S4ANASUYAFREE.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("AnasuyaTalk");
		S1RajaHead->PlayAnimation(1);//talk loop

		
		std::string dia_1 = "L5thank";
		std::string dia_2 = "L5promise";
		std::string dia_3 = "L5summon";
		

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		

		Control::Audio->LoadSample(aVOL5S5Talk2_1Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk2_2Latika,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk2_3Latika,	AudioVO);
		

		_dialogVOArray.push_back(aVOL5S5Talk2_1Latika);
		_dialogVOArray.push_back(aVOL5S5Talk2_2Latika);
		_dialogVOArray.push_back(aVOL5S5Talk2_3Latika);
		

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		//S1RajaHead->PlayAnimation(2);	//TALK ANIM

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L3S5_AnnMary)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L3\\ANNMARYTALK.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("AnnMaryHead");
		

		
		std::string dia_1 = "L3rules";
		std::string dia_2 = "L3suferring";
		std::string dia_3 = "L3sin";
		std::string dia_4 = "L3time";
		

		_maryTalksArray[2] = true;
		

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		

		Control::Audio->LoadSample(aVOL3S5Talk1_1Ann,	AudioVO);
		Control::Audio->LoadSample(aVOL3S5Talk1_2Ann,	AudioVO);
		Control::Audio->LoadSample(aVOL3S5Talk1_3Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL3S5Talk1_4Ann,	AudioVO);
		

		_dialogVOArray.push_back(aVOL3S5Talk1_1Ann);
		_dialogVOArray.push_back(aVOL3S5Talk1_2Ann);
		_dialogVOArray.push_back(aVOL3S5Talk1_3Mary);
		_dialogVOArray.push_back(aVOL3S5Talk1_4Ann);
	

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		S1RajaHead->PlayAnimation(1);	//TALK ANIM

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L7S4Ameha)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L7\\AMEHATALK.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S1RajaHead = CharScene->GetObjectByID("AmeehaTalk");
		
		hairObj = new MeshObject(CharScene->GetObjectByID("hair"), 5, 5);
		hairObj->meshFX->SetSpeedAndDisplacement(3, 3, 5, 5);
		CharScene->InsertObject(hairObj, 1);

		
		std::string dia_1 = "L7ameeha";
		std::string dia_2 = "L7mary";
		std::string dia_3 = "L7breath";
		std::string dia_4 = "L7fate";
		std::string dia_5 = "L7curse";
		std::string dia_6 = "L7save";

		_maryTalksArray[0] = true;
		_maryTalksArray[5] = true;

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);
		_dialogArray.push_back(dia_6);

		Control::Audio->LoadSample(aVOL7S4Talk1_1Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL7S4Talk1_2Ameha,	AudioVO);
		Control::Audio->LoadSample(aVOL7S4Talk1_3Ameha,	AudioVO);
		Control::Audio->LoadSample(aVOL7S4Talk1_4Ameha,	AudioVO);
		Control::Audio->LoadSample(aVOL7S4Talk1_5Ameha,	AudioVO);
		Control::Audio->LoadSample(aVOL7S4Talk1_6Mary,	AudioVO);

		_dialogVOArray.push_back(aVOL7S4Talk1_1Mary);
		_dialogVOArray.push_back(aVOL7S4Talk1_2Ameha);
		_dialogVOArray.push_back(aVOL7S4Talk1_3Ameha);
		_dialogVOArray.push_back(aVOL7S4Talk1_4Ameha);
		_dialogVOArray.push_back(aVOL7S4Talk1_5Ameha);
		_dialogVOArray.push_back(aVOL7S4Talk1_6Mary);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		//S1RajaHead->PlayAnimation(2);	//TALK ANIM

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L5S6TreeGuardian)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L5\\TREE.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S6TreeMouth = CharScene->GetObjectByID("mouth");
		//S6TreeMouth->PlayAnimation(1);

		isMaskEnabled = false;

		std::string dia_1 = "L5you";
		std::string dia_2 = "L5different";
		std::string dia_3 = "L5your";
		std::string dia_4 = "L5hello";
		

		_maryTalksArray[3] = true;
		

		Control::Audio->LoadSample(aVOL5S5Talk3_1Tree,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk3_2Tree,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk3_3Tree,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk3_3Mary,	AudioVO);
				

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		


		_dialogVOArray.push_back(aVOL5S5Talk3_1Tree);
		_dialogVOArray.push_back(aVOL5S5Talk3_2Tree);
		_dialogVOArray.push_back(aVOL5S5Talk3_3Tree);
		_dialogVOArray.push_back(aVOL5S5Talk3_3Mary);
				

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L5S6TreeGuardianAfterBadgePlaced)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L5\\TREE.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		S6TreeMouth = CharScene->GetObjectByID("mouth");
		S6TreeMouth->PlayAnimation(0);

		isMaskEnabled = false;

		std::string dia_1 = "L5fool";
		std::string dia_2 = "L5might";

		Control::Audio->LoadSample(aVOL5S5Talk4_1Tree,	AudioVO);
		Control::Audio->LoadSample(aVOL5S5Talk4_2Tree,	AudioVO);		

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);

		_dialogVOArray.push_back(aVOL5S5Talk4_1Tree);
		_dialogVOArray.push_back(aVOL5S5Talk4_2Tree);	

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L6S3FakeJames )
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L6\\FAKEJAMES.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		L6S3JamesFace = CharScene->GetObjectByID("Face");
		L6S3JamesFace->PlayAnimation(0);

		isMaskEnabled = false;

		std::string dia_1 = "L6james";
		std::string dia_2 = "L6how";
		std::string dia_3 = "L6mary";
		std::string dia_4 = "L6sophia";
		std::string dia_5 = "L6treasure";

		_maryTalksArray[0] = true;
		_maryTalksArray[1] = true;

		Control::Audio->LoadSample(aVOL6S3Talk1_1Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL6S3Talk1_2Mary,	AudioVO);		
		Control::Audio->LoadSample(aVOL6S3Talk1_3James,	AudioVO);
		Control::Audio->LoadSample(aVOL6S3Talk1_4James,	AudioVO);
		Control::Audio->LoadSample(aVOL6S3Talk1_5James,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);
		_dialogArray.push_back(dia_5);

		_dialogVOArray.push_back(aVOL6S3Talk1_1Mary);
		_dialogVOArray.push_back(aVOL6S3Talk1_2Mary);	
		_dialogVOArray.push_back(aVOL6S3Talk1_3James);
		_dialogVOArray.push_back(aVOL6S3Talk1_4James);
		_dialogVOArray.push_back(aVOL6S3Talk1_5James);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L6S4RockMan)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L6\\ROCKMAN.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		L6S4Mouth = CharScene->GetObjectByID("RockMan");
		L6S4Mouth->PlayAnimation(1);

		startRockTimer = true;

		isMaskEnabled = false;

		std::string dia_1 = "L6portal";
		std::string dia_2 = "L6heart";
		std::string dia_3 = "L6made";
		std::string dia_4 = "L6my";

		_maryTalksArray[2] = true;

		Control::Audio->LoadSample(aVOL6S4Talk1_1Rockman,	AudioVO);
		Control::Audio->LoadSample(aVOL6S4Talk1_2Rockman,	AudioVO);		
		Control::Audio->LoadSample(aVOL6S4Talk1_3Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL6S4Talk1_4Rockman,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);

		_dialogVOArray.push_back(aVOL6S4Talk1_1Rockman);
		_dialogVOArray.push_back(aVOL6S4Talk1_2Rockman);	
		_dialogVOArray.push_back(aVOL6S4Talk1_3Mary);
		_dialogVOArray.push_back(aVOL6S4Talk1_4Rockman);

		_numDialogLines = (int)_dialogArray.size();  //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L6S5Sophia)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L6\\SOPHIA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		/*L6S5Mouth = CharScene->GetObjectByID("Sophia");
		L6S5Mouth->PlayAnimation(0);*/

		IsSophiaTalking = true;
		isMaskEnabled = false;

		std::string dia_1 = "L6shoes";
		std::string dia_2 = "L6cake";
		std::string dia_3 = "L6bag";
		std::string dia_4 = "L6spent";

		_maryTalksArray[0] = true;
		_maryTalksArray[3] = true;

		Control::Audio->LoadSample(aVOL6S5Talk1_1Mary,	AudioVO);
		Control::Audio->LoadSample(aVOL6S5Talk1_2Sofia,	AudioVO);		
		Control::Audio->LoadSample(aVOL6S5Talk1_3Sofia,	AudioVO);
		Control::Audio->LoadSample(aVOL6S5Talk1_4Mary,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);
		_dialogArray.push_back(dia_3);
		_dialogArray.push_back(dia_4);

		_dialogVOArray.push_back(aVOL6S5Talk1_1Mary);
		_dialogVOArray.push_back(aVOL6S5Talk1_2Sofia);	
		_dialogVOArray.push_back(aVOL6S5Talk1_3Sofia);
		_dialogVOArray.push_back(aVOL6S5Talk1_4Mary);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L6S5SophiaPlayingWithTablet)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L6\\SOPHIATABLET.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		/*L6S5Mouth = CharScene->GetObjectByID("SophiaTablet");
		L6S5Mouth->PlayAnimation(0);*/

		isMaskEnabled = false;

		std::string dia_1 = "L6mom";


		Control::Audio->LoadSample(aVOL6S5Talk2_1Sofia,	AudioVO);

		_dialogArray.push_back(dia_1);

		_dialogVOArray.push_back(aVOL6S5Talk2_1Sofia);

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}
	else if( dialogId == eDialog_L7S3Anantha)
	{
		CharScene = new CHOScene();
		CharScene->Load("DIALOG\\L7\\ANANTHA.SCN");
		CharScene->SetZOff(eZOrder_CutsceneFull);

		L7AnanthaHead = CharScene->GetObjectByID("ananthatalk");
		L7AnanthaEyeBlink = CharScene->GetObjectByID("eyesBlink");

		L7AnanthaHead->PlayAnimation(1);

		L7BlinkTimer = 2.0f;

		std::string dia_1 = "L7wait";
		std::string dia_2 = "L7pain";

		Control::Audio->LoadSample(aVOL7S3Talk1_1Anatha,	AudioVO);
		Control::Audio->LoadSample(aVOL7S3Talk1_2Anatha,	AudioVO);

		_dialogArray.push_back(dia_1);
		_dialogArray.push_back(dia_2);

		_dialogVOArray.push_back(aVOL7S3Talk1_1Anatha);
		_dialogVOArray.push_back(aVOL7S3Talk1_2Anatha);	

		_numDialogLines = (int)_dialogArray.size(); //Sac: converted Implicit to explicit

		float _lineWidth = 750;
		int _nLines = 4;
		EFont _font = eFont_Italic24;
		eTextAlignment _align = Align_Center;
		std::string temp = _dialogArray.at(_activeDialogIndex);
		if(StringTable::IsKeyFound(temp))
		{
			_lineWidth = StringTable::GetLineWidth(temp);
			_nLines = StringTable::GetNoOfLines(temp);
			_font = (EFont)StringTable::GetFont(temp);
		}
		SAFE_DELETE(DialogText);
		DialogText = new MultilineText(_nLines, PointSystem::CreateCPoint(683,685,0), eZOrder_CutsceneFull+12, red, 30, _lineWidth, _align, _font);
		DialogText->SetTextMultiline(StringTable::getStr(temp));

		//DialogText->SetTextMultiline(StringTable::getStr(_dialogArray.at(_activeDialogIndex)));
	}

	//Color it
	ColorRGB defColor = {1.0f, 0.90f, 0.80f};
	ColorRGB maryColor = {0.18f, 0.96f, 0.32f};
	if(_maryTalksArray[_activeDialogIndex])
		DialogText->SetColor(maryColor);
	else
		DialogText->SetColor(defColor);

	if(quickShow)
	{
		_fadeAlpha = 1.0f;

		if(isMaskEnabled)
		{
			GFInstance->CreateGreyscaleScene();
			GFInstance->UpdateBlurScene(1.0f);
		}

		DialogScene->SetSceneAlpha(1.0f);
		CharScene->SetSceneAlpha(1.0f);

		//Play Dialog Voice Over. Since this is played after dialog fades in, in Normal cases
		Control::Audio->PlaySample((eAudio)_dialogVOArray.at(_activeDialogIndex)); //TODO Uncomment this

		DialogText->setAlpha(1.0f);
	}
	else
	{
		if(isMaskEnabled)
		{
			GFInstance->CreateGreyscaleScene();
			GFInstance->UpdateBlurScene(0); 
		}

		DialogScene->SetSceneAlpha(0);
		CharScene->SetSceneAlpha(0);

		_isFadingIn = true;
	}
}

void DialogSystem::CloseDialogSystem()
{
	_isActive = false;

	_fadeAlpha = 0;
	_isFadingOut = false;

	GFInstance->TurnOffBlurScene();
	
	

	//Dispose Loaded Stuffs
	SAFE_DELETE(DialogScene);
	SAFE_DELETE(CharScene);
	SAFE_DELETE(DialogText);

	_dialogArray.clear();
	_dialogVOArray.clear();

	switch (_dialogId)
	{
		case eDialog_S1RajaBeach:
			ProfileSystem::Instance->ActiveProfile.L1S1_IsTalkedWithRaja = true;
			
	Control::Audio->UnloadSample(aVOL1S1_1Raja);
	Control::Audio->UnloadSample(aVOL1S1_2Raja);
	Control::Audio->UnloadSample(aVOL1S1_3Raja);
	Control::Audio->UnloadSample(aVOL1S1_4Raja);
			break;
		case eDialog_S3RajaFoyer:
			ProfileSystem::Instance->ActiveProfile.L1S3_RajaTalkOver = true;
			

	Control::Audio->UnloadSample(aVOL1S3Talk1_1Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk1_2Mary);
	Control::Audio->UnloadSample(aVOL1S3Talk1_3Mary);
	Control::Audio->UnloadSample(aVOL1S3Talk1_4Mary);
	Control::Audio->UnloadSample(aVOL1S3Talk1_5Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk1_6Mary);

			break;
		case eDialog_S3RajaFoyerAskForLetter:
			ProfileSystem::Instance->ActiveProfile.L1S3_RajaAskedForCreekLetter = true;
			
	Control::Audio->UnloadSample(aVOL1S3Talk2_1Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk2_2Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk2_3Mary);
	Control::Audio->UnloadSample(aVOL1S3Talk2_4Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk2_5Mary);
	Control::Audio->UnloadSample(aVOL1S3Talk2_6Raja);
	Control::Audio->UnloadSample(aVOL1S3Talk2_7Raja);
	
			break;
	

		case eDialog_S3RajaFoyerDark:
	Control::Audio->UnloadSample(aVOL1S3Talk0_Raja);
			break;

       case eDialog_S3RajaFoyerShowLetter:
	Control::Audio->UnloadSample(aVOL1S3Talk3_Raja);
			break;

		case eDialog_S4Creek:
			SAFE_DELETE(cv_CreekPortalShowFX);
			SAFE_DELETE(cv_CreekPortalLoopFX);
			ProfileSystem::Instance->ActiveProfile.L1S4_IsCreekDialogPlayed = true;

	
	Control::Audio->UnloadSample(aVOL1S4Talk1_1Creek);
	Control::Audio->UnloadSample(aVOL1S4Talk1_2Creek);
	Control::Audio->UnloadSample(aVOL1S4Talk1_3Creek);
	Control::Audio->UnloadSample(aVOL1S4Talk1_4Creek);
			break;

	case eDialog_L2S5SMoopanTired:
			
	Control::Audio->UnloadSample(aVOL2S5Talk1_1Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk1_2Mary);
	Control::Audio->UnloadSample(aVOL2S5Talk1_3Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk1_4Mary);
			break;

	case eDialog_L2S5SMoopanDrinking:
			
	Control::Audio->UnloadSample(aVOL2S5Talk2_1Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk2_2Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk2_3Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk2_4Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk2_5Moopan);
	Control::Audio->UnloadSample(aVOL2S5Talk2_6Mary);
			break;

	case eDialog_L3S5_AnnMary:
			
			
	
	Control::Audio->UnloadSample(aVOL3S5Talk1_1Ann);
	Control::Audio->UnloadSample(aVOL3S5Talk1_2Ann);
	Control::Audio->UnloadSample(aVOL3S5Talk1_3Mary);
	Control::Audio->UnloadSample(aVOL3S5Talk1_4Ann);
			break;

		case eDialog_L5S4Anasooya:
			
			ProfileSystem::Instance->ActiveProfile.L5_S4_AnasuyaTalked = true;

	
	Control::Audio->UnloadSample(aVOL5S5Talk1_1Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk1_2Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk1_3Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk1_4Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk1_5Latika);
	
			break;

	case eDialog_L5S4AnasooyaFree:
			
			ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken = true;

	
	Control::Audio->UnloadSample(aVOL5S5Talk2_1Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk2_2Latika);
	Control::Audio->UnloadSample(aVOL5S5Talk2_3Latika);
			break;

	case eDialog_L7S4Ameha:
			
			ProfileSystem::Instance->ActiveProfile.L7_S4_AmehaTalked = true;

	
	Control::Audio->UnloadSample(aVOL7S4Talk1_1Mary);
	Control::Audio->UnloadSample(aVOL7S4Talk1_2Ameha);
	Control::Audio->UnloadSample(aVOL7S4Talk1_3Ameha);
	Control::Audio->UnloadSample(aVOL7S4Talk1_4Ameha);
	Control::Audio->UnloadSample(aVOL7S4Talk1_5Ameha);
	Control::Audio->UnloadSample(aVOL7S4Talk1_6Mary);
			break;

	case eDialog_L6S3FakeJames:
			
			//ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken = true;
	Control::Audio->UnloadSample(aVOL6S3Talk1_1Mary);
	Control::Audio->UnloadSample(aVOL6S3Talk1_2Mary);
	Control::Audio->UnloadSample(aVOL6S3Talk1_3James);
	Control::Audio->UnloadSample(aVOL6S3Talk1_4James);
	Control::Audio->UnloadSample(aVOL6S3Talk1_5James);
	break;

	case eDialog_L6S4RockMan:
			
			//ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken = true;
	Control::Audio->UnloadSample(aVOL6S4Talk1_1Rockman);
	Control::Audio->UnloadSample(aVOL6S4Talk1_2Rockman);
	Control::Audio->UnloadSample(aVOL6S4Talk1_3Mary);
	Control::Audio->UnloadSample(aVOL6S4Talk1_4Rockman);
			break;

	case eDialog_L6S5Sophia:

		//ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken = true;
		Control::Audio->UnloadSample(aVOL6S5Talk1_1Mary);
		Control::Audio->UnloadSample(aVOL6S5Talk1_2Sofia);
		Control::Audio->UnloadSample(aVOL6S5Talk1_3Sofia);
		Control::Audio->UnloadSample(aVOL6S5Talk1_4Mary);
		break;
	case eDialog_L6S5SophiaPlayingWithTablet:

		//ProfileSystem::Instance->ActiveProfile.L5_S4_InvChainTaken = true;
		Control::Audio->UnloadSample(aVOL6S5Talk2_1Sofia);
		break;

	case eDialog_L7S3Anantha:
		Control::Audio->UnloadSample(aVOL7S3Talk1_1Anatha);
		Control::Audio->UnloadSample(aVOL7S3Talk1_2Anatha);
		break;
	}

	SendMessage(ActionProcessed, ID, HOSM_DialogClosed);
}

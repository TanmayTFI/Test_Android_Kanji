//====================================
// Framework
// Inventory Control
//====================================

#include "FoundInvPopup.h"
#include "Object.h"
#include "Point.h"
#include "HOObject.h"
#include "GameFlow.h"
#include "SConstants.h"
#include "Background.h"
#include "Hud.h"
#include "DataEnums.h"
#include "AssetsStringTable.h"

extern HudControl* GFHud;

const CPoint CasePos(683.0f, 324.0f, 0.0f);
const CPoint BaseCloseOffPos(134+98.0f, -180.0f, 0.0f);
#define FOUNDINVPOPUP_HOOBJ_SCALE 0.8f

const CPoint FoundInvSprTextPos(683.0f, 445.0f, 0.0f);

const CRectangle InventoryRect(247,226,531,323);
FoundInvPopup::FoundInvPopup( int InvSize )
{
	IsWaitingClose = false;
	InventorySize = InvSize;
	State = FoundInvPopup_Inactive;
	Alpha = 0.0f;
	WaitingCloseTimer = 0.0f;
	InvRectClicked = false;

	Case = GFInstance->LoadBufferObject(FOUNDINV_BASE_OBJ);
	Case->SetZ(eZOrder_FoundInvPopup);

	BkGndBlackMaskSpr = GFInstance->LoadBufferObject(FOUNDINV_MASK_OBJ);
	//BkGndBlackMaskSpr->LoadSprite("UI\\MASK.SPR");
	BkGndBlackMaskSpr->SetZ(eZOrder_FoundInvPopup-1);
	
	FoundInvSprText = NULL;
	FoundInvSprText = new CBitmapText();
	FoundInvSprText->SetZ(eZOrder_FoundInvPopup+2);
	FoundInvSprText->SetAlignment(Align_Center);
	FoundInvSprText->SetPos(&FoundInvSprTextPos);
	FoundInvSprText->LoadFont(eFont_InvPop);//LoadFont(eFont_25);
	FoundInvSprText->SetText("Tuttifrutti Games");
	FoundInvSprText->SetColor((233/255.0f),(204/255.0f),(131/255.0f));	
	FoundInvSprText->Show(false);
	
	Case->SetPos(&CasePos);

	caseLabel = NULL;
	caseLabel = new CBitmapText();
	caseLabel->SetZ(eZOrder_FoundInvPopup + 50);
	caseLabel->SetAlignment(Align_Center);
	caseLabel->SetPos(PointSystem::CreateCPoint(683, 125, 0));
	caseLabel->LoadFont(eFont_InvPop);//LoadFont(eFont_25);
	caseLabel->SetText(AssetsStringTable::getString("youfound"));
	caseLabel->SetColor((233 / 255.0f), (204 / 255.0f), (131 / 255.0f));
	caseLabel->Show(true);
	//caseLabel->LoadInit("LOCALIZED\\" + StringTable::GetMyLangauge() + "\\UI\\YOUFOUND.SPR", Case->Pos.x, Case->Pos.y, Case->ZOrder + 10);
	//caseLabel->SetUID("youfoundtext");

	Show(false);
}

void FoundInvPopup::disposeObjects()
{
	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
	{
		SAFE_DELETE(FIHOObjects[i]->pSprite);
		SAFE_DELETE(FIHOObjects[i]);
	}
	FIHOObjects.clear();
}


FoundInvPopup::~FoundInvPopup()
{
	delete FoundInvSprText; FoundInvSprText = NULL;
	delete caseLabel; caseLabel = NULL;

	disposeObjects();
	
	

	//Garbage collection in gameflow
	Case->Show(false); Case = NULL;
	//delete BkGndBlackMaskSpr;
	BkGndBlackMaskSpr = NULL;

 if( GFInstance != NULL )   
	if( !GFInstance->IsPartialCacheMem )
	{
		GFInstance->UnloadBufferObject(FOUNDINV_BASE_OBJ); 	
		GFInstance->UnloadBufferObject(FOUNDINV_MASK_OBJ);
	}
	
}

void FoundInvPopup::Required()
{
	if( IsWaitingClose )
	{
		WaitingCloseTimer -= Control::LogicRate;
		if( WaitingCloseTimer <= 0.0f )
		{
			WaitingCloseTimer = 0.0f;
			IsWaitingClose = false;
			SendMessage(ActionProcessed, ID, FOUNDINVPOPUP_CLOSE);
			//Close(); -> We close it from Hud[parent] 
			
			//Enable focus again!! 			
			GFHud->InvBoxCtrl->SetModal(false);
			GFHud->InvBoxCtrl->Close();//forced close as not needed further!!
		}
		return;
	}
	if( State == FoundInvPopup_TransIn )
		UpdateTransIn();
	else if( State == FoundInvPopup_TransOut )
		UpdateTransOut();
	else if( State == FoundInvPopup_Idle )
	{
		if( GFHud->InvBoxCtrl->Modal )//disable it so that inventory popup will have focus!! 
			GFHud->InvBoxCtrl->SetModal(false);
	}
}

bool FoundInvPopup::IsEmptyInvBoxSlot( int i )
{
	return( GFHud->InvBoxCtrl->InvBoxObj[i].HOObj == NULL );//IS NON-EMPTY!!
}

void FoundInvPopup::FlyToInvBoxSlot( int iObj, int iSlot )
{
	//K_LOG("FLYING TO invbox SLOT"); 			
	CPoint itemTargetPos;
	itemTargetPos.x = GFHud->InvBoxCtrl->InvBox->Pos.x + InvBoxItemXOff + (InvBoxItemSpaceX * iSlot);//flying to invbox
	itemTargetPos.y = GFHud->InvBoxCtrl->InvBox->Pos.y + InvBoxItemYOff;
	FIHOObjects[iObj]->SetZ(eZOrder_INVBOXITEMS+10);
	FIHOObjects[iObj]->Remove(&itemTargetPos,1.0f,false,MAX_INVBOXOBJ_SIZE);
}



int FoundInvPopup::GetFreeSlotPos( int size )
{	
	//Find free slot pos from left - to right!!
	if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+0) )//VISUAL FEEL RIGHT
		return 0;
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+1) )
		return 1;
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+2) )
		return 2;
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+3) )
		return 3;
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+4) )
		return 4;
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+5) )		
		return 5; //if 2 objects will use 6th slot too...				
	else if( IsEmptyInvBoxSlot(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj+6) )
		return 6; // no shifting needed
	else //no empty space left - inv is now full (visually need to shift entire objects leftwards once/twice & then flyover to last position!!)
		return ProfileSystem::Instance->ActiveProfile.InvObjectCnt; // shifting1 maybe needed												
}

void FoundInvPopup::Update()
{
	if( IsWaitingClose )
		return;
	switch( State )
	{
        case FoundInvPopup_Idle:
			    break;
		case FoundInvPopup_Inactive: 
		case FoundInvPopup_TransIn:
		case FoundInvPopup_TransOut:
		case FoundInvPopup_Waiting: //Sac: added as this was missing, possible crash, handle it properly 
				return;
	}

	
	CPoint mosPos = Control::Input->Mpos();
	bool click = Control::Input->LBclicked();
	bool ClickedOutside = false;
	Cursor::SetMode(CA_Normal);
	if( !Case->Collide(&mosPos) ) //CLICKED OUTSIDE
	{		
		Cursor::SetMode(CA_HandPointy);
		if( click )
		{
			ClickedOutside = true;
			Control::Audio->PlaySample(aSFXClick);
		}
	}

	//bool InvRectClicked = false;
	if(!ClickedOutside && InventoryRect.Intersection(&mosPos) )
	{
		Cursor::SetMode(CA_HandPointy);
		if( click )
			InvRectClicked = true;	
	}

	if(InvRectClicked || ClickedOutside)
	{
		//K_LOG("Inv Transition Out starts...");
		State = FoundInvPopup_TransOut;
		Control::Audio->PlayCollectFX();

		
		///////////////////////////////////////////////////////////
		int size = (int)FIHOObjects.size();
			

		int FreeSlotPos = GetFreeSlotPos(size);
						
		int MultiPieceSlot = 0;
		switch( size )
		{
			//maximum handles 2 inv objects!!
			case 1: //K_LOG("1 Object");
					MultiPieceSlot = GFHud->InvBoxCtrl->GetMultiPiece_InvBoxObjSlot( GFHud->InvBoxCtrl->GetRemapInvId(FIHOObjects[0]->eInv_Id) );
					if( MultiPieceSlot != -1 )//Already present, just fly to that location!!
					{
						//K_LOG("Multipiece Object: 1");
						if((MultiPieceSlot-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj) >= MAX_INVBOX_DISPLAYED_OBJECTS )//greater than scene, make it rightmost element!! 
								GFHud->InvBoxCtrl->ShiftInvBoxLeft(MultiPieceSlot-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);
						else if( MultiPieceSlot < ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj )//lesser than scene, make it leftmost element!!
								GFHud->InvBoxCtrl->ShiftInvBoxRight(ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj-MultiPieceSlot);
							
						
						FlyToInvBoxSlot(0,MultiPieceSlot-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);						
						GFHud->InvBoxCtrl->InvBoxObj[MultiPieceSlot].nPiecePicked++;
						GFHud->InvBoxCtrl->InvBoxObj[MultiPieceSlot].Text->SetText("("+std::to_string(GFHud->InvBoxCtrl->InvBoxObj[MultiPieceSlot].nPiecePicked)+"/"+std::to_string(GFHud->InvBoxCtrl->InvBoxObj[MultiPieceSlot].MaxPieces)+")");
					}
					else //fly this object to the last free slot available!!
					{
						if( FreeSlotPos <= (MAX_INVBOX_DISPLAYED_OBJECTS-1) )
							FlyToInvBoxSlot(0,FreeSlotPos);	
						else //if( FreeSlotPos > (MAX_INVBOX_DISPLAYED_OBJECTS-1) )							
						{
							//Fly to last slot!!
							GFHud->InvBoxCtrl->ShiftInvBoxLeft(ProfileSystem::Instance->ActiveProfile.InvObjectCnt-(MAX_INVBOX_DISPLAYED_OBJECTS-1)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);			
							FlyToInvBoxSlot(0,(MAX_INVBOX_DISPLAYED_OBJECTS-1));
						}						
					}
					break;
			case 2: //K_LOG("2 Objects"); //Fly them to end of list "ALWAYS"!! (Warning: Can't handle mutlipiece sliding at all!!! as 2 slots maybe "outside scene" case!!)
					if( FreeSlotPos <= (MAX_INVBOX_DISPLAYED_OBJECTS-2) )
					{
						FlyToInvBoxSlot(0,FreeSlotPos);	
						//shifting already handled above!!
						FlyToInvBoxSlot(1,FreeSlotPos+1);	
					}
					else //if( FreeSlotPos >= (MAX_INVBOX_DISPLAYED_OBJECTS-1) )							
					{
						//Fly to last slot!!
						GFHud->InvBoxCtrl->ShiftInvBoxLeft(ProfileSystem::Instance->ActiveProfile.InvObjectCnt-(MAX_INVBOX_DISPLAYED_OBJECTS-2)-ProfileSystem::Instance->ActiveProfile.iInvBoxStartObj);			
						FlyToInvBoxSlot(0,(MAX_INVBOX_DISPLAYED_OBJECTS-2));	
						FlyToInvBoxSlot(1,(MAX_INVBOX_DISPLAYED_OBJECTS-1));	
					}											
					break;
			default: 
						KMiscTools::messageBox((const char*)StringTable::getStr("ExErrTitle").c_str(),(const char*)StringTable::getStr("ExErrFoundInvPopop").c_str() );			
					exit(0);
					break;	
		}	
		///////////////////////////////////////////////////////////
		ProfileSystem::Instance->ActiveProfile.InvObjectCnt += size;
		SendMessage(ActionProcessed, ID, INVBOX_OPEN);
		Control::Input->ClearMouse();
		return;
	}
}

void FoundInvPopup::ActivateDirect(std::vector<int> *vecInvIds, CPoint mpos)
{
	CPoint pos;

	int randId = rand()%3;
	switch (randId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
	default:
		break;
	}

	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
	{
		delete FIHOObjects[i]->pSprite; FIHOObjects[i]->pSprite= NULL;
		delete FIHOObjects[i]; FIHOObjects[i] = NULL; 
	}	

	FIHOObjects.clear();
	//K_LOG("big sprite loDING...");
	float InvXOff, InvYOff, InvSpace;
	std::string ObjectsStr = "";
	InvXOff  = 0;
	InvSpace = 0.0f;
	InvYOff  = 68;

	pos = mpos;
							
	//pos.x += InvXOff + InvSpace;
	//pos.y += InvYOff;
							
	CHOObject *HOObj = new CHOObject();
	HOObj->eInv_Id = (*vecInvIds)[0];
	HOObj->LoadSprite(InvBigSprNames[HOObj->eInv_Id]);								
	HOObj->SetZ(eZOrder_FoundInvPopup+1);
	HOObj->SetPos(&pos);	
	HOObj->SetScale(FOUNDINVPOPUP_HOOBJ_SCALE);
	switch( HOObj->eInv_Id )
	{
		//Multi Inv Items
		case eInv_L1S1_RubyPart1:
		case eInv_L1S1_RubyPart2:
		case eInv_L1S1_RubyPart3:
		case eInv_L1S1_RubyPart4:HOObj->PlayAnimation(1);break; 
	}

	

	FIHOObjects.push_back(HOObj);
								
	ObjectsStr = StringTable::getStr(InventoryDescrips[HOObj->eInv_Id]);		

	int strLen = (int) ObjectsStr.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		ObjectsStr[i] = toupper(ObjectsStr[i]);
	FoundInvSprText->SetText(ObjectsStr);
	caseLabel->SetText(AssetsStringTable::getString("youfound"));
	ShowDirect(true);
	State = FoundInvPopup_TransIn;
	//SetModal(true);

	Alpha = 0.0f;
	Case->SetAlpha(Alpha);
	BkGndBlackMaskSpr->SetAlpha(Alpha);
	FoundInvSprText->SetAlpha(Alpha);
	caseLabel->SetAlpha(Alpha);

	int Objsize = (int)FIHOObjects.size();
	for(int i=0; i < Objsize; ++i)
	{
		FIHOObjects[i]->SetAlpha(Alpha);
		//GFHud->InvBoxCtrl->AddToInvBox(FIHOObjects[i]->eInv_Id,&FIHOObjects[i]->TargetPos);
	}

	GFInstance->IsActivePopup = true;	
														
							
}

void FoundInvPopup::ShowDirect(bool f)
{
	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
	{
		FIHOObjects[i]->Show(f);
	}

	FoundInvSprText->Show(false);
	Case->Show(false);
	caseLabel->Show(false);
	BkGndBlackMaskSpr->Show(f);
	InvRectClicked = true;
}


void FoundInvPopup::Activate(std::vector<int> *vecInvIds)
{
	CPoint pos;

	int randId = rand()%3;
	switch (randId)
	{
	case 0:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv1);		break;
	case 1:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv2);		break;
	case 2:		Control::Audio->QuickLoadAndPlaySFX(aSFXInvCollectv3);		break;
	default:
		break;
	}

	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
	{
		delete FIHOObjects[i]->pSprite; FIHOObjects[i]->pSprite= NULL;
		delete FIHOObjects[i]; FIHOObjects[i] = NULL; 
	}	

	FIHOObjects.clear();

	float InvXOff, InvYOff, InvSpace;
	std::string ObjectsStr = "";
	switch( InventorySize )
	{
		case INVENTORY_V1:	InvXOff  = 0;
							InvSpace = 0.0f;
							InvYOff  = -40;

							pos = CasePos;
							
							pos.x += InvXOff + InvSpace;
							pos.y += InvYOff;
							{
								CHOObject *HOObj = new CHOObject();
								HOObj->eInv_Id = (*vecInvIds)[0];
								HOObj->LoadSprite(InvBigSprNames[HOObj->eInv_Id]);								
								
								HOObj->SetZ(eZOrder_FoundInvPopup+1);
								HOObj->SetPos(&pos);
								FIHOObjects.push_back(HOObj);
								
								ObjectsStr = StringTable::getStr(InventoryDescrips[HOObj->eInv_Id]);								
							}							
							break;	
		
		case INVENTORY_V2:	InvYOff  = -40;
							if( vecInvIds->size() == 3 )
							{
								InvXOff = -98;//50.0f;
								InvSpace = 90.0f;
							}
							else
							{
								InvXOff = -75.0f;
								InvSpace = 130.0f;
							}

							
							for(unsigned int i = 0; i < vecInvIds->size(); ++i)
							{
								pos = CasePos;
								pos.x += InvXOff + (InvSpace * i);
								pos.y += InvYOff;

								CHOObject *HOObj = new CHOObject();
								HOObj->eInv_Id = (*vecInvIds)[i];
								HOObj->LoadSprite(InvBigSprNames[HOObj->eInv_Id]);								
								HOObj->SetZ(eZOrder_FoundInvPopup+1);
								HOObj->SetPos(&pos);
								HOObj->SetScale(FOUNDINVPOPUP_HOOBJ_SCALE);
								FIHOObjects.push_back(HOObj);
								if( ObjectsStr == "" )
									ObjectsStr = StringTable::getStr(InventoryDescrips[HOObj->eInv_Id]);
								else
									ObjectsStr += std::string(", "+StringTable::getStr(InventoryDescrips[HOObj->eInv_Id]));
								
							}
							
							break;
		default:			
							break;
	}

	int strLen = (int)ObjectsStr.size(); //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		ObjectsStr[i] = toupper(ObjectsStr[i]);
	FoundInvSprText->SetText(ObjectsStr);
	caseLabel->SetText(AssetsStringTable::getString("youfound"));
	Show(true);
	State = FoundInvPopup_TransIn;
	//SetModal(true);

	Alpha = 0.0f;
	Case->SetAlpha(Alpha);
	BkGndBlackMaskSpr->SetAlpha(Alpha);
	FoundInvSprText->SetAlpha(Alpha);
	caseLabel->SetAlpha(Alpha);

	int Objsize = (int)FIHOObjects.size();
	for(int i=0; i < Objsize; ++i)
	{
		FIHOObjects[i]->SetAlpha(Alpha);
		//GFHud->InvBoxCtrl->AddToInvBox(FIHOObjects[i]->eInv_Id,&FIHOObjects[i]->Pos);
	}

	GFInstance->IsActivePopup = true;	

}

void FoundInvPopup::Show(bool f)
{
	InvRectClicked = false;
	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
	{
		FIHOObjects[i]->Show(f);
	}

	FoundInvSprText->Show(f);
	Case->Show(f);
	caseLabel->Show(f);
	BkGndBlackMaskSpr->Show(f);
}

void FoundInvPopup::Close()
{
	GFInstance->IsActivePopup = false;
	//Control::Audio->PlaySample(aSFXClick);
	//Kill Inventory Popup at this point
	State = FoundInvPopup_Inactive;


	Show(false);
	SetModal(false);

	SendMessage(ActionProcessed, ID, INV_CLOSE);
}


void FoundInvPopup::UpdateTransIn()
{
	Alpha += 2.0f * Control::LogicRate;

	if(Alpha >= 1.0f)
	{
		Alpha = 1.0f;
		State = FoundInvPopup_Idle;
	}

	
	Case->SetAlpha(Alpha);
	//BkGndBlackMaskSpr->SetAlpha(Alpha);
	FoundInvSprText->SetAlpha(Alpha);
	caseLabel->SetAlpha(Alpha);


	int size = (int)FIHOObjects.size();
	for(int i=0; i < size; ++i)
		FIHOObjects[i]->SetAlpha(Alpha);
}

void FoundInvPopup::UpdateTransOut()
{
	bool objectsFlying = false;
	int size = (int)FIHOObjects.size();
	// int j = 0; //Sac: Unused variable, so commented it
	for(int i=0; i < size; ++i)
		if( FIHOObjects[i]->Status != eHO_Hidden )
		{
			if( FIHOObjects[i]->Status != eHO_Finished )
			{
				objectsFlying = true;
				//K_LOG("OBJECTS FLYING....");
			}
			else //flying objects!!
			{
				if( FIHOObjects[i]->eInv_Id == -1 )
				{
						KMiscTools::messageBox((const char*)StringTable::getStr("ExErrTitle").c_str(),(const char*)StringTable::getStr("ExErrInvalidInvSprite").c_str() );			
					exit(0);
				}
				else
				{
					//K_LOG("Flied Obj added to InvBox");
					//GFHud->InvBoxCtrl->PostUpdateAddingToInvBox(FIHOObjects[i]->eInv_Id);
					GFHud->InvBoxCtrl->AddToInvBox(FIHOObjects[i]->eInv_Id,&FIHOObjects[i]->TargetPos);
				}
				
				FIHOObjects[i]->Status = eHO_Hidden;
			}
		}

	Alpha -= 2.0f * Control::LogicRate;

	if(Alpha <= 0.0f)
	{
		Alpha = 0.0f;

		if( objectsFlying == false )
		{
			WaitingCloseTimer = 0.35f;
			IsWaitingClose = true;		
			State = FoundInvPopup_Waiting;
			//K_LOG("INV WAITING CLOSE");

			for(int i=0; i < size; ++i)
			{
				if( FIHOObjects[i] != NULL && FIHOObjects[i]->Status == eHO_Finished )
				{
					delete FIHOObjects[i]->pSprite; FIHOObjects[i]->pSprite= NULL;
					delete FIHOObjects[i]; FIHOObjects[i] = NULL;
				}
			}
			FIHOObjects.clear();
		}
	}

	
	Case->SetAlpha(Alpha);
	caseLabel->SetAlpha(Alpha);
	FoundInvSprText->SetAlpha(Alpha);

	//int size = (int)FIHOObjects.size();
	//for(int i=0; i < size; ++i)
		//FIHOObjects[i]->SetAlpha(Alpha);
}

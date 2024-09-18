//====================================
//	DQFYLH
//	HO System
//	ExtrasCutscenePlayer
//====================================

#include "ExtrasCutscenePlayer.h"
#include "Application.h"
#include "CutsceneEnums.h"
#include "HOScene.h"
#include "AssetsStringTable.h"

const std::string VideoPaths[eCutscene_Max] =
{
	"Data\\HAVEYOU.OGV",				//  eCutscene_BackStory,
	"Data\\START.OGV",					//	eCutscene_L1Intro,
	"OGV\\L1S1\\KIDSPAINTING.OGV",		//	eCutscene_L1SophiaPainting,
	"OGV\\L1S4\\S4CREEKTALKS.OGV",		//	eCutscene_L1CreekNarration,
	"OGV\\L1S6\\PAPERCUTSCENE.OGV",		//	eCutscene_L1NewsPaper,
	"Data\\OUTRO.OGV",					//	eCutscene_L1EndCutScene,
	"OGV\\L2\\S1INTROCUTSCN.OGV",		//	eCutscene_L2Intro,
	"OGV\\L2\\S5MOOPPANTHEYYAM.OGV",	//	eCutscene_L2MoopanTheyyam,
	"OGV\\L2\\S6ENDCUTSCENE.OGV",		//	eCutscene_L2EndCutScene,
	"OGV\\L3\\S3SHOEMEMORY.OGV",		//	eCutscene_L3ShoeMemory,
	"OGV\\L3\\S4AMEEHADIARY.OGV",		//	eCutscene_L3DiaryNote,
	"OGV\\L4\\S4MEMORY.OGV",			//	eCutscene_L4FlowerMemory,
	"OGV\\L4\\S3AMEEHADIARYNOTE.OGV",	//	eCutscene_L4DiaryNote,
	"OGV\\L4\\S3ENDCUTSCENE.OGV",		//	eCutscene_L4EndCutScene,
	"OGV\\L5\\S1TIGERSHOOT.OGV",		//	eCutscene_L5GrandmaCutscene,
	"OGV\\L5\\S6OUTRO.OGV",				//	eCutscene_L5EndPeacockCutScene,
	"OGV\\L6\\S3FAKEJAMES.OGV",			//	eCutscene_L6FakeJames,
	"OGV\\L6\\S1VARUNICURSE.OGV",		//	eCutscene_L6VaruniCutScene,
	"OGV\\L6\\S5ENDCUTSCENE.OGV",		//	eCutscene_L6EndPortalCutscene,
	"OGV\\L7\\S1LASYAILLUTION.OGV",		//	eCutscene_L7LasyaIllusion,
	"OGV\\L7\\S3NIGHTMARE.OGV",			//	eCutscene_L7MarysNightmare,
	"OGV\\L7\\S3LASYAATTACK.OGV",		//	eCutscene_L7LasyaAttack,
	"OGV\\L7\\S4ENDCUTSCENE1.OGV",		//	eCutscene_L7EndCutScene1,
	"OGV\\L7\\S4ENDCUTSCENE2.OGV",		//	eCutscene_L7EndCutScene2,
	"OGV\\L8\\S1INTROCUTSCENE.OGV",		//	eCutscene_L8Intro,
	"OGV\\L8\\S2MAMMOTHINTRO.OGV",		//	eCutscene_L8FlyingMammoth,
	"OGV\\L8\\S5CLIMAX.OGV"				//	eCutscene_L8EndCutScene,
};

//if video is in data folder
const bool isDataVideo[eCutscene_Max] =
{
	true,	//eCutscene_Backstory
	true,	//eCutscene_L1S1_Intro
	false,	//
	false,	//
	false,	//
	true,	//eCutscene_L1EndCutScene
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false,	//
	false
};

ExtrasCutscenePlayer::ExtrasCutscenePlayer()
{
	if(!ProfileSystem::Instance->_isExtrasGamePlay)
	{
	
	LoadScene("EXTRAS\\CUTSCN\\CUTSCN.SCN");
	HOScene->GetObjectByID("Mask")->SetScale(2.0f);
	GFInstance->PostUpdate(1);
	GFInstance->IsScnElmntsPreloaded[eArea_ExtrasCutscenePlayer] = true;
	{
		//Setting the Current Area, to prevent if user Force exit while here
		ProfileSystem::Instance->ActiveProfile.CurrentArea = ProfileSystem::Instance->ActiveProfile.ExtrasRoomPrevArea;
	}

	cv_Cutscn = NULL;
	time = 0;
	subIndex = 0;
	subMax = 0;

	subtitleText = new CBitmapText();
	subtitleText->SetZ(10000);
	subtitleText->LoadFont(eFont_26);
	subtitleText->SetText("");
	subtitleText->SetAlignment(Align_Center);
	subtitleText->SetColor(0.9, 0.9, 0.9);
	subtitleText->SetPos(PointSystem::CreateCPoint(685, 745, 0));

	vid_time = new CBitmapText();
	vid_time->SetZ(10000);
	vid_time->LoadFont(eFont_25);
	//GFApp->lpFontSmall->setHeightPix(fontSize);
	vid_time->SetText("");
	//LineTexts[numTextLines]->SetScale(scale);
	vid_time->SetAlignment(Align_Center);
	vid_time->SetColor(0.9, 0.9, 0.9);
	vid_time->SetPos(PointSystem::CreateCPoint(685, 500, 0));

	//get Video ID
	if(GFInstance->activeCutsceneID == -1)
	{
		#ifdef K_WIN32
			WARN(StringTable::getString("ERR_CutScn"));
		#else
			WARN(StringTable::getString("ERR_CutScn"));
		#endif
	}
	else
	{
	//	CRectangle RectVidFull(171, 96, 1024, 576);
		CRectangle RectVidFull(GFApp->video_startX, GFApp->video_startY, GFApp->video_width,GFApp->video_height);
        std::string vid_name = VideoPaths[GFInstance->activeCutsceneID];
        if (GFInstance->activeCutsceneID ==0)
        {
            #if !defined _CEBUILD
                vid_name =std::string("Data\\HAVEYOU_SE.OGV");
            #endif
        }
		cv_Cutscn = new CVideo(vid_name, RectVidFull, false, eZOrder_CutsceneFull, NULL, NULL, isDataVideo[GFInstance->activeCutsceneID]);
		//cv_Cutscn->SetScale(1.334f);
		cv_Cutscn->SetScale(GFApp->video_aspectRatio);
		cv_Cutscn->PlayFadeInVideo(2.0f);
		cv_Cutscn->SetSkipActive();

		if (GFInstance->activeCutsceneID == 0)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("haveyou6"));
			subMax = 6;
		}
		else if (GFInstance->activeCutsceneID == 1)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("start1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("start2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("start3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("start4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("start5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("start6"));
			subMax = 6;
		}
		else if (GFInstance->activeCutsceneID == 2)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("kidspainting1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("kidspainting2"));
			subMax = 2;
		}
		else if (GFInstance->activeCutsceneID == 3)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("creektalk3"));
			subMax = 3;
		}
		else if (GFInstance->activeCutsceneID == 4)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("papercut3"));
			subMax = 3;
		}
		else if (GFInstance->activeCutsceneID == 5)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("outro1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("outro2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("outro3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("outro4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("outro5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("outro6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("outro7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("outro8"));
			subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("outro9"));
			subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("outro10"));
			subMax = 10;
		}
		else if (GFInstance->activeCutsceneID == 6)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l2intro3"));
			subMax = 3;
		}
		else if (GFInstance->activeCutsceneID == 7)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("moopantheyyam4"));
			subMax = 4;
		}
		else if (GFInstance->activeCutsceneID == 8)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end8"));
			subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end9"));
			subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end10"));
			subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end11"));
			subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end12"));
			subtitles[12] = AssetsStringTable::getSub(AssetsStringTable::getString("l2end13"));
			subMax = 13;
		}
		else if (GFInstance->activeCutsceneID == 9)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("shoememory5"));
			subMax = 5;
		}
		else if (GFInstance->activeCutsceneID == 10)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary8"));
			subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary9"));
			subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary10"));
			subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary11"));
			subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiary12"));
			subMax = 12;
		}
		else if (GFInstance->activeCutsceneID == 11)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("s4memory4"));
			subMax = 4;
		}
		else if (GFInstance->activeCutsceneID == 12)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote8"));
			subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote9"));
			subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote10"));
			subtitles[10] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote11"));
			subtitles[11] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote12"));
			subtitles[12] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote13"));
			subtitles[13] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote14"));
			subtitles[14] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote15"));
			subtitles[15] = AssetsStringTable::getSub(AssetsStringTable::getString("ameehadiarynote16"));
			subMax = 16;
		}
		else if (GFInstance->activeCutsceneID == 13)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("l4end6"));
			subMax = 6;
		}
		else if (GFInstance->activeCutsceneID == 14)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("tigershoot"));
			subMax = 1;
		}
		else if (GFInstance->activeCutsceneID == 15)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("s6outro1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("s6outro2"));
			subMax = 2;
		}
		else if (GFInstance->activeCutsceneID == 16)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("fakejames1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("fakejames2"));
			subMax = 2;
		}
		else if (GFInstance->activeCutsceneID == 17)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("varunicurse5"));
			subMax = 5;
		}
		else if (GFInstance->activeCutsceneID == 18)
		{
			//No Subtitles
		}
		else if (GFInstance->activeCutsceneID == 19)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lasyaillusion"));
			subMax = 1;
		}
		else if (GFInstance->activeCutsceneID == 20)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("nightmare4"));
			subMax = 4;
		}
		else if (GFInstance->activeCutsceneID == 21)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack8"));
			subtitles[8] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack9"));
			subtitles[9] = AssetsStringTable::getSub(AssetsStringTable::getString("lv7lasyaattack10"));
			subMax = 5;
		}
		else if (GFInstance->activeCutsceneID == 22)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene5"));
			subMax = 5;
		}
		else if (GFInstance->activeCutsceneID == 23)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_4"));
			subtitles[4] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_5"));
			subtitles[5] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_6"));
			subtitles[6] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_7"));
			subtitles[7] = AssetsStringTable::getSub(AssetsStringTable::getString("endcutscene2_8"));
			subMax = 8;
		}
		else if (GFInstance->activeCutsceneID == 24)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("lv8intro3"));
			subMax = 3;
		}
		else if (GFInstance->activeCutsceneID == 25)
		{
			//No Subtitles
		}
		else if (GFInstance->activeCutsceneID == 26)
		{
			subtitles[0] = AssetsStringTable::getSub(AssetsStringTable::getString("climax1"));
			subtitles[1] = AssetsStringTable::getSub(AssetsStringTable::getString("climax2"));
			subtitles[2] = AssetsStringTable::getSub(AssetsStringTable::getString("climax3"));
			subtitles[3] = AssetsStringTable::getSub(AssetsStringTable::getString("climax4"));
			subMax = 4;
		}
	}
	}
}


void ExtrasCutscenePlayer::Update()
{
	Cursor::SetMode(CA_Normal);

	CPoint pos = Control::Input->Mpos();

	if(cv_Cutscn==NULL)
	{
		if(Control::Input->LBclicked())
		{
			SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			return;
		}
	}
	else
	{
		//Subtitles Code
		time = cv_Cutscn->GetTime();

		if (time >= subtitles[subIndex].begin && time < subtitles[subIndex].end)
		{
			subtitleText->SetText(subtitles[subIndex].text);
		}
		else
			subtitleText->SetText("");
		
		if (time > subtitles[subIndex].end)
		{
			if(subMax-1 > subIndex)
				subIndex++;
		}

		//vid_time->SetText(std::to_string(time));
		/*
		else
		{
			subtitleText->SetText(std::to_string(time));
		}
		*/
		//

		if(cv_Cutscn->isSkipMouseOver())
			Cursor::SetMode(CA_HandPointy);
		if(cv_Cutscn->isEndReached())
		{
			subtitleText->SetText("");
			GFInstance->activeCutsceneID = -1;
			cv_Cutscn->StopVideo();
			SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
			return;
		}
	}
#ifdef ENABLE_ANDROID_BACK_BUTTON
		if (Control::Input->isKeyBackSensed()) {
			if(cv_Cutscn==NULL)
				{	
					SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
					return;
				}
			else
			{
				GFInstance->activeCutsceneID = -1;
				cv_Cutscn->StopVideo();
				SendMessage(ControlExit, ID, HOSM_ReturnToMenu);
				return;
			}
		}
#endif






}

ExtrasCutscenePlayer::~ExtrasCutscenePlayer()
{
	SAFE_DELETE(cv_Cutscn);

	SAFE_DELETE(subtitleText);
}

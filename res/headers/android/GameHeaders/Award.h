//===========================================================
// 
// Award
// Sujin
//===========================================================

#ifndef AWARD_H
#define AWARD_H

enum AwardCategory
{
	kAward_Collect_30_Awards = 0,			//0		0 DRONA
	kAward_HO_Without_Hint_1,				//1		1 NA SUCITA 1
	kAward_HO_Without_Hint_3,				//2		32 NA SUCITA 2
	kAward_HO_Without_Hint_5,				//3		33 NA SUCITA 3
	kAward_HO_Without_Hint_10,				//4		34 NA SUCITA 4
	kAward_MangoMiniGame_Without_Misthrow,				//5		2 Arjuna
	kAward_HO_Within_Time_150,				//6		3 Vayu
	kAward_HO_Within_Time_90,				//7		3 Vayu
	kAward_HO_Within_Time_30,				//8		3 Vayu
	kAward_HO_With_Match3,					//9		4 Sama kanta
	kAward_Mini_Without_Skip_2,				//10	5 Agni
	kAward_Mini_Without_Skip_4,				//11	5 Agni
	kAward_Mini_Without_Skip_6,				//12	5 Agni
	kAward_Mini_Within_Time_1,				//13	6 GURU
	kAward_Mini_Jigsaw_Mohenjodaro_3,			//14	7 NIRMOKTR
	kAward_Pick5_WithinTime,				//15	8 PRASPHURANA
	kAward_Game_without_Skip,				//16	9 CHANAKYA
	kAward_Game_Without_Hint,				//17	10 MA CARA
	kAward_Game_Without_Hint_HO,			//18	11 HOP PATI
	kAward_Game_Without_Skip_CutScene,      //19	12 TARTARITI
	kAward_Game_Within_Time_10hr,			//20	13 KARMA
	kAward_Game_Within_Time_4hr,			//21	13 KARMA
	kAward_Start_Journey_Narara,					//22	14 PRARAMBHA
	kAward_MainStory_OnExpert,				//23	15 DEVARUPIN
	kAward_CreeksMansion_Without_hint,		//24	16 AVARA CARA
	kAward_FastTravel,						//25	17 SIDDHI
	kAward_TreasureRoom_Escape,				//26	18 UDGHATIN
	kAward_Collect_Inventory_Item_33,       //27	19 UPALAMBHAKA
	kAward_Collect_Memory_1,				//28	20 SITA
	kAward_Collect_Memory_All,				//29	21 MATR RAKSA
	kAward_Collect_Morphing_1,				//30	22 DRASTTA
	kAward_Collect_Morphing_All,			//31	23 GARUDA
	kAward_Game_Without_Tutorial,			//32	24 ADHIMUKTI
	kAward_TabletMiniGame_Without_Hint,					//33	25 ZISAVA
	kAward_Unveiled_Maskedman,					//34	26 PATHAKA
	kAward_Find_Who_Wreck_Indus,			//35	27 KAUSHIK
	kAward_Visit_4_World,					//36	28 PATHIKA
	kAward_Save_Sophia,						//37	29 RAKSHAK
	kAward_Tame_Magical_peacock,			//38	30 DAMAKA
	kAward_Decrypt_Indus_Script,			//39	31 VIGRAHA
	kAward_Complete_Mini_Soldier_Without_Hint,						//40	35 AJNA CHAKRA - To Add

	kAward_Max
};

class Award
{

private:

	bool _isCompleted;
	int _totalNumOfEventsToComplete;
	int _currentNumOfEvents;


public:

	AwardCategory _awardCategory;
	const char * _awardSteamId;

	Award(AwardCategory awardCategory, int totalNumOfEventsToComplete, int currentNumOfEvents);
	bool GetISCompleted();
	void UpdateEvents(int count);
	int GetTotalNumOfEventsToComplete();
	
};

#endif
	
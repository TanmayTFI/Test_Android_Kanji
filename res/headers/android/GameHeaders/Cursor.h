//====================================
// Framework
// Cursor Class
//====================================

#ifndef FRMWRK_CURSOR_DEF
#define FRMWRK_CURSOR_DEF true
#include "VideoDefs.h"
#include "LangDefs.h"

enum eCursorAnim
{
	CA_Normal,
	CA_Look,	
	CA_HandPointy,
	CA_ExitUp,
	CA_ExitDown,
	CA_ExitLeft,
	CA_ExitRight,
	CA_ExitRightDiag,
	CA_ExitLeftDiag,
	CA_Gear,
	CA_Help,
	CA_Speech,
	CA_HandTake
};

class CObject;
class CPoint;
class CBitmapText;

class Cursor
{
	public:
		#ifdef JAPANESE_BUILD
			static const char* JapSceneNames( int rDescId );
		#endif

		static CObject   *rDescTip;
		static CBitmapText *rDescText;

		//CObject* GetSprite(){ return Sprite; }
		static CObject           *Sprite;
		static CObject           *CurObjs[8];
		static int currentMode;

		static void Create();
		static void Destroy();

		static void SetInMenu(bool f); // only used in Android as of now
		static void Show(bool f);
		static void SetPos(const CPoint* pos);
		static void SetXY( float x, float y );
		static float GetX();
		static float GetY();

		static bool IsSystemCursor();
		static void HideSystemCursor();
		static void UseSystemCursor();

		static void SetMode(int mode, int rDescId = -1);
		static int  GetMode();

		static bool		 Visible;
	private:		
		static bool		 SystemCursor;
		static bool		 inMenu; // only used in Android now

		Cursor();
		~Cursor();
};


#endif

//====================================
// Utils
// SuperLog
// - Faisal
//====================================

#ifndef UTILS_SUPERLOG_DEF
#define UTILS_SUPERLOG_DEF true

#include "Framework.h"
#include "Control.h"

class CObject;
class CBitmapText;

#define NUM_SLOG_LINES 12

class SLOG:public Control
{
	protected:
		static CBitmapText *_sLogTexts[NUM_SLOG_LINES];
		static CObject	*_maskSprite;
		static int _lastIndex;

		static std::string	_storedTxtArray[NUM_SLOG_LINES];
		static int			_logCount[NUM_SLOG_LINES];

		static int			_logChangeIndex;

		static long	_logStartTime;
		static long	_logEndTime;

	public:
		static void Create();
		static void Destroy();

		static void Show(bool f);

		static void DumpTextsToKLOG();

		static bool checkPreviousLogsAndIncrementIndex(std::string txt);

		static void LogLoadingStartTime();
		static void LogLoadingEndTime(std::string loadItemName);

		static void Log(std::string txt);
		static void Log(std::string txt, int val);
		static void Log(std::string txt, float val);
		static void Log(std::string txt, CPoint* pt);
		static void Log(std::string txt, std::vector<int> vecint, int numCols=0);
		static void Log(std::string txt, int* arrayPt, int numItems);

		static bool Visible;
	private:
		SLOG();
		~SLOG();
};


#endif
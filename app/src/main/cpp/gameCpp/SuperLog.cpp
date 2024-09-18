//====================================
// Utils
// Super Log
//====================================

#include "SuperLog.h"
#include "Object.h"
#include "BitmapText.h"
#include "GameFlow.h"
#include "Fonts.h"
#include "Util.h"

extern HudControl * GFHud;

CBitmapText*	SLOG::_sLogTexts[NUM_SLOG_LINES] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
CObject*		SLOG::_maskSprite = NULL;
bool			SLOG::Visible = false;
int				SLOG::_lastIndex = NUM_SLOG_LINES-1;
std::string		SLOG::_storedTxtArray[NUM_SLOG_LINES]= {"", "", "", "", "", "", "", "", "", "", "", ""};
int				SLOG::_logCount[NUM_SLOG_LINES]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int				SLOG::_logChangeIndex = -1;
long			SLOG::_logStartTime=0;
long			SLOG::_logEndTime=0;

SLOG::SLOG()
{
}

SLOG::~SLOG()
{
}

void SLOG::Create()
{
	if( _maskSprite != NULL )
		return;

	_maskSprite = new CObject();
	_maskSprite->LoadInit("NONGAME\\SUPERLOG\\MASK.SPR", 0, -384, 32000);
	
	float startX= 171;
	float startY=24, vertGap = 30.0f;
	for (int i = 0; i < NUM_SLOG_LINES; i++)
	{
		_sLogTexts[i] = new CBitmapText();
		ColorRGB gold ={(233/255.0f),(204/255.0f),(131/255.0f)};
		_sLogTexts[i]->SetTextProps(_storedTxtArray[i], startX, startY+i*vertGap, 32100, gold, eFont_25);
		if(i==_lastIndex)
			_sLogTexts[i]->SetText(":SuperLOG 1.0:/>");
	}

	Visible = true;

	_logChangeIndex = -1;

	Show(false);
}

void SLOG::Destroy()
{
	delete _maskSprite;
	for (int i = 0; i < NUM_SLOG_LINES; i++)
	{
		SAFE_DELETE(_sLogTexts[i]);
	}
}

void SLOG::Show(bool f)
{
	Visible = f;
	for (int i = 0; i < NUM_SLOG_LINES; i++)
	{
		_sLogTexts[i]->Show(f);
	}
	_maskSprite->Show(f);
}

///////// LOGGING FUNCTIONS //////////
void SLOG::DumpTextsToKLOG()
{
	for (int i = 0; i < NUM_SLOG_LINES; i++)
	{
		if(_sLogTexts[i]->Text != "")
			K_LOG(_sLogTexts[i]->Text.c_str());
	}
	SLOG::Log("Dumped to KLOG");
}

void SLOG::LogLoadingStartTime()
{
	_logStartTime = KMiscTools::getMilliseconds();
}

void SLOG::LogLoadingEndTime(std::string loadItemName)
{
	_logEndTime = KMiscTools::getMilliseconds();

	long elapsedLoadTime = _logEndTime - _logStartTime;
	SLOG::Log(std::string(loadItemName+" - Load Time:" + MKSTR(elapsedLoadTime) + " ms").c_str());
}

bool SLOG::checkPreviousLogsAndIncrementIndex(std::string txt)
{
	bool isCountChange = false;
	for (int i = _lastIndex; i >= 0 ; i--)
	{
		if(_storedTxtArray[i]==txt)
		{
			//equals increase count
			_logCount[i]++;
			_logChangeIndex = i;
			isCountChange = true;
		}
	}

	if (!isCountChange)
	{
		//Shift the previous logs up
		for (int i = 0; i < _lastIndex; i++)
		{
			_logCount[i]		= _logCount[i+1];
			_storedTxtArray[i]	= _storedTxtArray[i+1];
			_sLogTexts[i]->SetText(_sLogTexts[i+1]->Text);
		} 
	}

	return isCountChange;
}

void SLOG::Log(std::string txt, int val)
{
	for (int i = _lastIndex; i >= 0 ; i--)
	{
		if(_storedTxtArray[i]==txt)
		{
			//equals increase count
			_logCount[i]++;
			_sLogTexts[i]->SetText(std::string(txt + MKSTR(val) + " ["+ MKSTR(_logCount[i])+"]").c_str());
			
			return;
		}
	}

	//Shift the previous logs up
	for (int i = 0; i < _lastIndex; i++)
	{
		_logCount[i]		= _logCount[i+1];
		_storedTxtArray[i]	= _storedTxtArray[i+1];
		_sLogTexts[i]->SetText(_sLogTexts[i+1]->Text);
	}

	//set Last one as text
	_logCount[_lastIndex]=1;
	_storedTxtArray[_lastIndex]=txt;
	_sLogTexts[_lastIndex]->SetText(std::string(txt + MKSTR(val)).c_str());

	/*if(checkPreviousLogsAndIncrementIndex(txt))
	{
		_sLogTexts[_logChangeIndex]->SetText(std::string(txt + MKSTR(val) + " ["+ MKSTR(_logCount[_logChangeIndex])+"]").c_str());
	}
	else
	{
		_sLogTexts[_lastIndex]->SetText(std::string(txt + MKSTR(val)).c_str());
	}*/
}

void SLOG::Log(std::string txt, int* arrayPt, int numItems)
{
	std::string dispText = txt;

	for (int i = 0; i < numItems; i++)
	{
		dispText += (MKSTR(*(arrayPt+i)));
		if(i<(numItems-1))
			dispText += ",";
	}
	if(checkPreviousLogsAndIncrementIndex(txt))
	{
		_sLogTexts[_logChangeIndex]->SetText(dispText);
	}
	else
	{
		_sLogTexts[_lastIndex]->SetText(dispText);
	}
}

void SLOG::Log(std::string txt, std::vector<int> vecint, int numCols)
{
	std::string dispText = txt;
	int size = (int)vecint.size();  //Sac: converted Implicit to explicit
	for (int i = 0; i < size; i++)
	{
		dispText += (MKSTR(vecint[i]));
		if(numCols!=0)
		{
			if((i+1)%numCols == 0)
				dispText+= ";";
		}
		else
		{
			if(i<(size-1))
				dispText += ",";
		}
	}
	if(checkPreviousLogsAndIncrementIndex(txt))
	{
		_sLogTexts[_logChangeIndex]->SetText(dispText);
	}
	else
	{
		_sLogTexts[_lastIndex]->SetText(dispText);
	}
}

void SLOG::Log(std::string txt, CPoint* pt)
{
	for (int i = _lastIndex; i >= 0 ; i--)
	{
		if(_storedTxtArray[i]==txt)
		{
			//equals increase count
			_logCount[i]++;
			_sLogTexts[i]->SetText(std::string(txt + MKSTR(pt->x) + ","+MKSTR(pt->y) + " ["+ MKSTR(_logCount[i])+"]").c_str());
			
			return;
		}
	}

	//Shift the previous logs up
	for (int i = 0; i < _lastIndex; i++)
	{
		_logCount[i]		= _logCount[i+1];
		_storedTxtArray[i]	= _storedTxtArray[i+1];
		_sLogTexts[i]->SetText(_sLogTexts[i+1]->Text);
	}

	//set Last one as text
	_logCount[_lastIndex]=1;
	_storedTxtArray[_lastIndex]=txt;
	_sLogTexts[_lastIndex]->SetText(std::string(txt + MKSTR(pt->x) + ","+MKSTR(pt->y)).c_str());
}

void SLOG::Log(std::string txt, float val)
{
	for (int i = _lastIndex; i >= 0 ; i--)
	{
		if(_storedTxtArray[i]==txt)
		{
			//equals increase count
			_logCount[i]++;
			_sLogTexts[i]->SetText(std::string(txt + MKSTR(val) + " ["+ MKSTR(_logCount[i])+"]").c_str());
			
			return;
		}
	}

	//Shift the previous logs up
	for (int i = 0; i < _lastIndex; i++)
	{
		_logCount[i]		= _logCount[i+1];
		_storedTxtArray[i]	= _storedTxtArray[i+1];
		_sLogTexts[i]->SetText(_sLogTexts[i+1]->Text);
	}

	//set Last one as text
	_logCount[_lastIndex]=1;
	_storedTxtArray[_lastIndex]=txt;
	_sLogTexts[_lastIndex]->SetText(std::string(txt + MKSTR(val)).c_str());
}

void SLOG::Log(std::string txt)
{
	for (int i = _lastIndex; i >= 0 ; i--)
	{
		if(_storedTxtArray[i]==txt)
		{
			//equals increase count
			_logCount[i]++;
			_sLogTexts[i]->SetText(std::string(txt + " ["+ MKSTR(_logCount[i])+"]").c_str());
			
			return;
		}
	}

	//Shift the previous logs up
	for (int i = 0; i < _lastIndex; i++)
	{
		_logCount[i]		= _logCount[i+1];
		_storedTxtArray[i]	= _storedTxtArray[i+1];
		_sLogTexts[i]->SetText(_sLogTexts[i+1]->Text);
	}

	//set Last one as text
	_logCount[_lastIndex]=1;
	_storedTxtArray[_lastIndex]=txt;
	_sLogTexts[_lastIndex]->SetText(txt);
}

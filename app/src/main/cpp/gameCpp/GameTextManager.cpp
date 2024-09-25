//====================================
// Framework
// Cursor Class
//====================================

#include "GameTextManager.h"
#include "Resources.h"
#include "Console.h"
#include "DataEnums.h"
#include <string>


std::string langPrefix ="EN";

static GameTextManager* _instance = NULL;

GameTextManager::GameTextManager()
{
}

GameTextManager::~GameTextManager()
{
}


GameTextManager* GameTextManager::GetInstance()
{
	if(_instance==NULL)
	{
		_instance = new GameTextManager();
	}

	return _instance;
}

void GameTextManager::ClearLevelBanners()
{
	_bannerJSONDoc.Clear();
}

std::string GameTextManager::GetBannerText(const std::string keyString)
{
	std::string bannerText="";
	if(_bannerJSONDoc.HasMember(keyString.c_str()))
	{
		bannerText = _bannerJSONDoc[keyString.c_str()].GetString();
	}
	else
	{
		#ifdef _DEBUG
			#ifdef K_WIN32
				WARN((const char*)L"Er0x2201- Banner Text Not Found");
			#else
				WARN((const char*)"Er0x2201- Banner Text Not Found");
			#endif
		#endif
	}
	return bannerText;
}

void GameTextManager::LoadBannerForLevel()
{
	std::string jsonFileStr="BNR\\GAMETEXTV2.JSON";

#pragma region LOADING_FILE
	//Converts unsigned char_array to stream
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(jsonFileStr), lpCompressedData, nCompressedDataSize) )
	{
		#ifdef K_WIN32
			WARN((const char*)L"Er0x2202- JSON failed to open");
		#else
			WARN((const char*)"Er0x2202- JSON failed to open");
		#endif
		return;
	}
	/* Copy contents and add a terminal 0 */

	char *_lpfileBytes = new char [nCompressedDataSize+1];
	memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
	_lpfileBytes[nCompressedDataSize] = 0;
	std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray 
	delete [] _lpfileBytes;
	_lpfileBytes = NULL;
	/////////////////////////////////////////////////////////////////////

	if( stream.fail() )
	{
		#ifdef K_WIN32
			WARN((const char*)L"Er0x2203- Banner JSON failed to open");
		#else
			WARN((const char*)"Er0x2203- Banner JSON failed to open");
		#endif
		return;
	}  
#pragma endregion

	//converting stream data to string
	std::string jsonString = stream.str();

	const char* cstr = jsonString.c_str();
	
	//_jsonDocument.Clear();
	_bannerJSONDoc.Parse(cstr);
 
    /* Accessing values in document. */
	//if(_bannerJSONDoc.IsObject() )
	//{
	//	assert(_bannerJSONDoc.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

	//	//WARN((const char*)L"JSON parse success"); //Debug Purposes Only

	//	//Retrieving objects array
	//	assert(_bannerJSONDoc.HasMember("hello"));
	//	assert(_bannerJSONDoc["hello"].IsString());		
	//	std::string text = _bannerJSONDoc["hello"].GetString();

	////	K_LOG("Text loaded from JSON :%s", text.c_str());

	//	/*
	//	if( objDataArray[i].HasMember("pivotX") )
	//	{
	//		assert(objDataArray[i]["pivotX"].IsNumber());
	//		assert(objDataArray[i]["pivotX"].IsInt());
	//		pivotX = objDataArray[i]["pivotX"].GetInt();
	//	}
	//	*/
	//}
	//else
	//{
	//	#ifdef K_WIN32
	//		WARN((const char*)L"Banner JSON parse failed");
	//	#else
	//		WARN((const char*)"Banner JSON parse failed");
	//	#endif
	//}
}

void GameTextManager::SetMyLangauge(int lang)
		{
			myLang=lang;
			switch (lang)
			{
				case LN_ENGLISH:
					langPrefix ="EN";
					break;
				case LN_GERMAN:
					langPrefix ="DE";
					break;
				//case LN_SPANISH:
					//langPrefix ="ES";
					//break;
				case LN_FRENCH:
					langPrefix ="FR";
					break;
				case LN_ITALIAN:
					langPrefix ="IT";
					break;
				case LN_NEDERLANDS:
					langPrefix ="ND";
					break;
				case LN_HUNGARIAN:
					langPrefix ="HU";
					break;
				case LN_PORTUGUESE:
					langPrefix ="PR";
					break;
				case LN_BRAZILIAN_PORTUGUESE:
					langPrefix ="BR";
					break;
				case LN_JAPANESE:
					langPrefix ="JN";
					break;
				case LN_SIMPLIFIED_CHINESE:
					langPrefix ="CS";
					break;
				case LN_KOREAN:
					langPrefix ="KR";
					break;
				case LN_TRADITIONAL_CHINESE:
					langPrefix ="CT";
					break;
				case LN_RUSSIAN:
					langPrefix ="RU";
					break;
				case LN_UKRAINIAN:
					langPrefix ="UR";
					break;
/*				case HINDI:
					langPrefix ="HI";
					break;
				case ARABIC:
					langPrefix ="AB";
					break;   
*/
				default:
					langPrefix ="EN";
					break;
			}

		}

std::string GameTextManager::GetMyLangauge()
		{
				return	langPrefix ="EN";
		}

int GameTextManager::GetMyLangaugeID()
		{
				return	myLang;
		}
void GameTextManager::LoadAllGameTexts()
{

	std::string jsonFileStr="BNR\\GAMETEXTV2_"+langPrefix+".JSON";
	//Converts unsigned char_array to stream
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(jsonFileStr), lpCompressedData, nCompressedDataSize) )
	{
		#ifdef K_WIN32
			WARN((const char*)L"Er0x2204- Game text JSON failed to open");
		#else
			WARN((const char*)"Er0x2204- Game text JSON failed to open");
		#endif
		return;
	}
	/* Copy contents and add a terminal 0 */

	char *_lpfileBytes = new char [nCompressedDataSize+1];
	memcpy (_lpfileBytes, lpCompressedData, nCompressedDataSize);//copy unsigned_chararray to const_chararray
	_lpfileBytes[nCompressedDataSize] = 0;
	std::stringstream stream(_lpfileBytes);//create stream from "const"_chararray 
	delete [] _lpfileBytes;
	_lpfileBytes = NULL;
	/////////////////////////////////////////////////////////////////////

	if( stream.fail() )
	{
		#ifdef K_WIN32
			WARN((const char*)L"Er0x2205- Banner JSON failed to open");
		#else
			WARN((const char*)"Er0x2205- Banner JSON failed to open");
		#endif
		return;
	}  
	switch(myLang)
	{
		case LN_JAPANESE:
		case LN_SIMPLIFIED_CHINESE:
		case LN_TRADITIONAL_CHINESE:
		case LN_RUSSIAN:
		case LN_UKRAINIAN:
//		case HINDI:
//		case ARABIC:
		case LN_KOREAN:
		case LN_ENGLISH:
		case LN_GERMAN:
		//case LN_SPANISH:
		case LN_FRENCH:
		case LN_ITALIAN:
		case LN_NEDERLANDS:
		case LN_HUNGARIAN:
		case LN_PORTUGUESE:
		case LN_BRAZILIAN_PORTUGUESE:
		default:
			std::string jsonString = stream.str();
			const char* cstr = jsonString.c_str();
			_bannerJSONDoc.Parse(cstr);
			break;	
	}
	/*
	//converting stream data to string
	std::string jsonString = stream.str();

	const char* cstr = jsonString.c_str();
	_bannerJSONDoc.Parse(cstr);
	*/
}

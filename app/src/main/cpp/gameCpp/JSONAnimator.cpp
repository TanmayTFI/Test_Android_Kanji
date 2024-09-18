//===========================================================
// 
// Framework
// Handles a group of unit animator objects, initialized with a single json file,
// updates all children unit animator objects also
// Author : Faisal
// JSON impelmentation : SAJITH
// JSONAnimator
//===========================================================

#include "JSONAnimator.h"
#include "Resources.h"
#include "Control.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <rapidjson/filereadstream.h>

using namespace rapidjson;

JSONAnimator::JSONAnimator(int animID, std::function<void (int)> onCompleteCallback)
{
	_isSFXLoaded = false;
	_sfxKey = "";
	Filename= "";
	_isPlaying = false;
	_animTime = 0;
	_totalAnimTime = 0;

	_isLooping = false;
	_paused = false;

	_animID = animID;

	_onCompleteCallback = onCompleteCallback;
}

JSONAnimator::~JSONAnimator()
{
	_onCompleteCallback = nullptr;
	int size =(int)_unitAnimArray.size();
	for(int i=0; i < size; ++i)
	{
		delete _unitAnimArray[i];
		_unitAnimArray[i] = NULL;
	}
	_unitAnimArray.clear();
}

void JSONAnimator::PlayAnim(bool IsLoop)
{
	SetEnableObjects(true);
	if(_totalAnimTime>0)
	{
        int size = (int)_unitAnimArray.size();  //Sac: converted Implicit to explicit

		for(int i=0; i < size; ++i)
		{
			_unitAnimArray[i]->ResetAnim();
		}

		_animTime = 0;
		_isPlaying = true;

		if(_isSFXLoaded)
			Control::Audio->QuickLoadAndPlaySFX(_sfxKey);

		_isLooping = IsLoop;
	}
}

void JSONAnimator::StopAnim()
{
	_isPlaying = false;
	
    int size = (int)_unitAnimArray.size();  //Sac: converted Implicit to explicit

	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->ResetAnim();
	}

	_animTime = 0;
}

void JSONAnimator::Update(float ds)
{
	if( _paused )
		return;

	if( _isPlaying )
	{
		float msTime = ds*1000;
		int size =(int)_unitAnimArray.size();
		for(int i=0; i < size; ++i)
		{
			_unitAnimArray[i]->Update(msTime);
		}

		_animTime += msTime;
		if(_animTime>_totalAnimTime)
		{
			if(_isLooping)
			{
				for(int i=0; i < size; ++i)
				{
					_unitAnimArray[i]->ResetAnim();
				}
			}
			else
			{
				//force set anim to final key state
				for(int i=0; i < size; ++i)
				{
					_unitAnimArray[i]->Update(msTime);
				}

				_isPlaying = false;
				if (_onCompleteCallback != nullptr)
				{
					_onCompleteCallback(_animID);
				}
			}
			_animTime = 0;
		}
	}
}

void JSONAnimator::SetEnableObjects(bool f)
{
	int size =(int)_unitAnimArray.size();
	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->setEnableObject(f);
	}
}

void JSONAnimator::SetAlphaForObjects(float alpha)
{
	int size =(int)_unitAnimArray.size();
	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->SetAlphaForObjects(alpha);
	}
}


void JSONAnimator::FadeOutObjects(float speed)
{
	int size =(int)_unitAnimArray.size();
	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->FadeOutObject(speed);
	}
}


void JSONAnimator::SetAnimTime(float animTime)
{
	animTime *= 1000;
	if (animTime > _totalAnimTime)
	{
		animTime = std::fmod(animTime, _totalAnimTime);
	}
	int size =(int)_unitAnimArray.size();
	float latestTime = 0;
	for(int i=0; i < size; ++i)
	{
		float time = _unitAnimArray[i]->GetAnimKey(animTime)->keytime;
		if (time > latestTime)
		{
			latestTime = time;
		}
	}
	_animTime = latestTime;
	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->SetAnimTime(_animTime);
	}
}


void JSONAnimator::SetPosOffset(int x, int y )
{
	int size =(int)_unitAnimArray.size();
	for(int i=0; i < size; ++i)
	{
		_unitAnimArray[i]->SetPosOffset(x,y);
	}
}

bool JSONAnimator::IsAnimFinished()
{
	if(!_isLooping)
	{
		if( _animTime > _totalAnimTime)
			return true;
	}
	return false;
}

bool JSONAnimator::IsAnimPlaying()
{
	return _isPlaying;
}

void JSONAnimator::parseAndLoadJSON(const std::string& jsonFileStr, CHOScene *hoScene, std::vector<SuperFX* >* superfxArray)
{

#pragma region LOADING_FILE

	Filename = jsonFileStr;
	//Filename to uppercase!!
    int strLen = (int) Filename.size();  //Sac: converted Implicit to explicit

	for( int i = 0; i < strLen; i++ )
		Filename[i] = toupper(Filename[i]);

	//Converts unsigned char_array to stream
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(Filename), lpCompressedData, nCompressedDataSize) )
	{
		#ifdef K_WIN32
			WARN("Er0x1101- JSON failed to open");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1101- JSON failed to open");
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
			WARN("Er0x1102- JSON failed to open");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1102- JSON failed to open");
		#endif
		//WARN(std::string("JSON Animation file failed to open: " + FileSystem::strPath(jsonFileStr)).c_str() );
		return;
	}  
#pragma endregion

	//converting stream data to string
	std::string jsonString = stream.str();

	const char* cstr = jsonString.c_str();
	
	Document document;  
	document.Parse( cstr );
 
    /* Accessing values in document. */
	if(document.IsObject() )
	{
		assert(document.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

		//Retrieving objects array
		assert(document.HasMember("ObjectData"));
		const Value& objDataArray = document["ObjectData"];
		assert(objDataArray.IsArray());

		if(document.HasMember("sfxID"))
		{
			_isSFXLoaded = true;
			_sfxKey = document["sfxID"].GetString();
		}

		SizeType numObjects = objDataArray.Size();
		//SLOG::LogInt("JSON Num objs:",numObjects);

		int numParticles = 0;
		if(superfxArray!=NULL)
            numParticles =(int) superfxArray->size();  //Sac: converted Implicit to explicit


		for (SizeType i = 0; i < numObjects; i++)
		{
			std::string objID = objDataArray[i]["ObjID"].GetString(); //get UID of object

			UnitAnimator *unitAnimator = NULL;

			//This is a SuperFX.
			SuperFX* fx=NULL;
			if( numParticles>0 && objDataArray[i].HasMember("isSuperfx") )
			{
				for (int i = 0; i < numParticles; i++)
				{
					if(objID == superfxArray->at(i)->UID)
					{
						fx = superfxArray->at(i);
						break;
					}
				}

				unitAnimator = new UnitAnimator(fx);
			}
			else
			{
				//Unit Animator for each object
				unitAnimator = new UnitAnimator(hoScene->GetObjectByID(objID));
			}

			int pivotX=0, pivotY=0;

			if( objDataArray[i].HasMember("pivotX") )
			{
				assert(objDataArray[i]["pivotX"].IsNumber());
				assert(objDataArray[i]["pivotX"].IsInt());
				pivotX = objDataArray[i]["pivotX"].GetInt();
			}

			if( objDataArray[i].HasMember("pivotY") )
			{
				assert(objDataArray[i]["pivotY"].IsNumber());
				assert(objDataArray[i]["pivotY"].IsInt());
				pivotY = objDataArray[i]["pivotY"].GetInt();
			}

			unitAnimator->GetAnimObject()->SetPivot((float)pivotX, (float)pivotY);

			const Value& animDataArray = objDataArray[i]["AnimData"];
			assert(animDataArray.IsArray());

			SizeType numKeys = animDataArray.Size();
			//SLOG::LogInt("OBJ Num keys:", numKeys);
			std::vector<UNITKEY *>	AnimKeys;

			float totalObjAnimTime = 0;
			for (SizeType i = 0; i < numKeys; i++)
			{
				UNITKEY *key = new UNITKEY();
				key->keyId = i;
				// Number is a JSON type, but C++ needs more specific type.

				assert(animDataArray[i]["Keytime"].IsNumber());
				assert(animDataArray[i]["Keytime"].IsInt());
				key->keytime = (float)animDataArray[i]["Keytime"].GetInt();

				if(i==numKeys-1)
				{
					totalObjAnimTime = (float)key->keytime; //keys are linear. End key denotes total anim time

					if( totalObjAnimTime > _totalAnimTime )
						_totalAnimTime = totalObjAnimTime;
				}

				if( animDataArray[i].HasMember("alpha"))
				{
					assert(animDataArray[i]["alpha"].IsNumber());
					assert(animDataArray[i]["alpha"].IsDouble());
					key->alpha = (float)animDataArray[i]["alpha"].GetDouble();
				}

				assert(animDataArray[i]["xPos"].IsNumber());
				assert(animDataArray[i]["xPos"].IsInt());
				key->xpos = animDataArray[i]["xPos"].GetInt();

				assert(animDataArray[i]["yPos"].IsNumber());
				assert(animDataArray[i]["yPos"].IsInt());
				key->ypos = animDataArray[i]["yPos"].GetInt();

				if( animDataArray[i].HasMember("rotation"))
				{
					assert(animDataArray[i]["rotation"].IsNumber());
					assert(animDataArray[i]["rotation"].IsDouble());
					key->rot = (float)animDataArray[i]["rotation"].GetDouble();
				}

				if( animDataArray[i].HasMember("scale"))
				{
					assert(animDataArray[i]["scale"].IsNumber());
					assert(animDataArray[i]["scale"].IsDouble());
					key->scale = (float)animDataArray[i]["scale"].GetDouble();
				}

				if( animDataArray[i].HasMember("easeType"))
				{
					assert(animDataArray[i]["easeType"].IsNumber());
					assert(animDataArray[i]["easeType"].IsInt());
					key->easeType = animDataArray[i]["easeType"].GetInt();
				}

				AnimKeys.push_back(key);
			}

			unitAnimator->SetAnimKeys(AnimKeys, totalObjAnimTime);
			_unitAnimArray.push_back(unitAnimator);
		}
	}
	else
	{
		#ifdef K_WIN32
			WARN("Er0x1103- JSON parse failed");////Removed Wide string conversion for Windows	
		#else
			WARN((const char*)"Er0x1103- JSON parse failed");
		#endif
	}
}

//===========================================================
// 
// Framework
//===========================================================

#include "Scene2D.h"
#include "IOStream.h"
#include "Resources.h"
#include "Background.h"
#include "HOObject.h"
#include "Camera.h"
#include "Console.h"
#include "LangDefs.h"

CScene2D::CScene2D()
{
	SWidth = 0;
	SHeight = 0; 
}


CScene2D::~CScene2D()
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		delete Objects[i];
		Objects[i] = NULL;
	}
	Objects.clear();	
}

void CScene2D::SetCam(const Camera* cam)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i]->SetCam(cam);
	}
}

void CScene2D::Show(bool f)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
		Objects[i]->Show(f);	
}

void CScene2D::RenderToTarget(float offX, float offY)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
		if(Objects[i]->IsActive)
		{
			Objects[i]->Render(PointSystem::CreateCPoint(offX,offY,0));//Force render	
		}
}

void CScene2D::SetMaskRect(CRectangle maskrect)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
		if(Objects[i]->IsActive)
			Objects[i]->SetMaskRect(maskrect);
}

void CScene2D::Load(const std::string& Name)
{
	/*
	sceneFilename = Name;

	//Filename to uppercase!!
	int strLen = sceneFilename.size();
	for( int i = 0; i < strLen; i++ )
		sceneFilename[i] = toupper(sceneFilename[i]);
	*/
	/////////////////////////////////////////////////////////////////////
	//  		    Converts unsigned char_array to stream             //
	/////////////////////////////////////////////////////////////////////
	unsigned char *lpCompressedData = NULL;
	size_t nCompressedDataSize = 0;
	if(! KResource::loadResource (FileSystem::charPath(Name), lpCompressedData, nCompressedDataSize) )
	{
		std::string TmpStr = std::string("Er0x2001- Scene failed to open: " + FileSystem::strPath(Name)).c_str();	
		#ifdef K_WIN32
			WARN((TmpStr).c_str());//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
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
		std::string TmpStr = std::string("Er0x2002- Failed to load scene: " + FileSystem::strPath(Name)).c_str();	
		#ifdef K_WIN32
			WARN((TmpStr).c_str()) ;//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
		#else
			WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
		#endif	
		return;
	}
	
	//Discard Mode [Tool side only]
	std::string tmpStr;
	stream >> tmpStr;//string	

	//Read version
	int version;
	stream >> version;//int

	if( version == 1 )
	{
		//Establish Camera Data		
		stream >> SWidth;//int
		stream >> SHeight;//int
		
		if( version == 1 )//Discard two lines of HOScene for now [temp funcionality]
		{			
			stream >> tmpStr;
			stream >> tmpStr;
		}
		
		//Read Objects
		int objCount;
		stream >> objCount;//int

		for( int i=0 ; i < objCount; ++i )
		{
			if( version == 1 )
			{
				SceneData data;
				stream >> data.objType;
				stream >> data.uid;
				stream >> data.spriteString;
				stream >> data.spriteAnim;
				stream >> data.x;
				stream >> data.y;
				stream >> data.ZOrder;

				if( data.objType == BACKGROUND )
				{
					stream >> data.texName;
				}
				else if(data.objType == HIDDEN_OBJECT )
				{
					stream >> data.HOName;
					stream >> data.ScaleFactor;//skip					
					stream >> data.InteractiveObj;//skip
				}

				CObject *pObj = CreateObject(data);
				if(pObj)
					Objects.push_back(pObj);
			}
		}
	}
}

CObject* CScene2D::CreateObject(SceneData data)
{
	CObject *pObj = NULL;	

	pObj = pCreateObject(data.objType);//creates new Object!!
	pObj->UID = data.uid;

	if( data.spriteString != "NULL")
	{
		pObj->LoadSprite(data.spriteString);
		pObj->PlayAnimation(data.spriteAnim);
	}
	pObj->Pos.x = pObj->InitPos.x = float(data.x);
	pObj->Pos.y = pObj->InitPos.y = float(data.y);
	
	pObj->ZOrder = data.ZOrder;

	if( data.objType == BACKGROUND )
	{
		pObj->Load(data.texName);					
	}
	else if( data.objType == HIDDEN_OBJECT )
	{
#ifdef JAPANESE_BUILD 
		if( Name == "L5\\HT\\HOTTUB.SCN" || Name == "L6\\PTB\\PTB.SCN"  )
		{
			std::string::size_type index = 0, size = data.HOName.size(); 
			for( ; index < size ; index++ )
				if( data.HOName[index] == '_' )
					data.HOName[index] = ' ';
		}
		else //For japanese GM_HO scenes build load HO name from 
			HOName = std::string(StringTable::getStr(data.HOName.c_str()));
#else
		std::string::size_type index = 0, size = data.HOName.size(); 
		for( ; index < size ; index++ )
			if( data.HOName[index] == '_' )
				data.HOName[index] = ' ';
#endif

		pObj->Load(data.HOName,(data.InteractiveObj == 999));//					
	}
	
	return pObj;
}


CObject* CScene2D::pCreateObject(const std::string& objType)
{
	CObject *pObj = NULL;

	if( objType == OBJECT )
	{
		pObj = new CObject();
	}
	else if( objType == BACKGROUND )
	{
		pObj = new CBackground();
	}
	else if( objType == HIDDEN_OBJECT )
	{
		pObj = new CHOObject(VFX_FOUNDHO);
	}
	else
	{
		KPTK::logMessage(objType.c_str());
		#ifdef K_WIN32
			FATAL("Er0x2003- Unknown object type parsed in Scene2D.");////Removed Wide string conversion for Windows	
		#else
			FATAL((const char*)"Er0x2003- Unknown object type parsed in Scene2D.");
		#endif		
		
	}

	return pObj;
}

void CScene2D::SetSceneAlpha(float alpha)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i]->SetAlpha(alpha);
	}
}

void CScene2D::Offset(const CPoint& off)
{
	CPoint temp;

	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		temp = Objects[i]->Pos;
		temp.x += off.x;
		temp.y += off.y;
		Objects[i]->SetPos(&temp);
	}
}


void CScene2D::RestoreXY()
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i]->Pos.x = Objects[i]->InitPos.x;
		Objects[i]->Pos.y = Objects[i]->InitPos.y;		
	}
}

void CScene2D::SaveXY()
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i]->InitPos.x = Objects[i]->Pos.x;
		Objects[i]->InitPos.y = Objects[i]->Pos.y;		
	}
}

void CScene2D::ShiftXY( float x, float y)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		Objects[i]->Pos.x = Objects[i]->InitPos.x+x;
		Objects[i]->Pos.y = Objects[i]->InitPos.y+y;		
	}
}

CObject* CScene2D::GetObjectByID(const std::string& id)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		if( Objects[i]->UID == id )
		{
			return Objects[i];
		}
	}

#ifdef _DEBUG
	K_LOG("GetObjectByID NULL - Please check the ID name/case of -%s",id.c_str());
#endif // _DEBUG

	return NULL;
}

CObject* CScene2D::pCollideWithObject(const CPoint* pMouse,  bool AllowBGs)
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		if( Objects[i]->Visible &&
			Objects[i]->Collide(pMouse) &&
			(AllowBGs || Objects[i]->Type != BACKGROUND))
		{
			return Objects[i];
		}
	}

	return NULL;
}

void CScene2D::DeleateAllObjects()
{
	int size = (int)Objects.size();
	for(int i=0; i < size; ++i)
	{
		if (Objects[i] != NULL)
		{
			delete Objects[i];
		}
		Objects[i] = NULL;
	}
	Objects.clear();
}


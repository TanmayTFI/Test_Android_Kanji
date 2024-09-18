//===========================================================
// 
// Framework
//===========================================================

#include "TextureFactory.h"
#include "Util.h"
#ifdef K_WIN32
	#include "winnt.h"
#endif
using namespace protected_TextureFactory;

CTexFactory* CTexFactory::Instance = NULL;

	

void CTexFactory::Create()
{
	if(CTexFactory::Instance == NULL)
		CTexFactory::Instance = new CTexFactory();
}

void CTexFactory::Destroy()
{
	if(CTexFactory::Instance != NULL)
	{
		delete CTexFactory::Instance;
		CTexFactory::Instance = NULL;
	}
}

CTexFactory::CTexFactory()
{
	TotalTexSize = 4194304 * 2;//bytes to handle front-back buffer
}

CTexFactory::~CTexFactory()
{
	Purge();
}

void CTexFactory::Purge()
{
	for( std::vector<CTexture *>::iterator it = Textures.begin(); it != Textures.end(); ++it )
	{
		delete (*it);
		(*it) = NULL;
	}

	Textures.clear();
}


//Should only be called to cache image pointers
KGraphic* CTexFactory::GetImage( std::string Name, bool IsAlphaMask, bool forceHiQuality  )
{
	
	//Check if .png replaced by .jpg+_.jpg 
	if( !IsAlphaMask && ((CUtil::GetFileExtension(Name) == "jpa")||(CUtil::GetFileExtension(Name) == "JPA")) )
	{
		Name = CUtil::StripFileExtension(Name);
		IsAlphaMask = true;
	}

	
	
	
	int strLen = (int) Name.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Name[i] = toupper(Name[i]);
	
	
	if( Textures.size() )//atleast a single element present
	{
		int NameLen = (int) Name.size();  //Sac: converted Implicit to explicit
		std::vector<CTexture *>::iterator it = Textures.end();	
		//start iterating from last pushed texture	
		do
		{
			--it;
			if( (*it)->NameLen == NameLen )
				if( CUtil::IsEqual( (*it)->Name, Name, NameLen) )
				//if( (*it)->Name == Name )
				{
					(*it)->RefCount++;
					return (*it)->Image;
				}	
		}
		while( it != Textures.begin() );
		//no texture found!!
	}
	

	//The Texture has not been instanced yet, so create it.
	CTexture *Tex = new CTexture();
	
	
	if( !Tex->Load( Name, IsAlphaMask, forceHiQuality ) )
	{
		#ifdef K_WIN32
			KMiscTools::messageBox( (const char*)L"Loading Error", (const char*)std::wstring(L"Er0x5001- Unable to load Image: " + FileSystem::s2ws(Name)).c_str()); 
		#else
			KMiscTools::messageBox( (const char*)"Loading Error", (const char*)std::string("Er0x5001- Unable to load Image: " + Name).c_str()); 	
		#endif
		//KMiscTools::messageBox( (const char*)L"Error", (const char*)((std::string("Er0x5001- Unable to load Image: ") + Name).c_str()) );
		exit(0);
		//delete Tex;
		//return NULL;
	}
	else
		Textures.push_back(Tex);

	int w =  (int)Tex->Image->getWidth();
	int h = (int)Tex->Image->getHeight();
	int Size = w * h  * 4;
	TotalTexSize += Size;

	return Tex->Image;
}


void CTexFactory::LoadImg( std::string Name, bool IsAlphaMask, bool SkipLibSearch, bool forceHiQuality )
{
	
	//Check if .png replaced by .jpg+_.jpg 
	if( !IsAlphaMask && ((CUtil::GetFileExtension(Name) == "jpa")||(CUtil::GetFileExtension(Name) == "JPA")) )
	{
		Name = CUtil::StripFileExtension(Name);
		IsAlphaMask = true;
	}
	 

	int strLen = (int)Name.size();  //Sac: converted Implicit to explicit
	for( int i = 0; i < strLen; i++ )
		Name[i] = toupper(Name[i]);
	

	if( !SkipLibSearch )
		if( Textures.size() )//atleast a single element present
		{
			int NameLen = (int)Name.size(); //Sac: converted Implicit to explicit
			std::vector<CTexture *>::iterator it = Textures.end();	
			//start iterating from last pushed texture	
			do
			{
				--it;
				if( (*it)->NameLen == NameLen )
					if( CUtil::IsEqual( (*it)->Name, Name, NameLen) )
					//if( (*it)->Name == Name )
					{
						(*it)->RefCount++;
						return;
					}	
			}
			while( it != Textures.begin() );
			//no texture found!!
		}
	 
	
	//The Texture has not been instanced yet, so create it.
	CTexture *Tex = new CTexture();	
	//K_LOG( Name.c_str() );
	
	if( !Tex->Load( Name, IsAlphaMask, forceHiQuality  ) )
	{
		//std::wstring ErrMsg = L"Unable to load Image: " + FileSystem::s2ws(Name);
		//KMiscTools::messageBox( (const char*)L"Loading Error", (const char*)(ErrMsg.c_str()) );
		#ifdef K_WIN32
			KMiscTools::messageBox( (const char*)L"Loading Error", (const char*)std::wstring(L"Er0x5002- Unable to load Image: " + FileSystem::s2ws(Name)).c_str()); 
		#else
			KMiscTools::messageBox( (const char*)"Loading Error", (const char*)std::string("Er0x5002- Unable to load Image: " + Name).c_str()); 	
		#endif
		exit(0);
		//delete Tex;
		//return;
	}
	else	
		Textures.push_back(Tex);
	
	int w =  (int)Tex->Image->getWidth();
	int h = (int)Tex->Image->getHeight();
	int Size = w * h  * 4;
	TotalTexSize += Size;
}


void CTexFactory::FreeLevelImages( int Level )
{	
	if( Textures.size() )//atleast a single element present
	{
		
		//start iterating......
		for( std::vector<CTexture *>::iterator it = Textures.begin(); it != Textures.end();  )
		{
			//K_LOG((*it)->Name.c_str());
			if(	(*it)->Name[0] == 'L' && (*it)->Name[1] == 'V' &&
				(*it)->Name[2] == char(49+Level) )
			{
				TotalTexSize -= (int)((*it)->Image->getWidth() * (*it)->Image->getHeight() * 4);				
				delete (*it);
				(*it) = NULL;				
				Textures.erase(it);
				it = Textures.begin();
			}
			else
				++it;
		}		
		//no texture found!!
	}
	//If we get here then the Factory didn't create the instance...
}

void CTexFactory::FreeImage( std::string Name )
{
	if( Name == "" )
		return;
	if( (CUtil::GetFileExtension(Name) == "jpa")||(CUtil::GetFileExtension(Name) == "JPA")  )
	{
		Name = CUtil::StripFileExtension(Name);
	}

	int strLen = (int)Name.size();//Make it uppercase!!
	for( int i = 0; i < strLen; i++ )
		Name[i] = toupper(Name[i]);

	//for( std::vector<CTexture *>::iterator it = Textures.begin(); it != Textures.end(); ++it )
	if( Textures.size() )//atleast a single element present
	{
		int NameLen = (int)Name.size();
		std::vector<CTexture *>::iterator it = Textures.end();	
		//start iterating from last pushed texture	
		do
		{
			--it;
			if( (*it)->NameLen == NameLen )
				if( CUtil::IsEqual( (*it)->Name, Name, NameLen) )
				//if( (*it)->Name == Name )
				{
					(*it)->RefCount--;

					if( (*it)->RefCount <= 0 )
					{
						TotalTexSize -= (int)((*it)->Image->getWidth() * (*it)->Image->getHeight() * 4);
						delete (*it);
						(*it) = NULL;
						Textures.erase(it);
					}					
					return;
				}
		}
		while( it != Textures.begin() );
		//no texture found!!
	}

	//If we get here then the Factory didn't create the instance...
}

void CTexFactory::FreeImage( KGraphic** Image )
{
	if( (*Image) == NULL )
	{
		return;
	}
	//for( std::vector<CTexture *>::iterator it = Textures.begin(); it != Textures.end(); ++it )
	if( Textures.size() )//atleast a single element present
	{
		std::vector<CTexture *>::iterator it = Textures.end();	
		//start iterating from last pushed texture	
		do
		{
			--it;
			if( (*it)->Image == (*Image) )
			{
				(*it)->RefCount--;

				if( (*it)->RefCount <= 0 )
				{
					TotalTexSize -= (int)((*it)->Image->getWidth() * (*it)->Image->getHeight() * 4);
					delete (*it);
					(*it) = NULL;
					Textures.erase(it);
				}

				(*Image) = NULL;

				return;
			}
		}while( it != Textures.begin() );
		//no texture found!!
	}

	//If we get here then the Factory didn't create the instance...
}

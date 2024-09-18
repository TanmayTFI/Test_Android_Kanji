//===========================================================
// 
// Framework
//===========================================================

#include "Texture.h"
#include "GameFlow.h"
#include "Util.h"
#include "string.h"
#include "ProfileSystem.h"

namespace protected_TextureFactory
{
	CTexture::CTexture()
	:	Name("none"),
		RefCount(0),
		Image(NULL)
	{
		NameLen = 0;
	}

	CTexture::~CTexture()
	{
		if(Image != NULL)
		{
			Image->freePicture();
			delete Image;
		}

		Image = NULL;
	}

	
	bool CTexture::Load(std::string File, bool IsAlphaMask, bool forceHiQuality )
	{
		if( Image )
			return false;
		
		
		Name = File;
		
		RefCount = 1;

		Image = KPTK::createKGraphic();	
		
        bool bHiQuality = true;
        
        if( !forceHiQuality )// forced to hiquality to avoid checking each tex file strcomparison here!!
        {
            if( GFInstance->IsForceTextures16Bit )
            {
                bHiQuality = false;
                Image->setTextureColorPriority(false);
            }
        }
        
		bHiQuality = true;//24 bit quality for all elements!!
		if( IsAlphaMask  )
		{
			std::string imageFile = FileSystem::strPath(File+".JPG");							
			std::string maskFile = FileSystem::strPath(File+"_.JPG");	
           
			//try loading opaque image with alpha mask - truecolor(hidef) load
			if( !Image->loadPictureWithMask( imageFile.c_str(),maskFile.c_str(), bHiQuality,true ) )//load hiquality - truecolor 24 bit
			{
				//try loading opaque image- truecolor(hidef) load
				if( !Image->loadPicture( imageFile.c_str(), bHiQuality, false, true ) )//load hiquality - truecolor 24 bit
				{
					delete Image;
					Image = NULL;

					return false;
				}				
			}			
		}
		else
		{
			bool hasAlpha = (CUtil::GetFileExtension(File) == "PNG");
            
			std::string imageFile = FileSystem::strPath(File);
			//Load alpha & nonalpha images!!- truecolor(hidef) load
			if( !Image->loadPicture( imageFile.c_str(), bHiQuality, hasAlpha,true ) )//load hiquality - truecolor 24 bit
			{
				delete Image;
				Image = NULL;

				return false;
			}
			
		}		
		NameLen = (int)Name.size();  //Sac: converted Implicit to explicit
		Image->setTextureQuality(true);
		Image->allowTextureWrap(K_WRAP_MIRROR);//CLAMP
		return true;		
	}
}


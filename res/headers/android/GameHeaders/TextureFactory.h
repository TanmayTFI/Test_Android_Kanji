//===========================================================
// 
// Framework
//===========================================================

#ifndef TEXTUREFACTORY_H
#define TEXTUREFACTORY_H

#include "Texture.h"

class CTexFactory
{
	private:
		CTexFactory();
		~CTexFactory();	

		std::vector< protected_TextureFactory::CTexture* > Textures;	

	public:
		int TotalTexSize;
		static CTexFactory* Instance;
		static void				Create();
		static void				Destroy();

		
		void Purge();

		KGraphic* GetImage( std::string Name, bool IsAlphaMask = false, bool forceHiQuality = false  );
        void LoadImg( std::string Name, bool IsAlphaMask = false, bool SkipLibSearch = false, bool forceHiQuality = false );
		void FreeImage( KGraphic** Image );
		void FreeImage( std::string Name );
		void FreeLevelImages( int Level );

	};

#endif
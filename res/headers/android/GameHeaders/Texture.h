//===========================================================
// 
// Framework
//===========================================================

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resources.h"

namespace protected_TextureFactory
{
	class CTexture
	{
		public:
			std::string Name;
			int NameLen;
			int RefCount;			
			KGraphic *Image;

			CTexture();
			~CTexture();

			bool Load(std::string File, bool IsAlphaMask = false, bool forceHiQuality = false);
	};
}

#endif
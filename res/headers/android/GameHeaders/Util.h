//===========================================================
// 
// Framework
//===========================================================

#ifndef UTIL_H
#define UTIL_H

#include <math.h>
#include <string>
#include <sstream>

#define MKSTR(x)	CUtil::ToString(x)
#define MKFLT(x)	CUtil::ToFloat(x)
#define MKINT(x)	CUtil::ToInt(x)

static const float		 Epsilon = 0.0000001f;
static float HintParticleTimerValue = 8.0f;
static float HintParticleEmitTimerValue = 32.0f;


/** Convert string to uppercase */
#define KStringToUpper(__buffer) do { size_t n = 0; while (__buffer[n]) { __buffer[n] = toupper (__buffer[n]); n++; } } while (0)

class CUtil
{
	public:
		//Fast Modulus
		static int Mod( int num, int div )
		{
			return (num%div); //changed back for now
		}

		//Float equality
		static bool Equal( float a, float b )
		{
			return ( (a >= b - Epsilon) && (a <= b + Epsilon) );
		}

		//Returns the directory of a file - used in 'CSprite' load function
		static std::string StripFileName(const std::string& File)
		{
			std::string StrippedFile = File;

			int i = (int)StrippedFile.size()-1;

			//Delete till we find a '\'
			while( StrippedFile[i] != '\\' && StrippedFile[i] != '/' ) 
			{
				StrippedFile.erase(i);
				i--;

				if(StrippedFile.size() <= 0 )
				{
					return "";//Strip File Name Failed
					//Bad stuff happened
				}
			}
			return StrippedFile;
		}

		//Returns the directory of a file
		static bool IsEqual(const std::string& Str1, const std::string& Str2, int StrLen)
		{
			for( int i= (StrLen-1); i >= 0; i-- )
			{
				if( Str1[i] != Str2[i] )
					return false;				
			}

			return true;
		}

		//Returns the file extension 
		static std::string GetFileExtension(const std::string& File)
		{
			std::string FileExt = "nil";
			int i = (int)File.size()-1;
			if( i > 2 )//copy extension							
			{
				char Ext[4];
				Ext[0] = (char)File[i-2];
				Ext[1] = (char)File[i-1];
				Ext[2] = (char)File[i];
				Ext[3] = '\0';
				FileExt = Ext;
			}
			return FileExt;
		}
		

		//Returns the full file path without extension
		static std::string StripFileExtension(const std::string& File)
		{
			std::string StrippedFile = File;

			int i = (int)StrippedFile.size()-1;
			if( i )
			{
				bool Strip = true;
				while( Strip )
				{
					if( StrippedFile[i] == '.' )//strip . too
						Strip = false;
					
				
					StrippedFile.erase(i);
					i--;
				
					if(StrippedFile.size() <= 0 )
					{
						return "NULL";
						//Bad stuff happened
					}
				}
			}

			return StrippedFile;
		}
		
		//Converts a valid type to string (bool, int, float, etc.)
		template<class Template>
		static std::string ToString( Template Type )
		{
			std::ostringstream OutStringStream;
			OutStringStream << Type;
			return OutStringStream.str();
		}

		static int ToInt( std::string s )
		{
			std::stringstream ss(s);
			int i;
			ss >> i;

			return i;
		}

		static float ToFloat( std::string s )
		{
			std::stringstream ss(s);
			float f;
			ss >> f;

			return f;
		}
};

#endif
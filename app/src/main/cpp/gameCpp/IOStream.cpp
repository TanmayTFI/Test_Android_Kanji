//===========================================================
// 
// Framework
//===========================================================

#include "IOStream.h"
#include "Util.h"
#include "Console.h"
#include "Resources.h"


//Avoid forcing value to bool
#pragma warning (disable :4800)

EEndian CIOStream::Endianness = Endian_Invalid;

CIOStream::CIOStream()
: pStream(NULL)
{

}

CIOStream::~CIOStream()
{
	Close();
}

bool CIOStream::Open(std::string File, EIOType iotype)
{
	Close();//close previous streams!!

	if( CIOStream::Endianness == Endian_Invalid )
		CheckEndian();

	switch(iotype)
	{		
		case File_Read:
			pStream = new std::fstream(File.c_str(), std::ios_base::in);				
			if( pStream->fail() )
			{
				std::string TmpStr = std::string("Er0x1801- Stream unable to open file: " + File).c_str();		
				#ifdef K_WIN32
					WARN((TmpStr).c_str()) ;//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
				#else
					WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
				#endif
				return false;				
			}
			break;
		case File_Write:
			pStream = new std::fstream(File.c_str(), std::ios_base::out);
			if( pStream->fail() )
			{
				std::string TmpStr = std::string("Er0x1802- Stream unable to write file: " + File).c_str();	
				#ifdef K_WIN32
					WARN((TmpStr).c_str()) ;//WIDESTR WARNINGS	////Removed Wide string conversion for Windows	
				#else
					WARN( (const char*)(TmpStr.c_str()) );//WIDESTR WARNINGS	
				#endif

				return false;				
			}
			break;
		default:
            
			//Unsupported stream at this time
			#ifdef K_WIN32
				FATAL("Er0x1803- I/O file stream type not supported.");////Removed Wide string conversion for Windows	
			#else
				FATAL((const char*)"Er0x1803- I/O file stream type not supported.");
			#endif
            /*  When FATAL is called it will automatically exit after that, so this code is useless, and hence commenting
            int x = 100000;
			while(--x) {}
			exit(1);
             */
	}
	return true;
}

void CIOStream::Close()
{	
	if(pStream)
	{
		delete pStream;
		pStream = NULL;
	}
}

void CIOStream::WriteInt(int n)
{
	if(CIOStream::Endianness == Endian_Big)
		n = SwapEndian(n);

	std::string str = MKSTR(n);

	WriteString(str);
}

void CIOStream::WriteFloat(float f)
{
	if(CIOStream::Endianness == Endian_Big)
		f = SwapEndian(f);
		
	std::string str = MKSTR(f);

	WriteString(str);
}


void CIOStream::WriteChar(unsigned char c)
{
	(*pStream) << c << std::endl;
}

void CIOStream::WriteBool(bool b)
{
	WriteInt( static_cast<int>(b) );
}

void CIOStream::WriteString(const std::string& string)
{
	(*pStream) << string.c_str() << std::endl;
}

int CIOStream::ReadInt()
{
	std::string str = ReadString();
	int n = CUtil::ToInt(str);

	if(CIOStream::Endianness == Endian_Big)
		n = SwapEndian(n);

	return n;
}

float CIOStream::ReadFloat()
{
	std::string str = ReadString();
	float f = CUtil::ToFloat(str);

	if(CIOStream::Endianness == Endian_Big)
		f = SwapEndian(f);

	return f;
}

unsigned char CIOStream::ReadChar()
{
	char c[1];
	
	pStream->getline(c, 1);

	return c[0];
}

bool CIOStream::ReadBool()
{
	bool b = static_cast<bool>( ReadInt() );

	return b;
}

std::string CIOStream::ReadString()
{
	char s[2048];
	std::string str;

	
	pStream->getline(s, 2048);
	str = s;

	

	return str;
}

int CIOStream::ReadIntSplit(char a)
{
	std::string::size_type index;
	std::string str = ReadString();

	index = str.find(a);
	str = str.substr(index+1);

	int n = CUtil::ToInt(str);

	if(CIOStream::Endianness == Endian_Big)
		n = SwapEndian(n);

	return n;
}

float CIOStream::ReadFloatSplit(char a)
{
	std::string::size_type index;
	std::string str = ReadString();

	index = str.find(a);
	str = str.substr(index+1);

	float n = CUtil::ToFloat(str);

	if(CIOStream::Endianness == Endian_Big)
		n = SwapEndian(n);

	return n;
}

bool CIOStream::ReadBoolSplit(char a)
{
	std::string::size_type index;
	std::string str = ReadString();

	index = str.find(a);
	str = str.substr(index+1);

	int n = CUtil::ToInt(str);

	if(CIOStream::Endianness == Endian_Big)
		n = SwapEndian(n);

	bool b = static_cast<bool>( n );

	return b;
}

std::string CIOStream::ReadStringSplit(char a)
{
	std::string::size_type index;
	std::string str = ReadString();

	index = str.find(a);
	str = str.substr(index+1);

	return str;
}

//Only valid for ints/floats on x86/x64/PPC hardware
template <typename t>
t CIOStream::SwapEndian(t n)
{
	t out;
	char* bout = (char*)&out;
	char* bin  = (char*)&n;

	bout[0] = bin[3];
	bout[1] = bin[2];
	bout[2] = bin[1];
	bout[3] = bin[0];

	return out;
}

void CIOStream::CheckEndian()
{
	int i = 1;
	char *p = (char *)&i;

	if(p[0] == 1)
		CIOStream::Endianness = Endian_Little;
	else 
		CIOStream::Endianness = Endian_Big;
}

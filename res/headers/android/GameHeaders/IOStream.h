//===========================================================
// 
// Framework
//===========================================================
//===========================================================
// Important Information
//
// Endianness
// Considering our target platforms are Windows and Mac OSX,
// we must write an I/O class which supports both big and 
// little endian architecture. The largest percentage of our 
// targets will be using little endian machines, and our 
// pipeline toolset is .Net based and thus little endian as
// well. To make development easier with these systems in mind,
// we won't be outputting target-specific data files. Instead,
// we will output all data files in little endian. On little
// endian hardware, we will perform native read and writes.
// On big endian hardware, we will byte swap both input and 
// output to maintain this paradigm. The choice to byte swap
// on big endian architectures at run-time may incur more 
// performance cost than is acceptable, in which case we may
// be forced to export all data target-specific.
// 
// Integral type sizes
// Fortunately, all x86, x64, and PowerPC architectures use
// the same set of sizes for the C++ integral types we need to 
// use (int, char, float) so we will not be defining fixed-
// width integral types nor special read/write routines for
// such types.
//
// Output Format
// CIOStream writes plain text data line by line. There 
// will eventually be a CIOBinaryStream deriving from
// this class, but for now it's unneccessary.
//===========================================================

#ifndef FRMWRK_IOSTREAM_H
#define FRMWRK_IOSTREAM_H

#include "Framework.h"
#include "Enums.h"
#include <iostream>
#include <istream>
#include <string>

class CIOStream
{
	protected:
		
		static EEndian Endianness;
		
		void CheckEndian();

	public:
		std::fstream *pStream;
		
		

		CIOStream();
		virtual ~CIOStream();

		virtual bool Open(std::string File, EIOType iotype);
		virtual void Close();

		virtual void WriteInt	(int n);
		virtual void WriteFloat	(float f);
		virtual void WriteChar	(unsigned char c);
		virtual void WriteBool  (bool b);
		virtual void WriteString(const std::string& string);

		virtual int				ReadInt();
		virtual float			ReadFloat();
		virtual unsigned char	ReadChar();
		virtual bool			ReadBool();
		virtual std::string		ReadString();

		//Specific support for csv files
		virtual int				ReadIntSplit(char a);
		virtual float			ReadFloatSplit(char a);
		virtual bool			ReadBoolSplit(char a);
		virtual std::string     ReadStringSplit(char a);

		//Designed to swap endianness
		//for integers and floats
		template <typename t>
		t SwapEndian(t n);
};

#endif
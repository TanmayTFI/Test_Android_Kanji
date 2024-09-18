//===========================================================
// 
// Framework
//===========================================================

#ifndef ENUMS_H
#define ENUMS_H

enum EScreenEffect
{
	NoEffect,
	FadeToWhite,
	FadeToBlack,
	FadeToNormal
};

enum EScreenStatus
{
	FadeInactive,
	FadingToNormal,
	FadingToWhite,
	FadingToBlack
};

enum EEndian
{
	Endian_Little,
	Endian_Big,
	Endian_Invalid
};

enum EIOType
{
	File_Write,
	File_Read,
	IO_Invalid
};

#endif
//====================================
// Framework
// Resolution definitions
//====================================

#ifndef FRMWRK_RESOLUTION_DEF
#define FRMWRK_RESOLUTION_DEF


	//////////////
	// Of 18 modes, only full screenmodes supported, switch to lower window display modes for all else!!//
	//////////////
	//1920x1200
	//1920x1080
	//1768x992
	//1680x1050	
	//1600X900

	//1600x1200 (4:3)
	//1400X1050 (4:3)
	//1600x1024 
	//1600x900
	//1280x1024 

	//1280x960 (4:3)
	//1280x800 
	//1280x600 - will clip off game elements!!

	//1152x864 (4:3)
	//1176x664
	
////////////////////
///// 7 modes //////
////////////////////
//1366x768 
//1360x768
//1280x768
//1280X720
//1024x768 (4:3)
// 800x600 (4:3)
// 640x480 (4:3)
	
enum eResolution
{
	R_INVALID,
    R640x480,  //1
	R800x600,  //2
 	R1024x640, //3 Mac Book pro
	R1024x768, //4 
    R1152x720,  //5 Mac Book pr
	R1280x768, //6
	R1280x720, //7
	R1280x800, //8 Mac Book pro
    R1344x756, //9 Odd resolution found in Benq monitor (with Mac ??)
	R1360x768, //10
	R1366x768, //11
	R1600x900, //12 Odd resolution found in Benq monitor (with Mac ??)
    R1920x1080, //13 Full HD
	R1920x1200, //14 Full HD+
    R2560x1440, //15 QHD
    R3840x2160, //16 4K
    R7680x4320, //17 8K
	Resolution_Max
};

struct SResolution
{
	int Width, Height, BPP;
	bool CanWindow;
};

#endif

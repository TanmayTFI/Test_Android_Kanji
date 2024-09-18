//====================================
// Framework
// Save Data
//====================================

#ifndef FRMWRK_SAVEDATA_DEF
#define FRMWRK_SAVEDATA_DEF

class CIOStream;

class SaveData
{
	public:
		SaveData();
		virtual ~SaveData();

		void Save(CIOStream *stream);
		void SaveBackup(CIOStream *stream);
		bool Load(CIOStream *stream);
		void Invalidate();
		int     ProfileVersion;
		int		Resolution;
        int     BPP;
        bool	Windowed;

	protected:
		virtual void Write(CIOStream *stream) = 0;
		virtual void Read(CIOStream* stream) = 0;


};

#endif
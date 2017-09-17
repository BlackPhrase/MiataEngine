#include "Sound.hpp"

CSound::CSound(IEngineInterface *apEngine) : mpEngine(apEngine){}
CSound::~CSound() = default;

/*
================
S_Init
================
*/
bool CSound::Init()
{
	mpEngine->GetLogger()->Printf("Sound Initialization");
	
	/*
	mpCvarList->Register(&nosound);
	mpCvarList->Register(&volume);
	mpCvarList->Register(&precache);
	mpCvarList->Register(&loadas8bit);
	mpCvarList->Register(&bgmvolume);
	mpCvarList->Register(&bgmbuffer);
	mpCvarList->Register(&ambient_level);
	mpCvarList->Register(&ambient_fade);
	mpCvarList->Register(&snd_noextraupdate);
	mpCvarList->Register(&snd_show);
	mpCvarList->Register(&_snd_mixahead);
	*/
	
	//if(host_parms.memsize < 0x800000)
	{
		//mpEngine->GetCvarController()->SetCvarString("loadas8bit", "1");
		//mpEngine->GetLogger()->Printf("loading all sounds as 8bit\n");
	};
	
	snd_initialized = true;
	
	return true;
};

// =======================================================================
// Shutdown sound engine
// =======================================================================
void CSound::Shutdown()
{
	mpEngine->GetLogger()->Printf("Sound shutdown...");
};

void CSound::Frame()
{
};
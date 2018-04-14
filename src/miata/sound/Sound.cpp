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
	
	if(COM_CheckParm("-simsound"))
		fakedma = true;
	
	Cmd_AddCommand("play", S_Play);
	Cmd_AddCommand("playvol", S_PlayVol);
	Cmd_AddCommand("stopsound", S_StopAllSoundsC);
	Cmd_AddCommand("soundlist", S_SoundList);
	Cmd_AddCommand("soundinfo", S_SoundInfo_f);
	
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
	
	Startup();
	
	SND_InitScaletable();
	
	//if(host_parms.memsize < 0x800000)
	{
		//mpEngine->GetCvarController()->SetCvarString("loadas8bit", "1");
		//mpEngine->GetLogger()->Printf("loading all sounds as 8bit\n");
	};
	
	known_sfx = Hunk_AllocName(MAX_SFX * sizeof(sfx_t), "sfx_t");
	num_sfx = 0;

	// create a piece of DMA memory

	if(fakedma)
	{
		shm = (void *)Hunk_AllocName(sizeof(*shm), "shm");
		shm->splitbuffer = 0;
		shm->samplebits = 16;
		shm->speed = 22050;
		shm->channels = 2;
		shm->samples = 32768;
		shm->samplepos = 0;
		shm->soundalive = true;
		shm->gamealive = true;
		shm->submission_chunk = 1;
		shm->buffer = Hunk_AllocName(1 << 16, "shmbuf");
	}

	Con_Printf("Sound sampling rate: %i\n", shm->speed);

	// provides a tick sound until washed clean

	//	if (shm->buffer)
	//		shm->buffer[4] = shm->buffer[5] = 0x7f;	// force a pop for debugging

	ambient_sfx[AMBIENT_WATER] = S_PrecacheSound("ambience/water1.wav");
	ambient_sfx[AMBIENT_SKY] = S_PrecacheSound("ambience/wind2.wav");
	
	snd_initialized = true;
	
	S_StopAllSounds(true);
	
	return true;
};

// =======================================================================
// Shutdown sound engine
// =======================================================================
void CSound::Shutdown()
{
	mpEngine->GetLogger()->Printf("Sound shutdown...");
	
	if(!sound_started)
		return;

	if(shm)
		shm->gamealive = 0;

	shm = 0;
	sound_started = 0;

	if(!fakedma)
		SNDDMA_Shutdown();
};

void CSound::Frame()
{
};

/*
================
S_Startup
================
*/
void CSound::Startup()
{
	int rc;

	if(!snd_initialized)
		return;

	if(!fakedma)
	{
		rc = SNDDMA_Init();

		if(!rc)
		{
#ifndef _WIN32
			Con_Printf("S_Startup: SNDDMA_Init failed.\n");
#endif
			sound_started = 0;
			return;
		}
	}

	sound_started = 1;
};
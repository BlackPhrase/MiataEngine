/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// sound.h -- client sound i/o functions

#ifndef __SOUND__
#define __SOUND__


void S_Init (void);
void S_Startup (void);
void S_Shutdown (void);
void S_StartSound (int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol,  float attenuation);
void S_StaticSound (sfx_t *sfx, vec3_t origin, float vol, float attenuation);
void S_StopSound (int entnum, int entchannel);
void S_StopAllSounds(qboolean clear);
void S_ClearBuffer (void);
void S_Update (vec3_t origin, vec3_t v_forward, vec3_t v_right, vec3_t v_up);
void S_ExtraUpdate (void);

sfx_t *S_PrecacheSound (const char *sample);
void S_TouchSound (const char *sample);
void S_ClearPrecache (void);
void S_BeginPrecaching (void);
void S_EndPrecaching (void);
void S_InitPaintChannels (void);



// ====================================================================
// User-setable variables
// ====================================================================

#define	MAX_CHANNELS			128
#define	MAX_DYNAMIC_CHANNELS	8


extern	channel_t   channels[MAX_CHANNELS];
// 0 to MAX_DYNAMIC_CHANNELS-1	= normal entity sounds
// MAX_DYNAMIC_CHANNELS to MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS -1 = water, etc
// MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS to total_channels = static sounds

extern	int			total_channels;

//
// Fake dma is a synchronous faking of the DMA progress used for
// isolating performance in the renderer.  The fakedma_updates is
// number of times S_Update() is called per second.
//

extern qboolean 		fakedma;
extern int 			fakedma_updates;
extern int		paintedtime;
extern vec3_t listener_origin;
extern vec3_t listener_forward;
extern vec3_t listener_right;
extern vec3_t listener_up;
extern volatile dma_t *shm;
extern volatile dma_t sn;
extern vec_t sound_nominal_clip_dist;

extern	cvar_t loadas8bit;
extern	cvar_t bgmvolume;
extern	cvar_t volume;

extern qboolean	snd_initialized;

extern int		snd_blocked;

void S_LocalSound (const char *s);

void S_AmbientOff (void);
void S_AmbientOn (void);

#endif

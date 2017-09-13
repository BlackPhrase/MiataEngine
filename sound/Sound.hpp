#pragma once

#include "ISound.hpp"

//====================================================================
// User-setable variables
//====================================================================

#define MAX_CHANNELS 128
#define MAX_DYNAMIC_CHANNELS 8

//====================================================================

// !!! if this is changed, it much be changed in asm_i386.h too !!!
struct channel_t
{
	///
	bool IsLooping() const;
	
	///
	void SetOrigin(const vec3_t &origin);
	
	///
	const vec3_t &GetOrigin() const;
	
	sfx_t *sfx{nullptr};      // sfx number
	
	int leftvol{0};     // 0-255 volume
	int rightvol{0};    // 0-255 volume
	
	int end{0};         // end time in global paintsamples
	int pos{0};         // sample position in sfx
	int looping{0};     // where to loop, -1 = no looping
	int entnum{0};      // to allow overriding a specific sound
	int entchannel{0};  //
	
	vec3_t origin{0.0f, 0.0f, 0.0f};   // origin of sound effect
	
	float dist_mult{0.0f}; // distance multiplier (attenuation/clipK)
	
	int master_vol{0};  // 0-255 master volume
};

class CSound final : public ISound
{
public:
	CSound();
	~CSound();
	
	bool Init() override;
	void Shutdown() override;
	
	void Frame() override;
private:
	// 0 to MAX_DYNAMIC_CHANNELS-1	= normal entity sounds
	// MAX_DYNAMIC_CHANNELS to MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS -1 = water, etc
	// MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS to total_channels = static sounds
	channel_t channels[MAX_CHANNELS]{};

	int total_channels{0};
	
	bool snd_initialized{false};
};
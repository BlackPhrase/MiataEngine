#include "commontypes.hpp"
#include "Video.hpp"

C_EXPORT IVideo *GetVideo()
{
	static CVideo Video;
	return &Video;
};
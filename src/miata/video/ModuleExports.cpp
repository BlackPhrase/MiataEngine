#include "commontypes.hpp"
#include "Video.hpp"

C_EXPORT IVideo *GetVideo(IEngineInterface *apEngineInterface)
{
	static CVideo Video(apEngineInterface);
	return &Video;
};
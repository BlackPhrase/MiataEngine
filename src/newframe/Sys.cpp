#include "Sys.hpp"

int CSys::FileOpenRead(const char *path, int *hndl)
{
};

int CSys::FileOpenWrite(const char *path)
{
};

void CSys::FileClose(int handle)
{
};

void CSys::FileSeek(int handle, int position)
{
};

int CSys::FileRead(int handle, void *dest, int count)
{
};

int CSys::FileWrite(int handle, void *data, int count)
{
};

int	CSys::FileTime(const char *path)
{
};

void CSys::mkdir(const char *path)
{
};

void CSys::MakeCodeWriteable(unsigned long startaddr, unsigned long length)
{
};

void CSys::DebugLog(const char *file, char *fmt, ...)
{
};

void CSys::Error(const char *error, ...)
{
};

void CSys::Printf(const char *fmt, ...)
{
};

void CSys::Quit()
{
};

double CSys::GetDoubleTime()
{
};

char *CSys::GetConsoleInput()
{
};

/// called to yield for a little bit so as
/// not to hog cpu when paused or debugging
void CSys::Sleep()
{
};

/// Perform Key_Event() callbacks until the input que is empty
void CSys::SendKeyEvents()
{
};

void CSys::LowFPPrecision()
{
};

void CSys::HighFPPrecision()
{
};

void CSys::SetFPCW()
{
};
#include "Draw.hpp"

void CDraw::Init()
{
};

void CDraw::Character(int x, int y, int num)
{
};

void CDraw::DebugChar(char num)
{
};

void CDraw::SubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height)
{
};

void CDraw::Pic(int x, int y, qpic_t *pic)
{
};

void CDraw::TransPic(int x, int y, qpic_t *pic)
{
};

void CDraw::TransPicTranslate(int x, int y, qpic_t *pic, byte *translation)
{
};

void CDraw::ConsoleBackground(int lines)
{
};

void CDraw::BeginDisc()
{
};

void CDraw::EndDisc()
{
};

void CDraw::TileClear(int x, int y, int w, int h)
{
};

void CDraw::Fill(int x, int y, int w, int h, int c)
{
};

void CDraw::FadeScreen()
{
};

void CDraw::String(int x, int y, const char *str)
{
};

void CDraw::Alt_String(int x, int y, const char *str)
{
};

qpic_t *CDraw::PicFromWad(const char *name)
{
};

qpic_t *CDraw::CachePic(const char *path)
{
};

void CDraw::Crosshair()
{
};
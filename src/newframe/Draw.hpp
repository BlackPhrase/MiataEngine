#pragma once

class CDraw
{
public:
	void Init();
	
	void Character(int x, int y, int num);
	void DebugChar(char num);
	
	void SubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height);
	void Pic(int x, int y, qpic_t *pic);
	
	void TransPic(int x, int y, qpic_t *pic);
	void TransPicTranslate(int x, int y, qpic_t *pic, byte *translation);
	
	void ConsoleBackground(int lines);
	
	void BeginDisc();
	void EndDisc();
	
	void TileClear(int x, int y, int w, int h);
	
	void Fill(int x, int y, int w, int h, int c);
	void FadeScreen();
	
	void String(int x, int y, const char *str);
	void Alt_String(int x, int y, const char *str);
	
	qpic_t *PicFromWad(const char *name);
	qpic_t *CachePic(const char *path);
	
	void Crosshair();
};
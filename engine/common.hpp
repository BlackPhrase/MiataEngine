/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief general definitions

#pragma once

#define MAX_INFO_STRING 196
#define MAX_SERVERINFO_STRING 512
#define MAX_LOCALINFO_STRING 32768

//============================================================================

typedef struct sizebuf_s sizebuf_t;

sizebuf_t *SZ_Alloc(const char *name, int startsize);
void SZ_Free(sizebuf_t *buf);

//============================================================================

typedef struct link_s
{
	struct link_s *prev, *next;
} link_t;

void ClearLink(link_t *l);
void RemoveLink(link_t *l);
void InsertLinkBefore(link_t *l, link_t *before);
void InsertLinkAfter(link_t *l, link_t *after);

// (type *)STRUCT_FROM_LINK(link_t *link, type, member)
// ent = STRUCT_FROM_LINK(link,entity_t,order)
// FIXME: remove this mess!
#define STRUCT_FROM_LINK(l, t, m) ((t *)((byte *)l - (int)&(((t *)0)->m)))

//============================================================================

#define Q_MAXCHAR ((char)0x7f)
#define Q_MAXSHORT ((short)0x7fff)
#define Q_MAXINT ((int)0x7fffffff)
#define Q_MAXLONG ((int)0x7fffffff)
#define Q_MAXFLOAT ((int)0x7fffffff)

#define Q_MINCHAR ((char)0x80)
#define Q_MINSHORT ((short)0x8000)
#define Q_MININT ((int)0x80000000)
#define Q_MINLONG ((int)0x80000000)
#define Q_MINFLOAT ((int)0x7fffffff)

//============================================================================

extern bool bigendien;

extern short (*BigShort)(short l);
extern short (*LittleShort)(short l);
extern int (*BigLong)(int l);
extern int (*LittleLong)(int l);
extern float (*BigFloat)(float l);
extern float (*LittleFloat)(float l);

//============================================================================

void MSG_WriteChar(sizebuf_t *sb, int c);
void MSG_WriteByte(sizebuf_t *sb, int c);
void MSG_WriteShort(sizebuf_t *sb, int c);
void MSG_WriteLong(sizebuf_t *sb, int c);
void MSG_WriteFloat(sizebuf_t *sb, float f);
void MSG_WriteString(sizebuf_t *sb, char *s);
void MSG_WriteCoord(sizebuf_t *sb, float f);
void MSG_WriteAngle(sizebuf_t *sb, float f);

extern int msg_readcount;
extern bool msg_badread; // set if a read goes beyond end of message

void MSG_BeginReading();

int MSG_ReadChar();
int MSG_ReadByte();
int MSG_ReadShort();
int MSG_ReadLong();
float MSG_ReadFloat();
char *MSG_ReadString();

float MSG_ReadCoord();
float MSG_ReadAngle();

//============================================================================

void Q_memset(void *dest, int fill, int count);
void Q_memcpy(void *dest, const void *src, int count);
int Q_memcmp(const void *m1, const void *m2, int count);
void Q_strcpy(char *dest, const char *src);
void Q_strncpy(char *dest, const char *src, int count);
int Q_strlen(const char *str);
char *Q_strrchr(char *s, char c);
void Q_strcat(char *dest, const char *src);
int Q_strcmp(const char *s1, const char *s2);
int Q_strncmp(const char *s1, const char *s2, int count);
int Q_strcasecmp(const char *s1, const char *s2);
int Q_strncasecmp(const char *s1, const char *s2, int n);
int Q_atoi(const char *str);
float Q_atof(const char *str);

//============================================================================

extern char com_token[1024];
extern bool com_eof;

char *COM_Parse(const char *data);

char *COM_SkipPath(const char *pathname);
void COM_StripExtension(const char *in, char *out);
void COM_FileBase(const char *in, char *out);
void COM_DefaultExtension(const char *path, char *extension);

char *va(const char *format, ...);
// does a varargs printf into a temp buffer

//============================================================================

extern int com_filesize;
struct cache_user_s;

extern char com_gamedir[MAX_OSPATH];

void COM_WriteFile(const char *filename, const void *data, int len);
int COM_OpenFile(const char *filename, int *hndl);
int COM_FOpenFile(const char *filename, FILE **file);
void COM_CloseFile(int h);

byte *COM_LoadStackFile(const char *path, const void *buffer, int bufsize);
byte *COM_LoadTempFile(const char *path);
byte *COM_LoadHunkFile(const char *path);
void COM_LoadCacheFile(const char *path, struct cache_user_s *cu);
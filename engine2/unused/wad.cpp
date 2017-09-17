

void SwapPic(qpic_t *pic);

/*
==================
W_CleanupName

Lowercases name and pads with spaces and a terminating 0 to the length of
lumpinfo_t->name.
Used so lumpname lookups can proceed rapidly by comparing 4 chars at a time
Space padding is so names can be printed nicely in tables.
Can safely be performed in place.
==================
*/
void W_CleanupName(const char *in, char *out)
{
	int i;
	int c;

	for(i = 0; i < 16; i++)
	{
		c = in[i];
		if(!c)
			break;

		if(c >= 'A' && c <= 'Z')
			c += ('a' - 'A');
		out[i] = c;
	}

	for(; i < 16; i++)
		out[i] = 0;
}



/*
=============
W_GetLumpinfo
=============
*/
lumpinfo_t *W_GetLumpinfo(const char *name)
{
	int i;
	lumpinfo_t *lump_p;
	char clean[16];

	W_CleanupName(name, clean);

	for(lump_p = wad_lumps, i = 0; i < wad_numlumps; i++, lump_p++)
	{
		if(!strcmp(clean, lump_p->name))
			return lump_p;
	}

	Sys_Error("W_GetLumpinfo: %s not found", name);
	return NULL;
}

void *W_GetLumpName(const char *name)
{
	lumpinfo_t *lump;

	lump = W_GetLumpinfo(name);

	return (void *)(wad_base + lump->filepos);
}

void *W_GetLumpNum(int num)
{
	lumpinfo_t *lump;

	if(num < 0 || num > wad_numlumps)
		Sys_Error("W_GetLumpNum: bad number: %i", num);

	lump = wad_lumps + num;

	return (void *)(wad_base + lump->filepos);
}

/*
=============================================================================

automatic byte swapping

=============================================================================
*/

void SwapPic(qpic_t *pic)
{
	pic->width = LittleLong(pic->width);
	pic->height = LittleLong(pic->height);
}
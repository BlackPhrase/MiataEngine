void GL_Init();

PROC glArrayElementEXT;
PROC glColorPointerEXT;
PROC glTexCoordPointerEXT;
PROC glVertexPointerEXT;

lp3DFXFUNC glColorTableEXT;

bool gl_mtexable = false;

//====================================

BINDTEXFUNCPTR bindTexFunc;

#define TEXTURE_EXT_STRING "GL_EXT_texture_object"

void CheckTextureExtensions()
{
	char *tmp;
	qboolean texture_ext;
	HINSTANCE hInstGL;

	texture_ext = FALSE;
	/* check for texture extension */
	tmp = (unsigned char *)glGetString(GL_EXTENSIONS);
	while(*tmp)
	{
		if(strncmp((const char *)tmp, TEXTURE_EXT_STRING, strlen(TEXTURE_EXT_STRING)) == 0)
			texture_ext = TRUE;
		tmp++;
	}

	if(!texture_ext || COM_CheckParm("-gl11"))
	{
		hInstGL = LoadLibrary("opengl32.dll");

		if(hInstGL == NULL)
			Sys_Error("Couldn't load opengl32.dll\n");

		bindTexFunc = (void *)GetProcAddress(hInstGL, "glBindTexture");

		if(!bindTexFunc)
			Sys_Error("No texture objects!");
		return;
	}

	/* load library and get procedure adresses for texture extension API */
	if((bindTexFunc = (BINDTEXFUNCPTR)
	    wglGetProcAddress((LPCSTR) "glBindTextureEXT")) == NULL)
	{
		Sys_Error("GetProcAddress for BindTextureEXT failed");
		return;
	}
};

void CheckArrayExtensions()
{
	char *tmp;

	/* check for texture extension */
	tmp = (unsigned char *)glGetString(GL_EXTENSIONS);
	while(*tmp)
	{
		if(strncmp((const char *)tmp, "GL_EXT_vertex_array", strlen("GL_EXT_vertex_array")) == 0)
		{
			if(
			((glArrayElementEXT = wglGetProcAddress("glArrayElementEXT")) == NULL) ||
			((glColorPointerEXT = wglGetProcAddress("glColorPointerEXT")) == NULL) ||
			((glTexCoordPointerEXT = wglGetProcAddress("glTexCoordPointerEXT")) == NULL) ||
			((glVertexPointerEXT = wglGetProcAddress("glVertexPointerEXT")) == NULL))
			{
				Sys_Error("GetProcAddress for vertex extension failed");
				return;
			}
			return;
		}
		tmp++;
	}

	Sys_Error("Vertex array extension not present");
};

#ifdef _WIN32
void CheckMultiTextureExtensions()
{
	if(strstr(gl_extensions, "GL_SGIS_multitexture ") && !COM_CheckParm("-nomtex"))
	{
		Con_Printf("Multitexture extensions found.\n");
		qglMTexCoord2fSGIS = (void *)wglGetProcAddress("glMTexCoord2fSGIS");
		qglSelectTextureSGIS = (void *)wglGetProcAddress("glSelectTextureSGIS");
		gl_mtexable = true;
	}
}
#else
void CheckMultiTextureExtensions()
{
	gl_mtexable = true;
}
#endif

/*
===============
GL_Init
===============
*/
void GL_Init()
{
	gl_vendor = glGetString(GL_VENDOR);
	Con_Printf("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = glGetString(GL_RENDERER);
	Con_Printf("GL_RENDERER: %s\n", gl_renderer);

	gl_version = glGetString(GL_VERSION);
	Con_Printf("GL_VERSION: %s\n", gl_version);
	gl_extensions = glGetString(GL_EXTENSIONS);
	Con_Printf("GL_EXTENSIONS: %s\n", gl_extensions);

	//	Con_Printf ("%s %s\n", gl_renderer, gl_version);

	if(strnicmp(gl_renderer, "PowerVR", 7) == 0)
		fullsbardraw = true;

	if(strnicmp(gl_renderer, "Permedia", 8) == 0)
		isPermedia = true;

	CheckTextureExtensions();
	CheckMultiTextureExtensions();

	glClearColor(0, 0, 0, 0);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.666);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel(GL_FLAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

#if 0
	CheckArrayExtensions ();

	glEnable (GL_VERTEX_ARRAY_EXT);
	glEnable (GL_TEXTURE_COORD_ARRAY_EXT);
	glVertexPointerEXT (3, GL_FLOAT, 0, 0, &glv.x);
	glTexCoordPointerEXT (2, GL_FLOAT, 0, 0, &glv.s);
	glColorPointerEXT (3, GL_FLOAT, 0, 0, &glv.r);
#endif
};
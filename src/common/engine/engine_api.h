/*
Copyright (C) 2019-2020 BlackPhrase

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

#pragma once

struct engine_api_t
{
	struct console_api_t
	{
		void (*Printf)(const char *fmt, ...);
	};
	
	struct cmdbuffer_api_t
	{
		void (*AddText)(const char *text);
		void (*InsertText)(const char *text);
	};
	
	struct cvar_api_t
	{
		void (*Set)(const char *var_name, const char *value);
		void (*SetValue)(const char *var_name, float value);
	};
	
	struct cmd_api_t
	{
		void (*AddCommand)(const char *cmd_name, void (*callback)());
	};
	
	struct system_api_t
	{
	};
	
	struct sound_api_t
	{
		void (*LocalSound)(const char *sample);
		void (*ExtraUpdate)();
	};
	
	struct input_api_t
	{
	};
	
	console_api_t con;
	cmdbuffer_api_t cbuf;
	cvar_api_t cvar;
	cmd_api_t cmd;
	system_api_t system;
	sound_api_t sound;
	input_api_t input;
};
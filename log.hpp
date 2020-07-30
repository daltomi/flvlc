/*
	Copyright (C) 2015, 2019 daltomi <daltomi@disroot.org>

	This file is part of flvlc.

	flvlc is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	flvlc is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with flvlc.  If not, see <http://www.gnu.org/licenses/>.
*/


#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <assert.h>


static void va_fprintf(FILE *file, char const *const format, ...)
{
	assert(format);
	va_list vl;
	va_start(vl, format);
	vfprintf(file, format, vl);
	va_end(vl);
}

#define macro_string(x) #x
#define macrostr(x) macro_string(x)

#ifdef DEBUG
#define preformat __FILE__ ":" macrostr(__LINE__) ":"
#else
#define preformat
#endif

#define print_warning(...)	\
	va_fprintf(stdout, "[flvlc] Warning: " preformat __VA_ARGS__)

#define print_error(...)	\
	va_fprintf(stderr, "[flvlc] Error: " preformat __VA_ARGS__)

#define print_notice(...)	\
	va_fprintf(stdout, "[flvlc] Notice: " preformat __VA_ARGS__)

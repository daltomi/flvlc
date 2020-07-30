/*
	Copyright (C) 2015, 2019, 2019 daltomi <daltomi@disroot.org>

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

#include "playlist.hpp"

#include <dirent.h>
#include <sys/stat.h>

#include <FL/filename.H>
#include <fstream>
#include <string>

namespace FILES {
bool is_file(const char *);
bool is_uri(const char *);
bool make_name_file(std::string &name, const char *file);
}; // namespace FILES

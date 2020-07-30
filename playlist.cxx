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
#include "playlist.hpp"

Playlist::Playlist() : ipos(0) {}

Playlist::~Playlist() {}

bool Playlist::load(const char *path)
{
	std::string filename(path);
	filename += "playlist";
	std::ifstream file(filename);

	if (not file.is_open()) {
		return false;
	}

	unsigned int index;

	file >> index;

	while (true) {
		Item item{"", -1, false};

		file >> item.position;

		// - Elimina espacio en blanco que separa
		//	la posicion y el nombre del archivo.
		file.ignore(1, ' ');

		getline(file, item.name);

		if (file.fail()) {
			break;
		}

		item.b_watch = (item.position == -1) ? false : true;

		append(item);
	}

	file.close();

	if (items.size() == 0 or (index >= items.size())) {
		return false;
	}

	ipos = index;

	return true;
}

void Playlist::save(const char *path)
{
	if (items.size() == 0) {
		return;
	}

	std::string filename(path);
	filename += "playlist";
	std::ofstream file(filename, std::ios_base::trunc);

	file << ipos << '\n'; // begin line

	for (auto &item : items) {

		(item.b_watch) ? file << item.position : file << "-1";

		// - Espacio en blanco que separa
		// la posicion y el nombre del archivo.
		file << " ";

		// - Elimina el prefijo file://
		file << item.name.substr(7, item.name.size()) << '\n';
	}

	file.close();
}

void Playlist::clean()
{
	const Item itmp = items[ipos];
	items.clear();
	items.push_back(itmp);
	ipos = 0;
}

void Playlist::append(const int argc, const char *argv[])
{
	// clear index from Playlist::load()
	ipos = count();

	for (int i = 0; i < argc; ++i) {
		Item item{"", -1, false};

		if (not FILES::make_name_file(item.name, argv[i])) {
			continue;
		}

		items.push_back(item);
	}

#ifdef DEBUG
	print();
#endif
}

void Playlist::append(const char *name)
{
	Item item{name, -1, false};
	append(item);
}

// Nota: no avanza ipos, utilizar
// la func. position()
void Playlist::append(Item &item)
{
	// Item item{"", -1, false};

	if (FILES::make_name_file(item.name, item.name.c_str())) {
		items.push_back(item);
	}
}

const char *Playlist::current() const
{
	return items.at(ipos).name.c_str();
}

const char *Playlist::prev()
{
	--ipos;
	return items.at(ipos).name.c_str();
}

const char *Playlist::next()
{
	++ipos;
	return items.at(ipos).name.c_str();
}

const unsigned int Playlist::count() const
{
	return items.size();
}

bool Playlist::is_prev() const
{
	return ((ipos - 1) != -1);
}

bool Playlist::is_next() const
{
	return ((size_t)(ipos + 1) != count());
}

void Playlist::reset()
{
	ipos = 0;
}

// Unicamente para el actual item.
const char *Playlist::title() const
{
	return fl_filename_name(items[ipos].name.c_str());
}

bool Playlist::is_empty() const
{
	return (count() == 0);
}

const unsigned int Playlist::position() const
{
	return ipos + 1;
}

void Playlist::position(const unsigned int pos)
{
	ipos = pos - 1;
}

void Playlist::watch(const float pos)
{
	items.at(ipos).position = pos;
}

float Playlist::watch() const
{
	return items.at(ipos).position;
}

bool Playlist::is_watch() const
{
	if (!is_empty())
	{
		return (items.at(ipos).b_watch);
	}
	return false;
}

void Playlist::set_watch(bool val)
{
	if (!is_empty()) {
		items.at(ipos).b_watch = val;
	}
}

const ITEMS &Playlist::get_items() const
{
	return items;
}

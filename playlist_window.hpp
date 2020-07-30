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

#include "playlist.hpp"

#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Select_Browser.H>

class PlaylistWindow : public Fl_Menu_Window {
      public:
	PlaylistWindow(int x, int y, Playlist *pl);
	virtual ~PlaylistWindow();
	void select(const unsigned int pos) const;
	unsigned int selected() const;

      private:
	Fl_Select_Browser *browser;
	Fl_Button *btn_close;
	Fl_Button *btn_clean;
	Fl_Group *grp_playlist;
	unsigned int sel_line;
	Playlist *playlist;

      private:
	static void callback(Fl_Widget *w, void *data);
	void set_playlist();
};

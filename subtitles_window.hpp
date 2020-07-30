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

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Spinner.H>

#include "multimedia.hpp"
#include <vector>

class SubtitlesWindow : public Fl_Menu_Window {
      public:
	SubtitlesWindow(int x, int y, Multimedia *multimedia);
	~SubtitlesWindow();

      private:
	enum ID { VIDEO = 0, AUDIO };

	Fl_Choice *ch_video;
	Fl_Button *btn_open;
	Fl_Choice *ch_audio;
	Fl_Button *btn_close;
	Multimedia *multimedia;
	Fl_Group *grp_subtitle;
	Fl_Group *grp_audio;
	Fl_Spinner *spn_vDelay;
	Fl_Spinner *spn_aDelay;
	std::vector<int> map_sub[2];

      private:
	void init_choice(Fl_Choice *ch, Multimedia::Track track, ID id,
			 const int current);
	void open_file();
	static void callback(Fl_Widget *w, void *data);
};

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
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Window.H>
#include <FL/Fl_Preferences.H>

class Preferences_Window : public Fl_Menu_Window {
      public:
	Preferences_Window(int x, int y, Fl_Preferences *pref_parent);
	~Preferences_Window();

      private:
	Fl_Preferences *pref_parent;
	Fl_Group *grp_vlc;
	Fl_Group *grp_general;
	Fl_Input *input_param;
	Fl_Choice *ch_theme;
	Fl_Button *btn_cancel;
	Fl_Button *btn_save;
	Fl_Box *box_info_restart;
	Fl_Group *grp_playlist;
	Fl_Check_Button *chk_save;
	Fl_Box *box_info_save;

      private:
	static void callback(Fl_Widget *w, void *data);
	void initilize();

	// TODO: void initialize_theme();
};

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

#include "preferences_window.hpp"

Preferences_Window::Preferences_Window(int x, int y,
				       Fl_Preferences *pref_parent)
    : Fl_Menu_Window(x, y, 275, 240, 0), pref_parent(pref_parent)

{
	color((Fl_Color)215);

	grp_vlc = new Fl_Group(5, 80, 260, 50, "VLC:");
	grp_vlc->box(FL_BORDER_BOX);
	grp_vlc->color((Fl_Color)55);
	grp_vlc->labelsize(9);
	grp_vlc->labelfont(1);
	grp_vlc->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	input_param = new Fl_Input(89, 95, 166, 20, "(*) Parameters:");
	input_param->labelsize(9);
	input_param->textsize(9);
	input_param->callback(Preferences_Window::callback, this);

	grp_vlc->end();

	grp_general = new Fl_Group(5, 15, 260, 50, "General:");
	grp_general->box(FL_BORDER_BOX);
	grp_general->color((Fl_Color)55);
	grp_general->labelsize(9);
	grp_general->labelfont(1);
	grp_general->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	ch_theme = new Fl_Choice(63, 25, 192, 20, "(*) Theme:");
	ch_theme->down_box(FL_BORDER_BOX);
	ch_theme->labelsize(9);
	ch_theme->textsize(9);
	ch_theme->deactivate();
	ch_theme->callback(Preferences_Window::callback, this);

	grp_general->end();

	btn_cancel = new Fl_Button(200, 205, 60, 20, "&Cancel");
	btn_cancel->box(FL_SHADOW_BOX);
	btn_cancel->down_box(FL_SHADOW_BOX);
	btn_cancel->color((Fl_Color)55);
	btn_cancel->labelsize(9);
	btn_cancel->callback(Preferences_Window::callback, this);

	btn_save = new Fl_Button(125, 205, 60, 20, "&Save");
	btn_save->box(FL_SHADOW_BOX);
	btn_save->down_box(FL_SHADOW_BOX);
	btn_save->color((Fl_Color)55);
	btn_save->labelsize(9);
	btn_save->callback(Preferences_Window::callback, this);

	box_info_restart = new Fl_Box(5, 210, 95, 15, "(*) Need restart.");
	box_info_restart->labelsize(9);
	box_info_restart->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));

	grp_playlist = new Fl_Group(5, 145, 260, 50, "Playlist:");
	grp_playlist->box(FL_BORDER_BOX);
	grp_playlist->color((Fl_Color)55);
	grp_playlist->labelfont(1);
	grp_playlist->labelsize(9);
	grp_playlist->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	chk_save = new Fl_Check_Button(15, 160, 20, 25);
	chk_save->down_box(FL_DOWN_BOX);
	chk_save->callback(Preferences_Window::callback, this);

	box_info_save =
	    new Fl_Box(35, 160, 185, 25, "Automatically save and load.");
	box_info_save->labelsize(9);
	box_info_save->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));

	grp_playlist->end();

	end();

	initilize();

	box(FL_BORDER_BOX);
	set_modal();
	clear_border();
	position(x - (w() / 2), y - (h() / 2));
	show();
}

Preferences_Window::~Preferences_Window()
{
	delete btn_cancel;
	delete btn_save;
	delete ch_theme;
	delete input_param;
	delete box_info_restart;
	delete box_info_save;
	delete chk_save;
	///////////////////
	delete grp_vlc;
	delete grp_general;
	delete grp_playlist;
}

void Preferences_Window::initilize()
{
	char *char_value;
	int int_value;

	if (0 != pref_parent->get("vlc", char_value, "")) {
		input_param->value(char_value);
		free(char_value);
	}

	if (0 != pref_parent->get("pls/save", int_value, 0)) {
		chk_save->value(int_value);
	}
}

// TODO:
// void Preferences_Window::initialize_theme()
//{
//}

void Preferences_Window::callback(Fl_Widget *w, void *data)
{
	Preferences_Window *win = (Preferences_Window *)data;

	if (win->btn_cancel == w) {
		win->hide();
	} else if (win->btn_save == w) {
		win->pref_parent->set("vlc", win->input_param->value());
		win->pref_parent->set("pls/save", win->chk_save->value());
		win->pref_parent->flush();
		win->hide();
	}
}

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

#include "playlist_window.hpp"

PlaylistWindow::PlaylistWindow(int x, int y, Playlist *pl)
    : Fl_Menu_Window(x, y, 270, 220, 0), sel_line(0), playlist(pl)

{
	grp_playlist = new Fl_Group(5, 14, 260, 176, "Playlist");
	grp_playlist->box(FL_BORDER_BOX);
	grp_playlist->color(Fl_Color(55));
	grp_playlist->labelsize(9);
	grp_playlist->labelfont(1);
	grp_playlist->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	browser = new Fl_Select_Browser(10, 20, 245, 165);
	browser->textsize(9);
	browser->labelcolor(FL_GRAY0);
	browser->has_scrollbar(Fl_Browser_::VERTICAL);
	browser->box(FL_FLAT_BOX);
	browser->callback((Fl_Callback *)PlaylistWindow::callback, this);

	grp_playlist->end();

	btn_close = new Fl_Button(165, 195, 60, 20, "Close");
	btn_close->box(FL_SHADOW_BOX);
	btn_close->down_box(FL_SHADOW_BOX);
	btn_close->color(Fl_Color(55));
	btn_close->labelcolor(FL_GRAY0);
	btn_close->labelsize(9);
	btn_close->callback((Fl_Callback *)PlaylistWindow::callback, this);

	btn_clean = new Fl_Button(45, 195, 60, 20, "Clean");
	btn_clean->box(FL_SHADOW_BOX);
	btn_clean->down_box(FL_SHADOW_BOX);
	btn_clean->color(Fl_Color(55));
	btn_clean->labelcolor(FL_GRAY0);
	btn_clean->labelsize(9);
	btn_clean->callback((Fl_Callback *)PlaylistWindow::callback, this);

	end();

	set_playlist();

	box(FL_BORDER_BOX);
	color(Fl_Color(215));
	position(x - (w() / 2), y - (h() / 2));
	clear_border();
	set_modal();
	show();
}

PlaylistWindow::~PlaylistWindow()
{
	delete browser;
	delete btn_close;
	delete btn_clean;
	delete grp_playlist;
}

void PlaylistWindow::set_playlist()
{
	for (auto &item : playlist->get_items()) {
		browser->add(fl_filename_name(item.name.c_str()));
	}

	select(playlist->position());
}

unsigned int PlaylistWindow::selected() const
{
	return sel_line;
}

void PlaylistWindow::select(const unsigned int pos) const
{
	browser->select(pos);
}

void PlaylistWindow::callback(Fl_Widget *w, void *data)
{
	PlaylistWindow *win = (PlaylistWindow *)data;

	if (win->btn_clean == w) {
		win->browser->clear();
		win->browser->redraw();
		win->playlist->clean();
		win->browser->add(fl_filename_name(win->playlist->title()));
		win->browser->select(win->playlist->position());
		win->hide();
	} else if (win->btn_close == w or win->browser == w) {
		win->sel_line = win->browser->value();
		win->hide();
	}
}

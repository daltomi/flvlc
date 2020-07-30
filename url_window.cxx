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

#include "url_window.hpp"

URL_Window::URL_Window(int x, int y)
	: Fl_Menu_Window(x, y, 390, 70, 0), addr(nullptr)
{
	input = new Fl_Input(30, 10, 346, 24, "URL:");
	input->labelfont(1);
	input->labelsize(9);
	input->textfont(4);
	input->textsize(9);
	input->textcolor(Fl_Color(FL_BLUE));

	box_info = new Fl_Box(30, 40, 200, 10);
	box_info->labelsize(9);
	box_info->align(Fl_Align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE));
	box_info->label("Specify the protocol: http, ftp,...");

	btn_ok = new Fl_Button(245, 40, 60, 20, "&Accept");
	btn_ok->box(FL_SHADOW_BOX);
	btn_ok->color(FL_BACKGROUND2_COLOR);
	btn_ok->labelsize(9);
	btn_ok->callback(URL_Window::callback, (void *)this);
	btn_ok->deactivate();

	btn_cancel = new Fl_Button(316, 40, 60, 20, "&Cancel");
	btn_cancel->box(FL_SHADOW_BOX);
	btn_cancel->color(FL_BACKGROUND2_COLOR);
	btn_cancel->labelsize(9);
	btn_cancel->callback(URL_Window::callback, (void *)this);

	box(FL_BORDER_BOX);
	color((Fl_Color)215);
	set_modal();
	clear_border();
	position(x - (w() / 2), y - (h() / 2));
	end();
	show();

	Fl::add_timeout(0.100f, check_input_empty, (void *)this);
}

URL_Window::~URL_Window()
{
	Fl::remove_timeout(URL_Window::check_input_empty);
	delete btn_ok;
	delete box_info;
	delete btn_cancel;
	delete input;
}

const char *URL_Window::value() const
{
	return addr;
}

void URL_Window::callback(Fl_Widget *w, void *data)
{
	URL_Window *win = (URL_Window *)data;

	if (win->btn_ok == w) {
		win->addr = win->input->value();
	}

	win->hide();
}

void URL_Window::check_input_empty(void *data)
{
	URL_Window *win = (URL_Window *)data;

	if (nullptr == win->input->value() or
	    strlen(win->input->value()) == 0) {
		win->btn_ok->deactivate();
	} else {
		win->btn_ok->activate();
	}

	Fl::repeat_timeout(0.100f, URL_Window::check_input_empty, data);
}

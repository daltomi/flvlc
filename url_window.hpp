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
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Window.H>

class URL_Window : public Fl_Menu_Window {
      public:
	URL_Window(int x, int y);
	~URL_Window();
	const char *value() const;

      private:
	Fl_Input *input;
	Fl_Button *btn_ok;
	Fl_Button *btn_cancel;
	Fl_Box *box_info;
	const char *addr;

      private:
	static void callback(Fl_Widget *w, void *data);
	static void check_input_empty(void *data);
};

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
#include <FL/filename.H>
#include <FL/x.H>
/**
   icons xpm
*/
#include "icons.h"
/**
   should by define
*/
extern void event_control(Fl_Widget *);
extern void window_close_x(Fl_Widget *, void *);
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Tooltip.H>
#include <FL/Fl_Window.H>
#include <FL/fl_ask.H>

#include "version.hpp"

class MainWindow : public Fl_Double_Window {
      public:
	MainWindow();
	enum class STATE : char {
		MUTE,
		PLAY,
		PAUSE,
		VOLUME,
		VIDEO,
		STOP,
		NEXT,
		PREV,
		MENU_QUIT,
		MENU_OPEN_FILE,
		MENU_OPEN_URL,
		PLAYLIST,
		REPEAT,
		WATCH,
		SUBTITLES,
		PREFERENCES,
		SNAPSHOT
	};
	enum class REPEAT : char { NONE = 0, MEDIA, PLAYLIST };
	enum class WATCH : char { OFF = 0, ON };
	/**
	   should by define
	*/
	int handle(int);

	Fl_Slider *sl_video;
	Fl_Button *btn_play_pause;
	Fl_Button *btn_stop;
	Fl_Button *btn_prev;
	Fl_Button *btn_next;
	Fl_Slider *sl_volume;
	Fl_Button *btn_mute;
	Fl_Button *btn_repeat;
	Fl_Button *btn_watch;
	Fl_Button *btn_playlist;
	Fl_Button *btn_subtitles;
	Fl_Button *btn_preferences;
	Fl_Button *btn_snapshot;
	Fl_Window *display;
	Fl_Box *box_status;
	Fl_Box *box_time;
	Fl_Group *box_controls;
	REPEAT repeat_current;

      private:
	bool toggle_mute = false;
	bool toggle_pause = false;
	bool toggle_repeat = false;
	Fl_Tooltip *tips;

      private:
	static Fl_Menu_Item menu_items[];
	static void cb_menu(Fl_Menu_ *, long);
	void conf_button(Fl_Button *btn, STATE state, const char *tip,
			 Fl_Pixmap &active, Fl_Pixmap &inactive);

      public:
	int get_id() const;
	void center_on_screen();
	int get_w() const;
	int get_h() const;
	void mute_toggle();
	void repeat_toggle();
	void play_pause_toggle();
	void enable_seek_controls(bool prev, bool next);
	void toggle_fullscreen();
	void toggle_maximize();
	void show_play_controls();
	void hide_play_controls();
	void popup_menu();
	void watch_on();
	void watch_off();
	void showOSD();
};

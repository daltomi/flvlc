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

#include "main_window.hpp"

void MainWindow::cb_menu(Fl_Menu_ *o, long v)
{
	event_control((Fl_Widget *)o);
}

constexpr unsigned char SZMENU = 10;

Fl_Menu_Item MainWindow::menu_items[] = {
	//{"&File", 0,  0, 0, FL_SUBMENU | FL_MENU_DIVIDER, FL_NORMAL_LABEL, 0,
	//SZMENU, 0},
	{
		"&Open file...", 0, (Fl_Callback *)MainWindow::cb_menu,
		(void *)(STATE::MENU_OPEN_FILE), 0, FL_NORMAL_LABEL, 0,
		SZMENU, 0
	},
	{
		"&Open URL...", 0, (Fl_Callback *)MainWindow::cb_menu,
		(void *)(STATE::MENU_OPEN_URL), FL_MENU_DIVIDER, FL_NORMAL_LABEL, 0,
		SZMENU, 0
	},
	{
		"&Quit", 0, (Fl_Callback *)MainWindow::cb_menu,
		(void *)(STATE::MENU_QUIT), 0, FL_NORMAL_LABEL, 0,
		SZMENU, 0
	},
	{ 0 }
};

MainWindow::MainWindow() : Fl_Double_Window(0, 0, 445, 415, TITLE)
{
	clear_flag(16);

	box(FL_FLAT_BOX);
	color(FL_BLACK);
	selection_color(FL_BACKGROUND_COLOR);
	labeltype(FL_NO_LABEL);
	when(FL_WHEN_RELEASE);
	callback(window_close_x);
	repeat_current = REPEAT::NONE;

	display = new Fl_Window(0, 0, w(), 347);
	display->box(FL_FLAT_BOX);
	display->color(FL_BLACK);
	display->clear_border();
	// display->resizable(this);
	display->end();

	resizable(display);

	tips = new Fl_Tooltip();
	tips->size(10);
	tips->delay(0.5);

	box_controls = new Fl_Group(0, 350, 444, 85);
	box_controls->box(FL_FLAT_BOX);
	box_controls->color(FL_BLACK);

	sl_video = new Fl_Slider(3, 349, 438, 10);
	sl_video->type(3);
	sl_video->box(FL_BORDER_BOX);
	sl_video->color((Fl_Color)38);
	sl_video->selection_color((Fl_Color)228);
	sl_video->clear_visible_focus();
	sl_video->user_data((void *)(STATE::VIDEO));
	sl_video->align(Fl_Align(FL_ALIGN_BOTTOM_LEFT));
	sl_video->deactivate();
	sl_video->minimum(0);
	sl_video->callback((Fl_Callback0 *)event_control);
	sl_video->slider_size(0.1f);

	btn_play_pause = new Fl_Button(15, 365, 25, 25);
	conf_button(btn_play_pause, STATE::PLAY, "Play", image_play_active,
		    image_play_inactive);

	btn_stop = new Fl_Button(45, 365, 25, 25);
	conf_button(btn_stop, STATE::STOP, "Stop", image_stop_active,
		    image_stop_inactive);

	btn_prev = new Fl_Button(75, 365, 25, 25);
	conf_button(btn_prev, STATE::PREV, "Previous media", image_prev_active,
		    image_prev_inactive);

	btn_next = new Fl_Button(105, 365, 25, 25);
	conf_button(btn_next, STATE::NEXT, "Next media", image_next_active,
		    image_next_inactive);

	btn_repeat = new Fl_Button(135, 365, 25, 25);
	conf_button(btn_repeat, STATE::REPEAT, "Repeat none",
		    image_repeat_active, image_repeat_inactive);

	btn_watch = new Fl_Button(165, 365, 25, 25);
	conf_button(btn_watch, STATE::WATCH, "Watch off", image_watch_active,
		    image_watch_inactive);

	btn_subtitles = new Fl_Button(195, 365, 25, 25);
	conf_button(btn_subtitles, STATE::SUBTITLES, "Subtitles / Audio",
		    image_subtitles_active, image_subtitles_inactive);

	btn_snapshot = new Fl_Button(225, 365, 25, 25);
	conf_button(btn_snapshot, STATE::SNAPSHOT, "Snapshot",
		    image_snapshot_active, image_snapshot_inactive);

	btn_preferences = new Fl_Button(255, 365, 25, 25);
	conf_button(btn_preferences, STATE::PREFERENCES, "Preferences",
		    image_preferences_active, image_preferences_inactive);

	{
		Fl_Box *o = new Fl_Box(281, 370, 5, 5);
		o->box(FL_FLAT_BOX);
		o->hide();
		//	o->color(FL_RED);
		o->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
		Fl_Group::current()->resizable(o);
	}

	sl_volume = new Fl_Slider(370, 370, 70, 10);
	sl_volume->type(3);
	sl_volume->color((Fl_Color)38);
	sl_volume->selection_color((Fl_Color)228);
	sl_volume->box(FL_BORDER_BOX);
	sl_volume->labelsize(7);
	sl_volume->deactivate();
	sl_volume->clear_visible_focus();
	sl_volume->user_data((void *)(STATE::VOLUME));
	sl_volume->align(Fl_Align(257)); // FL_ALIGN_BOTTOM_RIGHT));
	sl_volume->callback((Fl_Callback0 *)event_control);
	sl_volume->minimum(0);

	btn_mute = new Fl_Button(340, 365, 25, 25);
	conf_button(btn_mute, STATE::MUTE, "Mute off", image_volume_active,
		    image_volume_inactive);

	Fl_Group *box_bar = new Fl_Group(1, 392, 443, 26);
	box_bar->box(FL_FLAT_BOX);
	// o->color(FL_FOREGROUND_COLOR);
	box_bar->color(FL_BLACK);
	box_bar->labelcolor(FL_BACKGROUND2_COLOR);
	box_bar->align(Fl_Align(FL_ALIGN_BOTTOM_LEFT));

	box_time = new Fl_Box(325, 393, 130, 21, "00:00:00 / 00:00:00");
	box_time->box(FL_FLAT_BOX);
	box_time->color(FL_GRAY0);
	box_time->labelsize(9);
	box_time->labelcolor((Fl_Color)55);
	box_time->align(Fl_Align(FL_ALIGN_CENTER | FL_ALIGN_INSIDE));
	// Fl_Group::current()->resizable(box_time);

	{
		Fl_Box *o = new Fl_Box(150, 398, 90, 16);
		o->box(FL_FLAT_BOX);
		o->hide();
		Fl_Group::current()->resizable(o);
	}

	btn_playlist = new Fl_Button(268, 392, 50, 21, "0 / 0");
	conf_button(btn_playlist, STATE::PLAYLIST, "Show playlist",
		    image_playlist_active, image_playlist_inactive);

	box_status = new Fl_Box(1, 392, 114, 21);
	box_status->box(FL_FLAT_BOX);
	box_status->color(FL_BLACK);
	box_status->labelsize(9);
	box_status->labelcolor((Fl_Color)55);
	box_status->align(Fl_Align(324 | FL_ALIGN_INSIDE));
	box_status->image(image_file);
	box_status->tooltip("Media empty");
	box_status->label("None");
	Fl_Group::current()->resizable(box_status);

	box_bar->end();

	box_controls->end();

	size_range(445, 415);
	take_focus();
	set_modal();
	end();
}

void MainWindow::conf_button(Fl_Button *btn, STATE state, const char *tip,
			     Fl_Pixmap &active, Fl_Pixmap &inactive)
{
	btn->box(FL_NO_BOX);
	btn->down_box(FL_NO_BOX);
	btn->selection_color((Fl_Color)34);
	btn->clear_visible_focus();
	btn->color((Fl_Color)FL_GRAY0);
	btn->labelsize(9);
	btn->labelcolor((Fl_Color)55);
	btn->user_data((void *)(state));
	btn->align(Fl_Align(256 | FL_ALIGN_INSIDE));
	btn->callback((Fl_Callback0 *)event_control);
	btn->image(active);
	btn->deimage(inactive);
	btn->tooltip(tip);
	btn->deactivate();
}

void MainWindow::center_on_screen()
{
	int x, y, w, h;
	Fl::screen_xywh(x, y, w, h);
	this->position((w - this->w()) / 2, (h / 2) - this->h());
}

int MainWindow::get_id() const
{
	return fl_xid((const Fl_Window *)display);
}

int MainWindow::get_w() const
{
	return (display != nullptr) ? display->w() : 0;
}

int MainWindow::get_h() const
{
	return (display != nullptr) ? display->h() : 0;
}

void MainWindow::mute_toggle()
{
	if (toggle_mute) {
		btn_mute->image(image_volume_active);
		btn_mute->tooltip("Mute off");
	} else {
		btn_mute->image(image_volume_mute);
		btn_mute->tooltip("Mute on");
	}

	btn_mute->redraw_label();
	toggle_mute = !toggle_mute;
}

void MainWindow::repeat_toggle()
{
	if (repeat_current == REPEAT::NONE) {
		btn_repeat->image(image_repeat_media);
		repeat_current = REPEAT::MEDIA;
		btn_repeat->tooltip("Repeat media");
	} else if (repeat_current == REPEAT::MEDIA) {
		btn_repeat->image(image_repeat_playlist);
		repeat_current = REPEAT::PLAYLIST;
		btn_repeat->tooltip("Repeat playlist");
	} else {
		btn_repeat->image(image_repeat_active);
		repeat_current = REPEAT::NONE;
		btn_repeat->tooltip("Repeat none");
	}

	btn_repeat->redraw_label();
}

void MainWindow::watch_on()
{
	btn_watch->image(image_watch_active_on);
	btn_watch->tooltip("Watch on");
	btn_watch->redraw_label();
}

void MainWindow::watch_off()
{
	btn_watch->image(image_watch_active);
	btn_watch->tooltip("Watch off");
	btn_watch->redraw_label();
}

void MainWindow::play_pause_toggle()
{
	if (toggle_pause) {
		btn_play_pause->image(image_play_active);
		btn_play_pause->tooltip("Play");
	} else {
		btn_play_pause->image(image_pause);
		btn_play_pause->tooltip("Pause");
	}

	btn_play_pause->redraw_label();
	toggle_pause = !toggle_pause;
}

void MainWindow::enable_seek_controls(bool prev, bool next)
{
	(prev) ? btn_prev->activate() : btn_prev->deactivate();
	(next) ? btn_next->activate() : btn_next->deactivate();

	btn_prev->redraw();
	btn_next->redraw();
}

void MainWindow::toggle_fullscreen()
{
	if (0 == fullscreen_active()) {
		hide_play_controls();
		fullscreen();
		 resize(0, 0, Fl::w(), Fl::h());
	} else {
		fullscreen_off();
		show_play_controls();
	}
}

void MainWindow::show_play_controls()
{ 
	box_controls->show();
}

void MainWindow::hide_play_controls()
{
	box_controls->hide();
}

void MainWindow::popup_menu()
{
	const Fl_Menu_Item *item =
	    menu_items->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
	if (item) {
		item->do_callback((Fl_Widget *)item, item->user_data());
	}
}

void MainWindow::toggle_maximize()
{
	using Size = struct { int x, y, w, h; };
	static bool toggle = true;
	static Size sz_save{0, 0, 0, 0};

	if (toggle) {
		// I hope, you gcc, do a good work here.
		sz_save = Size{x(), y(), w(), h()};
		resize(0, 0, Fl::w(), Fl::h());
	} else {
		resize(sz_save.x, sz_save.y, sz_save.w, sz_save.h);
	}
	toggle = !toggle;
}

// - Muestra el cuadro de control si el modo
//   fullscreen esta activo.
void MainWindow::showOSD()
{
	if (0 != fullscreen_active()) {
		int x, y;
		Fl::get_mouse(x, y); // server side.

		if (y > box_controls->y()) {
			show_play_controls();
		} else {
			hide_play_controls();
		}
	}
}

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

#include "subtitles_window.hpp"

SubtitlesWindow::SubtitlesWindow(int x, int y, Multimedia *multimedia)
    : Fl_Menu_Window(x, y, 230, 230), multimedia(multimedia)

{
	this->color((Fl_Color)215);

	grp_subtitle = new Fl_Group(5, 15, 220, 85, "Subtitle:");
	grp_subtitle->box(FL_BORDER_BOX);
	grp_subtitle->color((Fl_Color)55);
	grp_subtitle->labelsize(9);
	grp_subtitle->labelfont(1);
	grp_subtitle->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	ch_video = new Fl_Choice(25, 25, 190, 20);
	ch_video->down_box(FL_BORDER_BOX);
	ch_video->textsize(9);
	ch_video->callback(SubtitlesWindow::callback, (void *)this);

	spn_vDelay = new Fl_Spinner(80, 65, 60, 20, "Delay (sec) :");
	spn_vDelay->box(FL_BORDER_BOX);
	spn_vDelay->labelsize(9);
	spn_vDelay->textsize(9);
	spn_vDelay->minimum(-100);
	spn_vDelay->maximum(100);
	spn_vDelay->callback(SubtitlesWindow::callback, (void *)this);

	btn_open = new Fl_Button(155, 65, 60, 20, "&Open file...");
	btn_open->box(FL_SHADOW_BOX);
	btn_open->down_box(FL_SHADOW_BOX);
	btn_open->color((Fl_Color)55);
	btn_open->selection_color(FL_DARK1);
	btn_open->labelsize(9);
	btn_open->callback(SubtitlesWindow::callback, (void *)this);
	btn_open->clear_visible_focus();

	grp_subtitle->end();

	grp_audio = new Fl_Group(5, 115, 220, 75, "Audio:");
	grp_audio->box(FL_BORDER_BOX);
	grp_audio->color((Fl_Color)55);
	grp_audio->labelsize(9);
	grp_audio->labelfont(1);
	grp_audio->align(Fl_Align(FL_ALIGN_TOP_LEFT));

	ch_audio = new Fl_Choice(25, 125, 190, 20);
	ch_audio->down_box(FL_BORDER_BOX);
	ch_audio->textsize(9);
	ch_audio->callback(SubtitlesWindow::callback, (void *)this);

	spn_aDelay = new Fl_Spinner(80, 155, 60, 20, "Delay (sec) :");
	spn_aDelay->labelsize(9);
	spn_aDelay->textsize(9);
	spn_aDelay->minimum(-100);
	spn_aDelay->maximum(100);
	spn_aDelay->callback(SubtitlesWindow::callback, (void *)this);

	grp_audio->end();

	btn_close = new Fl_Button(155, 200, 60, 20, "&Close");
	btn_close->box(FL_SHADOW_BOX);
	btn_close->down_box(FL_SHADOW_BOX);
	btn_close->color((Fl_Color)55);
	btn_close->selection_color(FL_DARK1);
	btn_close->labelsize(9);
	btn_close->callback(SubtitlesWindow::callback, (void *)this);
	btn_close->clear_visible_focus();

	end();

	box(FL_BORDER_BOX);
	border(0);
	set_modal();
	position(x - (w() / 2), y - (h() / 2));

	Multimedia::Track track = multimedia->get_video_subtitle_description();
	int current = multimedia->get_video_subtitle();

	init_choice(ch_video, track, ID::VIDEO, current);

	track = multimedia->get_audio_track_description();
	current = multimedia->get_audio_track();

	init_choice(ch_audio, track, ID::AUDIO, current);

	spn_vDelay->value(multimedia->get_video_subtitle_delay());
	spn_aDelay->value(multimedia->get_audio_track_delay());

	show();
}

SubtitlesWindow::~SubtitlesWindow()
{
	delete btn_close;
	delete btn_open;
	delete ch_video;
	delete ch_audio;
	delete spn_vDelay;
	delete spn_aDelay;
	///////////////////
	delete grp_subtitle;
	delete grp_audio;
}

void SubtitlesWindow::init_choice(Fl_Choice *ch, Multimedia::Track track, ID id,
				  const int current)
{
	unsigned int count = 0;

	while (track != nullptr) {
		ch->add(track->psz_name);

		map_sub[id].push_back(track->i_id);

		if (track->i_id == current) {
			ch->value(count);
		}
		track = track->p_next;
		++count;
	}

	if (0 == count) {
		ch->deactivate();

		if (id == ID::VIDEO) {
			spn_vDelay->deactivate();
		}

		if (id == ID::AUDIO) {
			spn_aDelay->deactivate();
		}
	}
}

void SubtitlesWindow::callback(Fl_Widget *w, void *data)
{
	SubtitlesWindow *win = (SubtitlesWindow *)data;

	if (win->btn_open == w) {
		win->open_file();
	} else if (win->btn_close == w) {
		win->hide();
	} else if (win->ch_video == w) {
		const int value = win->ch_video->value();
		win->multimedia->set_video_subtitle(
		    win->map_sub[ID::VIDEO][value]);
	} else if (win->ch_audio == w) {
		const int value = win->ch_audio->value();
		win->multimedia->set_audio_track(
		    win->map_sub[ID::AUDIO][value]);
	} else if (win->spn_vDelay == w) {
		win->multimedia->set_video_subtitle_delay(
		    win->spn_vDelay->value());
	} else if (win->spn_aDelay == w) {
		win->multimedia->set_audio_track_delay(
		    win->spn_aDelay->value());
	}
}

void SubtitlesWindow::open_file()
{
	Fl_Native_File_Chooser chooser;
	chooser.title("Open subtitle file");
	chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);

	if (chooser.show() != 0) {
		return;
	}

	multimedia->set_video_subtitle(chooser.filename());

	hide();
}

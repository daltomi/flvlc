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
#include "flvlc.hpp"

namespace FLVLC {
static MainWindow window;
static Multimedia *multimedia = nullptr;
static Playlist playlist;
static PlaylistWindow *pl_window = nullptr;
static SubtitlesWindow *sbt_window = nullptr;
static URL_Window *url_window = nullptr;
static Preferences_Window *pref_window = nullptr;
static Fl_Preferences *preferences = nullptr;
static MainWindow::STATE state = MainWindow::STATE::STOP;
static bool b_fullscreen = false;
static constexpr char *time_cero = "00:00:00 / 00:00:00";
static std::string time_length;
static float video_percent_low;
static float video_percent_high;
};

void FLVLC::cb_end()
{
	std::thread th{action_stop_next};
	th.detach();
}

/* No se recibe feedback si estado es pause*/
void FLVLC::cb_time_changed(float value)
{
	update_time_info(value);

	window.sl_video->value(value);

	if (playlist.is_watch()) {
		playlist.watch(value);
	}
}

void FLVLC::cb_length_changed(float value)
{
	// TODO: al archivo de configuración.
	const unsigned int minutes_seek_low = 1;
	const unsigned int minutes_seek_high = 4;

	video_percent_low = (value * minutes_seek_low) / 100;
	video_percent_high = (value * minutes_seek_high) / 100;
	time_length = parse_time(value);
	window.enable_seek_controls(playlist.is_prev(), playlist.is_next());
	window.sl_video->bounds(1, value);
	window.sl_video->value(1);
	update_label_playlist();
	if (nullptr != pl_window) {
		pl_window->select(playlist.position());
	}
}

void FLVLC::update_time_info(const float value)
{
	static std::string time_now;

	time_now = parse_time(value);
	time_now += " / " + time_length;
	window.box_time->label(time_now.c_str());
}

void FLVLC::update_label_playlist()
{
	static std::string label;
	std::stringstream ss;
	ss << playlist.position() << " / " << playlist.count();
	label = ss.str();
	window.btn_playlist->label(label.c_str());
	window.btn_playlist->redraw();
}

/* extern callack */
void event_control(Fl_Widget *w)
{
	using STATE = MainWindow::STATE;

	const STATE id = (const STATE)(w->argument());

	switch (id) {
	case STATE::PLAY:
		FLVLC::action_play_pause();
		break;

	case STATE::STOP:
		FLVLC::action_stop();
		break;

	case STATE::PREV:
		FLVLC::action_prev();
		break;

	case STATE::NEXT:
		FLVLC::action_next();
		break;

	case STATE::MUTE:
		FLVLC::action_mute();
		break;

	case STATE::REPEAT:
		FLVLC::action_repeat();
		break;

	case STATE::WATCH:
		FLVLC::action_watch();
		break;

	case STATE::VOLUME:
		FLVLC::action_volume();
		break;

	case STATE::VIDEO:
		if (FLVLC::window.sl_video->value() == FLVLC::window.sl_video->maximum()) {
			FLVLC::action_stop_next();
		}
		FLVLC::action_video();
		break;

	case STATE::MENU_OPEN_FILE:
		FLVLC::action_open_file();
		break;

	case STATE::MENU_OPEN_URL:
		FLVLC::action_open_url();
		break;

	case STATE::MENU_QUIT:
		FLVLC::action_quit();
		break;

	case STATE::PLAYLIST:
		FLVLC::action_playlist();
		break;

	case STATE::SUBTITLES:
		FLVLC::action_subtitles();
		break;

	case STATE::PREFERENCES:
		FLVLC::action_preferences();
		break;

	case STATE::SNAPSHOT:
		FLVLC::action_snapshot();
		break;

	default:
		print_warning("Unknow STATE %d\n", id);
	}
}

/* extern callback */
void window_close_x(Fl_Widget *, void *)
{
	Fl::remove_timeout(FLVLC::update_gui);

	FLVLC::save_playlist();

	if (nullptr != FLVLC::multimedia) {
		delete FLVLC::multimedia;
	}
	if (nullptr != FLVLC::preferences) {
		delete FLVLC::preferences;
	}
	exit(0);
}

void FLVLC::toggle_fullscreen()
{
	window.toggle_fullscreen();
}

const std::string FLVLC::parse_time(float value)
{
	lldiv_t qr = lldiv(value, 1000);
	qr = lldiv(qr.quot, 60);
	const float second = qr.rem;
	qr = lldiv(qr.quot, 60);
	const float minute = qr.rem;
	const float hour = qr.quot;

	std::stringstream ss;
	(hour < 10.0f) ? ss << "0" << hour << ":" : ss << hour << ":";
	(minute < 10.0f) ? ss << "0" << minute << ":" : ss << minute << ":";
	(second < 10.0f) ? ss << "0" << second : ss << second;

	return ss.str();
}

/*++++++++ Begin ACTION ++++++++*/

void FLVLC::action_mute()
{
	if (multimedia) {
		window.mute_toggle();
		multimedia->toggle_mute();
	}
}

void FLVLC::action_repeat()
{ 
	window.repeat_toggle();
}

void FLVLC::action_watch()
{
	if (playlist.is_watch()) {
		window.watch_off();
		playlist.set_watch(false);
	} else {
		window.watch_on();
		playlist.set_watch(true);
	}
}

void FLVLC::action_playlist()
{
	if (playlist.count() != 0 and pl_window == nullptr) {
		pl_window = new PlaylistWindow(Fl::event_x_root(),
					       Fl::event_y_root(), &playlist);

		while (pl_window->shown()) {
			Fl::wait();
		}

		const unsigned int sel_line = pl_window->selected();

		delete pl_window;
		pl_window = nullptr;

		if (0 != sel_line and sel_line != playlist.position()) {
			playlist.position(sel_line);
			play_seek(playlist.current());
		}

		window.enable_seek_controls(playlist.is_prev(),
					    playlist.is_next());
		update_label_playlist();
	}
}

void FLVLC::action_subtitles()
{
	if (!multimedia) {
		return;
	}

	if (FLVLC::state != MainWindow::STATE::STOP and
	    playlist.count() != 0 and sbt_window == nullptr) {
		sbt_window = new SubtitlesWindow(
		    Fl::event_x_root(), Fl::event_y_root(), multimedia);

		while (sbt_window->shown()) {
			Fl::wait();
		}

		delete sbt_window;
		sbt_window = nullptr;
	}
}

void FLVLC::action_preferences()
{
	if (nullptr == pref_window and nullptr != preferences) {
		pref_window = new Preferences_Window(
		    Fl::event_x_root(), Fl::event_y_root(), preferences);

		while (pref_window->shown()) {
			Fl::wait();
		}

		delete pref_window;
		pref_window = nullptr;
	}
}

void FLVLC::action_volume()
{
	if(multimedia) {
		multimedia->set_volume(FLVLC::window.sl_volume->value());
	}
}

void FLVLC::action_video()
{
	if (!multimedia) {
		return;
	}

	const float value = window.sl_video->value();

	if (FLVLC::state == MainWindow::STATE::PAUSE) {
		update_time_info(value);
	}
	multimedia->set_position(value);
}

void FLVLC::action_quit()
{ 
	window_close_x(nullptr, nullptr);
}

void FLVLC::action_play_pause()
{
	if (!multimedia or playlist.is_empty()) {
		return;
	}

	if (state != MainWindow::STATE::STOP) {
		state = (MainWindow::STATE::PAUSE == state)
			    ? MainWindow::STATE::PLAY
			    : MainWindow::STATE::PAUSE;
		window.play_pause_toggle();
		multimedia->pause();
	} else {
		// FIXME: usar cuando VLC solucione
		// el problema de volumen inicio
		// action_volume();
		play_seek(playlist.current());
	}

	Fl::redraw();
}

void FLVLC::action_stop()
{
	if (!multimedia or MainWindow::STATE::STOP == state) {
		return;
	}

	if (MainWindow::STATE::PLAY == state or
	    state != MainWindow::STATE::PAUSE) {
		window.play_pause_toggle();
	}

	state = MainWindow::STATE::STOP;
	window.btn_stop->deactivate();
	window.sl_video->value(0);
	window.sl_video->deactivate();
	window.btn_mute->deactivate();
	window.sl_volume->deactivate();
	window.btn_repeat->deactivate();
	window.btn_watch->deactivate();
	window.btn_subtitles->deactivate();
	window.btn_snapshot->deactivate();
	window.watch_off();
	playlist.set_watch(false);
	multimedia->stop();

	window.box_time->label(time_cero);

	Fl::redraw();
}

void FLVLC::action_prev()
{
	if (!multimedia or playlist.is_empty() or
	    not playlist.is_prev()) {
		return;
	}

	play_seek(playlist.prev());
}

void FLVLC::action_next()
{
	if (!multimedia or playlist.is_empty() or
	    not playlist.is_next()) {
		return;
	}

	play_seek(playlist.next());
}

/* thread */
void FLVLC::action_stop_next()
{
	if (window.repeat_current == MainWindow::REPEAT::MEDIA) {
		play_seek(playlist.current());
	} else if (window.repeat_current == MainWindow::REPEAT::PLAYLIST and
		   not playlist.is_next()) {
		action_stop();
		playlist.reset();
		action_play_pause();
	} else {
		action_stop();
		action_next();
	}
}

void FLVLC::action_open_file()
{
	Fl_Native_File_Chooser chooser;
	chooser.title("Open media file");
	chooser.type(Fl_Native_File_Chooser::BROWSE_MULTI_FILE);

	if (chooser.show() != 0) {
		return;
	}

	const unsigned int old_pos = playlist.count();

	for (auto i = 0; i < chooser.count(); ++i) {
		playlist.append(chooser.filename(i));
	}

	if (nullptr == multimedia) {
		make_multimedia();
	}

	if (old_pos > 0) {
		playlist.position(old_pos + 1);
	}

	play_seek(playlist.current());
}

void FLVLC::action_open_url()
{
	url_window = new URL_Window(Fl::event_x_root(), Fl::event_y_root());

	while (url_window->shown()) {
		Fl::wait();
	}

	const char *addr = url_window->value();

	if ((nullptr == addr) or (strlen(addr) == 0)) {
		delete url_window;
		url_window = nullptr;
		return;
	}

	window.sl_video->value(0);

	const unsigned int old_pos = playlist.count();

	/* Puede que addr no sea un archivo ni tampoco una URI*/
	playlist.append(addr);

	delete url_window;
	url_window = nullptr;

	/* Si fallo addr en append() se termina */
	if (playlist.count() == old_pos) {
		return;
	}

	if (nullptr == multimedia) {
		make_multimedia();
	}

	if (old_pos > 0) {
		playlist.position(old_pos + 1);
	}

	play_seek(playlist.current());
}

void FLVLC::action_snapshot()
{
	if (multimedia and state != MainWindow::STATE::STOP) {
		const char *dir = fl_getenv("HOM");
		if (dir == nullptr) {
			print_warning("Failed to save the snapshot in ""%s"". Using /tmp insted.\n", "HOME");
			dir = "/tmp/";
		}
		multimedia->snapshot(dir);
	}
}

/*++++++++ End ACTION ++++++++*/

/*
 * Funcion especial para:
 * 1 - action_playlist
 * 2 - action_prev
 * 3 - action_next
 * 4 - action_open_file
 * 5 - action_open_url
 * 6 - action_play_pause
 * */
void FLVLC::play_seek(const char *media)
{

	if (MainWindow::STATE::PAUSE == state or
	    MainWindow::STATE::STOP == state) {
		window.btn_play_pause->activate();
		window.btn_stop->activate();
		window.sl_video->activate();
		window.btn_mute->activate();
		window.sl_volume->activate();
		window.btn_repeat->activate();
		window.btn_watch->activate();
		window.btn_subtitles->activate();
		window.btn_playlist->activate();
		window.btn_snapshot->activate();
		window.play_pause_toggle();
	}


	state = MainWindow::STATE::PLAY;
	multimedia->play(media);
	update_title_info();

	if (multimedia->is_mute()) {
		multimedia->mute();
	}

	if (playlist.is_watch()) {
		window.watch_on();
		multimedia->set_position(playlist.watch());
	} else {
		window.watch_off();
	}

	Fl::redraw();
}

int MainWindow::handle(int event)
/* extern callback override*/
{
	int ret = Fl_Window::handle(event);

	switch (event) {
	case FL_KEYDOWN: {
		switch (Fl::event_key()) {
		case 'o':
			FLVLC::action_open_file();
			break;

		case 'h':
			if (false == FLVLC::b_fullscreen) {
				FLVLC::window.toggle_maximize();
			}
			break;

		case 'u':
			FLVLC::action_open_url();
			break;

		case 'l':
			FLVLC::action_playlist();
			break;

		case 't':
			FLVLC::action_subtitles();
			break;

		case 'p':
			FLVLC::action_preferences();
			break;

		case 'r':
			FLVLC::action_repeat();
			break;

		case 'w':
			FLVLC::action_watch();
			break;

		case FL_Escape:
		case 'q':
			FLVLC::action_quit();
			break;

		case ' ':
			FLVLC::action_play_pause();
			break;

		case 'm':
			FLVLC::action_mute();
			break;

		case 's':
			FLVLC::action_stop();
			break;

		case 'y':
			FLVLC::action_snapshot();
			break;

		case 'n':
			FLVLC::action_next();
			break;

		case 'b':
			FLVLC::action_prev();
			break;

		case 'f':
			FLVLC::toggle_fullscreen();
			break;

		case '0':
			FLVLC::up_volume();
			break;

		case '9':
			FLVLC::down_volume();
			break;

		case '.':
			if (FLVLC::state != MainWindow::STATE::STOP and
			    FLVLC::state == MainWindow::STATE::PAUSE) {
				double value =	FLVLC::window.sl_video->value() + 60.0;
				FLVLC::window.sl_video->value(value);
				if (value > FLVLC::window.sl_video->maximum()) {
					FLVLC::action_stop_next();
				}
				FLVLC::action_video();
			}
			break;

		case ',':
			if (FLVLC::state != MainWindow::STATE::STOP and
			    FLVLC::state == MainWindow::STATE::PAUSE) {
				double value =	FLVLC::window.sl_video->value() - 60.0;
				if (value < FLVLC::window.sl_video->minimum()) {
					value = FLVLC::window.sl_video->minimum();
				}
				FLVLC::window.sl_video->value(value);
				FLVLC::action_video();
			}
			break;


VIDEO_SEEK_UP: // usado por FL_MOUSEWHEEL

		case FL_Right:
			if (FLVLC::state != MainWindow::STATE::STOP) {
				double value =	FLVLC::window.sl_video->value() +
						FLVLC::video_percent_low;
				FLVLC::window.sl_video->value(value);
				if (value > FLVLC::window.sl_video->maximum()) {
					FLVLC::action_stop_next();
				}
				FLVLC::action_video();
			}
			break;

		case FL_Up:
			if (FLVLC::state != MainWindow::STATE::STOP) {
				double value =	FLVLC::window.sl_video->value() +
						FLVLC::video_percent_high;
				if (value > FLVLC::window.sl_video->maximum()) {
					FLVLC::action_stop_next();
				}
				FLVLC::window.sl_video->value(value);
				FLVLC::action_video();
			}
			break;

VIDEO_SEEK_DOWN: // usado por FL_MOUSEWHEEL

		case FL_Left:
			if (FLVLC::state != MainWindow::STATE::STOP) {
				double value =	FLVLC::window.sl_video->value() -
						FLVLC::video_percent_low;
				if (value < FLVLC::window.sl_video->minimum()) {
					value = FLVLC::window.sl_video->minimum();
				}
				FLVLC::window.sl_video->value(value);
				FLVLC::action_video();
			}
			break;

		case FL_Down:
			if (FLVLC::state != MainWindow::STATE::STOP) {
				double value =	FLVLC::window.sl_video->value() -
						FLVLC::video_percent_high;
				if (value < FLVLC::window.sl_video->minimum()) {
					value = FLVLC::window.sl_video->minimum();
				}
				FLVLC::window.sl_video->value(value);
				FLVLC::action_video();
			}
			break;

		}
		break;
	} // FL_KEYDOWN

	case FL_MOUSEWHEEL:
		if (not Fl::event_inside(box_controls)) {
			if (1 == Fl::event_dy()) {
				goto VIDEO_SEEK_DOWN;
			} else {
				goto VIDEO_SEEK_UP;
			}
		} else {
			if (1 ==  Fl::event_dy()) {
				FLVLC::down_volume();
			} else {
				FLVLC::up_volume();
			}
		}
		break;

	case FL_FULLSCREEN:
		FLVLC::b_fullscreen = !FLVLC::b_fullscreen;
		break;

	case FL_PUSH:
		if (Fl::event_button() == FL_RIGHT_MOUSE) {
			FLVLC::window.popup_menu();
		}
		break;
	} // end switch

	return ret;
}

void FLVLC::up_volume()
{
	if (MainWindow::STATE::STOP == state) {
		return;
	}

	int vol = FLVLC::window.sl_volume->value() + 15;
	if (vol > 200) {
		vol = 200;
	}
	// ver initialize_volume()
	FLVLC::window.sl_volume->value(vol);
	FLVLC::action_volume();
}

void FLVLC::down_volume()
{
	if (MainWindow::STATE::STOP == state) {
		return;
	}

	int vol = FLVLC::window.sl_volume->value() - 15;
	if (vol < 0) {
		vol = 0;
	}
	// ver initialize_volume()
	FLVLC::window.sl_volume->value(vol);
	FLVLC::action_volume();
}

void FLVLC::update_title_info()
{
	const char *title = playlist.title();
	window.box_status->tooltip(title);
	window.box_status->label(title);
}

void FLVLC::initialize_volume()
{
	// 0 == mute, 100 == 0dB
	window.sl_volume->bounds(0, 200);
	window.sl_volume->value(100);
}

/*
 * Se fuerza la actualización de FLTK */
void FLVLC::update_gui(void *)
{
	FLVLC::window.showOSD();
	Fl::check();
	Fl::repeat_timeout(0.100f, update_gui);
}

void FLVLC::make_multimedia()
{
	const int w = FLVLC::window.get_w();
	const int h = FLVLC::window.get_h();

	FLVLC::multimedia =
	    new Multimedia(preferences, FLVLC::window.get_id(), w, h);
	FLVLC::initialize_volume();
}

void FLVLC::initialize_preferences()
{
	if (nullptr == preferences) {
		preferences = new Fl_Preferences(Fl_Preferences::Root::USER,
						 "FLVLC", "flvlc");
		window.btn_preferences->activate();
	}
}

void FLVLC::load_playlist()
{
	int int_value;
	if (0 == preferences->get("pls/save", int_value, 0) or int_value == 0) {
		return;
	}

	char path[FL_PATH_MAX] = {'\0'};

	if (0 == preferences->getUserdataPath(path, FL_PATH_MAX - 1)) {
		return;
	}

	playlist.load(path);
}

void FLVLC::save_playlist()
{
	int int_value;
	if (0 == preferences->get("pls/save", int_value, 0) or
	    0 == playlist.count()) {
		return;
	}

	char path[FL_PATH_MAX];

	if (0 == preferences->getUserdataPath(path, FL_PATH_MAX - 1)) {
		return;
	}

	playlist.save(path);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

int main(const int argc, const char *argv[])
{
	print_notice("Version: %s - Build: %s\n", VERSION, BUILD);
	print_notice("VLC version: %s\n", libvlc_get_version());
	print_notice("VLC compiler: %s\n", libvlc_get_compiler());

	FLVLC::window.center_on_screen();
	FLVLC::window.show();
	FLVLC::initialize_preferences();
	Fl::add_timeout(0.50f, FLVLC::update_gui);

	FLVLC::load_playlist();

	if (argc >= 2) {
		FLVLC::playlist.append(argc - 1, &argv[1]);
	}

	if (not FLVLC::playlist.is_empty()) {
		FLVLC::make_multimedia();
		FLVLC::action_play_pause();
	}

	return Fl::run();
}

flvlc
=====

Note: I published this project first in https://sourceforge.net/projects/flvlc/

<img src="https://git.disroot.org/daltomi/flvlc/raw/branch/master/screenshot.png"/>


Platform:
----------

- GNU/Linux.

- Windows (flvlc <=2.12, end support).


Compile
-------
- Linux

	- Requirements:

		  - FLTK >=1.3 (dev)

		  - libvlc >=2.1 ( or vlc 2.1 ) (dev)

			  Note: some "distros" need extra plugins.

		  - make

		  - pkg-config

		  - GCC support c++11

- Build

	```bash
	make release
			-- or --
	make debug
	```



Testing
---------

For testing enable debug build.
See logs from a terminal.



Help commands
-------------------

| Key	| Description
| --------|------------
| h | toggle maximize
| l | show playlist window
| p | show preferences window
| u | open URL
| y | take one snapshot
| o | open file
| t | show subtitles window
| w | watch toggle
| r | toggle repeat modes
| f | toggle fullscreen
| m | toggle mute audio
| s | stop media
| Esc - q |  quit
| n | next media
| b | previous media
| Space | toggle play/pause
| . | next video frame (pause)
| , | previous video frame (pause)
| 0 | volume up
| 9 | volume down
| arrow right | video foward low
| arrow left | video backward low
| arrow up | video foward high
| arrow down | video backward high
| Mouse wheel | video foward low / video backward low


Notes
-----

- Repeat modes:

	- none

	- repeat current media file.

	- repeat playlist

- Popup Menu:

	- Press right button.

- Watch button:

	- Save the current position video while running.

- Snapshot:

	- Save to temp directory.

- Automatically save and load playlist.

	- Not by default.

- Open URL:

	- Alpha, testing.

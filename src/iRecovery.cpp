/***
 * iRecovery++ libusb based usb interface for iBoot and iBSS
 * Copyright © 2010  GreySyntax
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "headers/Program.h"
#include "headers/Device.h"

using namespace std;

static vector<string> Plugins;
static Device MobileDevice;
static lua_State *L;

bool call_plugin(const char* name) {
	char file[0x800], file2[0x800];
	strcpy(file, name);
	strcat(file, ".irecv");
	vector<string>::iterator it;
	it = find(Plugins.begin(), Plugins.end(), file);

	if (it == Plugins.end())
		return false;

	strcpy(file2, "./plugins/");
	strcat(file2, file);

	cout << file2 << endl;
	luaL_dofile(L, file);
	lua_close(L);
	return true;
}

bool load_plugins() {

	Plugins = vector<string>();
	DIR *path;

	if ((path = opendir("./plugins")) != NULL) {

		struct dirent *loc;
		while ((loc = readdir(path)) != NULL) {

			string fname = string(loc->d_name);
			if (fname != "." && fname != ".." && fname.substr(fname.find_last_of(".") + 1) == "irecv")
				Plugins.push_back(fname);
		}

		if (Plugins.size() > 0) {
			L = lua_open();
			luaL_openlibs(L);
			//TODO Register functions
		}
		call_plugin("test");
		closedir(path);
		return true;
	}

	cout << "[Error] # " << errno << " while opening \"./plugins\"";
	return false;
}

void info(bool a, bool c, bool u, bool e, bool s, bool w, bool o) {
	if (a)
		cout << "-a\t\tenable auto-boot and restart the phone" << endl;
	if (c)
		cout << "-c <arg>\tsend a command to iBoot/iBSS" << endl;
	if (u)
		cout << "-u <arg>\tupload a file" << endl;
	if (e)
		cout << "-e <arg>\tupload a file and execute usb control exploit (0x21)" << endl;
	if (s)
		cout << "-s\t\tstart an interactive shell with iBoot/iBSS" << endl;
	if (w)
		cout << "-w\t\tshow warranty information" << endl;
	if (o)
		cout << "-o\t\tshow conditions" << endl;
}
bool init() {
	if (! MobileDevice.Connect()) {
		cout << "[Device] Failed to Connect." << endl;
		return false;
	}

	cout << "[Device] Connected." << endl;

	(void) signal(SIGTERM, shutdown);
	(void) signal(SIGQUIT, shutdown);
	(void) signal(SIGINT, shutdown);

	return true;
}

void shutdown(int sig) {

	MobileDevice.Disconnect();
	exit(1);

}

int setup(int argc, char *argv[]) {


	if (argc == 1) {
		info(true, true, true, true, true, true, true);
		return -1;
	}

	if (! load_plugins())
		return -1;

	int c;

	while ((c = getopt (argc, argv, "ac:u:e:swoh:?:")) != -1) {

		switch(c) {

		case 'a':
			if (! MobileDevice.IsConnected())
				if (! init()) return -1;
			if (! MobileDevice.AutoBoot()) return -1;
			return 1;
			break;

		case 'c':
			if (! MobileDevice.IsConnected())
				if (! init()) return -1;
			if (! MobileDevice.SendCommand(optarg))
				return -1;
			break;

		case 'u':
			if (! MobileDevice.IsConnected())
				if (! init()) return -1;
			if (! MobileDevice.Upload(optarg))
				return -1;
			break;

		case 'e':
			if (! MobileDevice.IsConnected())
				if (! init()) return -1;
			if (! MobileDevice.Exploit(optarg))
				return -1;
			break;

		case 's':
			if (! MobileDevice.IsConnected())
				if (! init()) return -1;
			if (! MobileDevice.Console())
				return -1;
			break;

		case 'w':
			warranty();
			break;

		case 'o':
			conditions();
			break;

		case 'h':
		case '?':
			switch((char)optarg[0]) {
			case 'a':
				info(true, false, false, false, false, false, false);
				break;

			case 'c':
				info(false, true, false, false, false, false, false);
				break;

			case 'u':
				info(false, false, true, false, false, false, false);
				break;

			case 'e':
				info(false, false, false, true, false, false, false);
				break;

			case 's':
				info(false, false, false, false, true, false, false);
				break;

			case 'w':
				info(false, false, false, false, false, true, false);
				break;

			case 'o':
				info(false, false, false, false, false, false, true);
				break;

			default:
				info(true, true, true, true, true, true, true);
				break;
			}
		break;

		default:
			info(true, true, true, true, true, true, true);
		}
	}

	if (MobileDevice.IsConnected())
		MobileDevice.Disconnect();

	return 1;
}

void warranty() {

	cout << "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\r\n";
	cout << "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\r\n";
	cout << "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\r\n";
	cout << "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\r\n";
	cout << "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\r\n";
	cout << "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\r\n";
	cout << "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\r\n";
	cout << "ALL NECESSARY SERVICING, REPAIR OR CORRECTION." << endl;
}

void conditions() {

	cout << "All rights granted under this License are granted for the term of\r\n";
	cout << "copyright on the Program, and are irrevocable provided the stated\r\n";
	cout << "conditions are met.  This License explicitly affirms your unlimited\r\n";
	cout << "permission to run the unmodified Program.  The output from running a\r\n";
	cout << "covered work is covered by this License only if the output, given its\r\n";
	cout << "content, constitutes a covered work.  This License acknowledges your\r\n";
	cout << "rights of fair use or other equivalent, as provided by copyright law.\r\n";

	cout << "You may make, run and propagate covered works that you do not\r\n";
	cout << "convey, without conditions so long as your license otherwise remains\r\n";
	cout << "in force.  You may convey covered works to others for the sole purpose\r\n";
	cout << "of having them make modifications exclusively for you, or provide you\r\n";
	cout << "with facilities for running those works, provided that you comply with\r\n";
	cout << "the terms of this License in conveying all material for which you do\r\n";
	cout << "not control copyright.  Those thus making or running the covered works\r\n";
	cout << "for you must do so exclusively on your behalf, under your direction\r\n";
	cout << "and control, on terms that prohibit them from making any copies of\r\n";
	cout << "your copyrighted material outside their relationship with you.\r\n";

	cout << "Conveying under any other circumstances is permitted solely under\r\n";
	cout << "the conditions stated below.  Sublicensing is not allowed; section 10\r\n";
	cout << "makes it unnecessary." << endl;
}


int main(int argc, char *argv[]) {

	cout << "iRecovery++  Copyright (C) 2010  GreySyntax\r\n";
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details `./iRecovery -w'.\r\n";
	cout << "This is free software, and you are welcome to redistribute it\r\n";
	cout << "under certain conditions; type `./iRecovery -o' for details.\r\n" << endl;

	return setup(argc, argv);
}

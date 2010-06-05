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

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <iostream>
#include <signal.h>

#define VERSION "1.0.7"

void info(bool a, bool c, bool u, bool e, bool s, bool w, bool o);
bool init();
void shutdown(int sig);
int setup(int argc, char *argv[]);
void help();
void warranty();
void conditions();

#endif /* PROGRAM_H_ */

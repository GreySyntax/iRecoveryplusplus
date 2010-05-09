//============================================================================
// Name        : iRecovery.cpp
// Author      : GreySyntax
// Version     : 1.0
// Copyright   : GreySyntax 2010(C)
// Description : A C++ implementation of "iRecovery 2.x"
//============================================================================

#include <iostream>
#include "headers/Device.h"

using namespace std;

#define VERSION "1.0.1"

int main() {
	cout << "iRecovery++ :: " << VERSION << endl;
	cout << "by GreySyntax" << endl << endl;
	cout << "Special thanks to posixninja" << endl << endl;

	Device MobileDevice;

	if (! MobileDevice.Connect()) {
		cout << "[Device] Failed to Connect." << endl;
		return -1;
	}

	cout << "[Device] Connected." << endl;
	MobileDevice.AutoBoot();
	MobileDevice.Disconnect();

	return 0;
}

#include "stdafx.h"
#include "pubsub.h"
#include "DummyPublisher.h"
#include "MouseController.h"
// PETALauncher.cpp : Defines the entry point for the console application.
//

VOID startup(LPCWSTR path)
{
	std::cout << "in startup\n";
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWDEFAULT);
	std::cout << "after attempt\n";
}

PubSubHandler psh;


int main()
{
	std::cout << "About to try to open optikey\n";
	startup(_T("C:\\Program Files (x86)\\OptiKey\\OptiKey.exe"));
	PubSubHandler *pubSubHandler; // no constructor right now.
	DummyPublisher pub = DummyPublisher(pubSubHandler);
	MouseController sub = MouseController(pubSubHandler);
	int x;
	int y;
	MousePosData mpd;
	EventMessage em;
	while (true) {
		std::cout << "Enter new x coord: ";
		std::cin >> x;
		std::cout << "Enter new y coord: ";
		std::cin >> y;
		mpd.x = x;
		mpd.y = y;
		em.topic = MousePos;
		em.data = &mpd;
		pub.Publish(em);
	}
    return 0;
}


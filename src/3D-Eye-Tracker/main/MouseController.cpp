#include "stdafx.h"

// Member Functions() 

void MouseController::readMessages() {
	EventMessage em = MouseController::getTopMessage();
	if (em.topic == EyeData) {
		std::vector<double> mpd = *static_cast<std::vector<double> *>(em.data);
		LPPOINT currentMousePos;
		GetCursorPos(currentMousePos);
		double x = mpd.at(0);
		double y = mpd.at(1);
		int xMax = 1920;
		int yMax = 1080;
		int xChange, yChange = 0;
		if (x > .2 && currentMousePos->x < xMax - 10) {
			xChange = 10;
		} else if (x < -.2 && currentMousePos->x > 10) {
			xChange = -10;
		} else if (y > .2 && currentMousePos->y < yMax - 10) {
			yChange = 10;
		} else if (y < -.2 && currentMousePos->y > 10) {
			yChange = -10;
		}

		setCursorPosition(currentMousePos->x + xChange, currentMousePos->y + yChange);
	} else if (em.topic == MousePos) {
		MousePosData * mpd = static_cast<MousePosData *>(em.data);
		setCursorPosition(mpd->x, mpd->y);
	}
}

void MouseController::setCursorPosition(int x, int y)
{
	SetCursorPos(x, y);
}
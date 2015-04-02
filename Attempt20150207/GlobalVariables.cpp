#include "GlobalVariables.h"

const Vec3b GlobalVariables::COLOR_OFFAL_BACKGROUND = Vec3b(255, 255, 255);
const Vec3b GlobalVariables::COLOR_OTHERS = Vec3b(0, 255, 255);
const Vec3b GlobalVariables::COLOR_HEART = Vec3b(255, 0, 0);
const Vec3b GlobalVariables::COLOR_LUNGS = Vec3b(0, 255, 0);
const Vec3b GlobalVariables::COLOR_DIAPHRAGM = Vec3b(255, 255, 0);
const Vec3b GlobalVariables::COLOR_LIVER = Vec3b(0, 0, 255);

int GlobalVariables::getOrganIndex(Organ organ) {
	if (organ == OTHERS) {
		return 0;
	}
	else if (organ == HEART) {
		return 1;
	}
	else if (organ == LUNGS) {
		return 2;
	}
	else if (organ == DIAPHRAGME) {
		return 3;
	}
	else if (organ == LIVER) {
		return 4;
	}
}

GlobalVariables::Organ GlobalVariables::getOrganByColor(Vec3b color) {
	if (color == GlobalVariables::COLOR_OTHERS) {
		return OTHERS;
	}
	else if (color == GlobalVariables::COLOR_LIVER) {
		return LIVER;
	}
	else if (color == GlobalVariables::COLOR_LUNGS) {
		return LUNGS;
	}
	else if (color == GlobalVariables::COLOR_DIAPHRAGM) {
		return DIAPHRAGME;
	}
	else if (color == GlobalVariables::COLOR_HEART) {
		return HEART;
	}
	return Organ::UNDEFINED;
}

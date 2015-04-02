#ifndef GLOBAL_VARIABLES_CUSTOM
#define GLOBAL_VARIABLES_CUSTOM
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

static class GlobalVariables {
public:
	static enum Organ { OTHERS = 1, HEART = 2, LUNGS = 4, DIAPHRAGME = 8, LIVER = 16, UNDEFINED = 32 };

static int getOrganIndex(Organ);
static Organ getOrganByColor(Vec3b color);

static const Vec3b COLOR_OFFAL_BACKGROUND;
static const Vec3b COLOR_OTHERS;
static const Vec3b COLOR_HEART;
static const Vec3b COLOR_LUNGS;
static const Vec3b COLOR_DIAPHRAGM;
static const Vec3b COLOR_LIVER;

static friend inline Organ  operator|(Organ a, Organ b)
{
	return static_cast<Organ>(static_cast<int>(a) | static_cast<int>(b));
}

};

#endif
#ifndef GLOBAL_VARIABLES_CUSTOM
#define GLOBAL_VARIABLES_CUSTOM
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

static class GlobalVariables {
public:
static enum Organ { OTHERS = 0, HEART = 1, LUNGS = 2, DIAPHRAGME = 3, LIVER = 4};

static const Vec3b COLOR_OFFAL_BACKGROUND;
static const Vec3b COLOR_OTHERS;
static const Vec3b COLOR_HEART;
static const Vec3b COLOR_LUNGS;
static const Vec3b COLOR_DIAPHRAGM;
static const Vec3b COLOR_LIVER;

static const Vec3b ANNOTATED_ORGAN_COLORS[5];

};

#endif
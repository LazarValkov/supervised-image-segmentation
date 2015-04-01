#include "GlobalVariables.h"

const Vec3b GlobalVariables::COLOR_OFFAL_BACKGROUND = Vec3b(255, 255, 255);
const Vec3b GlobalVariables::COLOR_OTHERS = Vec3b(0, 255, 255);
const Vec3b GlobalVariables::COLOR_HEART = Vec3b(255, 0, 0);
const Vec3b GlobalVariables::COLOR_LUNGS = Vec3b(0, 255, 0);
const Vec3b GlobalVariables::COLOR_DIAPHRAGM = Vec3b(255, 255, 0);
const Vec3b GlobalVariables::COLOR_LIVER = Vec3b(0, 0, 255);

const Vec3b GlobalVariables::ANNOTATED_ORGAN_COLORS[5] =
{ COLOR_OTHERS, COLOR_HEART, COLOR_LUNGS, COLOR_DIAPHRAGM, COLOR_LIVER };

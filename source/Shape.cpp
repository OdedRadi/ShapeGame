#pragma once
#include "stdafx.h"
#include"Shape.h"

IMPLEMENT_SERIAL (Shape, CObject, 1)
IMPLEMENT_SERIAL (MPolygon, CObject, 1)
IMPLEMENT_SERIAL (MSquare, CObject, 1)
IMPLEMENT_SERIAL (MParall, CObject, 1)
IMPLEMENT_SERIAL (MTriangle, CObject, 1)
IMPLEMENT_SERIAL (MCircle, CObject, 1)


Shape::Shape(void){}

Shape::~Shape(void){}

MPolygon::MPolygon(){}
MSquare::MSquare(){}
MParall::MParall(){}
MTriangle::MTriangle(){}
MCircle::MCircle(){}
#pragma once
#include"Shape.h"

class Command: public CObject
{
public:
	Command(Shape *s, int i)
	{
		shape=s;
		shapeIndex=i;
	}

	~Command()
	{
		delete shape;	
	}

	Shape* getShape()
	{
		return shape;
	}

	int getIndex()
	{
		return shapeIndex;
	}

private:
	Shape *shape;
	int shapeIndex;

};
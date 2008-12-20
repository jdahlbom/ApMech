/*
 *  mech.cpp
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 20.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "mech.h"

Mech::Mech() {
	this->x = 0;
	this->y = 0;
}

Mech::~Mech() {
}


void Mech::setLocation(int x, int y)
{
	this->x = x;
	this->y = y;
}

int Mech::getX()
{
	return this->x;
}

int Mech::getY()
{
	return this->y;
}

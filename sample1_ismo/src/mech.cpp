/*
 *  mech.cpp
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 20.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "mech.h"

Mech::Mech(int id): GameObject(id) {
}

Mech::~Mech() {
}

int Mech::serialize(uint8_t buffer[], int start, int buflength) {
	return 0;
}
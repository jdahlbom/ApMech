#include "mech.h"

Mech::Mech(int id): GameObject(id) {
}

Mech::~Mech() {
}

int Mech::serialize(uint8_t buffer[], int start, int buflength) {
	return 0;
}
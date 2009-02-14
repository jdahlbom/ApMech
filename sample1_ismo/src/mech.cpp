#include "mech.h"

Mech::Mech(int id): GameObject(id) {
}

Mech::~Mech() {
}

int Mech::serialize(unsigned char buffer[], int start, int buflength) {
	return 0;
}

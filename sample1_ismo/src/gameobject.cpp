/*
 *  gameobject.cpp
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 21.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameobject.h"

typedef std::pair<GameObject *, int> GoIdPair;
typedef std::pair<int, GameObject *> IdGoPair;

// allocate storage for the maps

std::map<GameObject *, int> *GameObject::go_to_id = NULL;
std::map<int, GameObject *> *GameObject::id_to_go = NULL;
// int GameObject::refcount = 0;

void GameObject::init_maps()
{
	if (GameObject::go_to_id == NULL)
		GameObject::go_to_id = new std::map<GameObject*, int>;
	if (GameObject::id_to_go == NULL)
		GameObject::id_to_go = new std::map<int, GameObject*>;
	
	// GameObject::refcount++;
	return;
}

void GameObject::deinit_maps()
{
	// GameObject::refcount--;
	if (GameObject::id_to_go->empty()) {
		delete GameObject::id_to_go;
		GameObject::id_to_go = NULL;
	}
	if (GameObject::go_to_id->empty()) {
		delete GameObject::go_to_id;
		GameObject::go_to_id = NULL;
	}
}


GameObject::GameObject(int id): visible(false), x(0), y(0), z(0), id(id) {

	GameObject::init_maps();

	GameObject::go_to_id->insert(GoIdPair(this, id));
	GameObject::id_to_go->insert(IdGoPair(id, this));

}

GameObject::~GameObject()
{
	GameObject::go_to_id->erase(this);
	GameObject::id_to_go->erase(this->id);
	
	GameObject::deinit_maps();
}
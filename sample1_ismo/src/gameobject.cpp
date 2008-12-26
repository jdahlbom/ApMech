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

void GameObject::initMaps()
{
    if (GameObject::go_to_id == NULL)
        GameObject::go_to_id = new std::map<GameObject*, int>;
    if (GameObject::id_to_go == NULL)
        GameObject::id_to_go = new std::map<int, GameObject*>;

    // GameObject::refcount++;
    return;
}

void GameObject::deinitMaps()
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


GameObject::GameObject(int id): visible(false), x(0), y(0), z(0), id(id), node(NULL) {

    GameObject::initMaps();
    std::stringstream stream;


    GameObject::go_to_id->insert(GoIdPair(this, id));
    GameObject::id_to_go->insert(IdGoPair(id, this));

    stream << id;
    id_s = stream.str();
}

GameObject::~GameObject()
{
    GameObject::go_to_id->erase(this);
    GameObject::id_to_go->erase(this->id);

    GameObject::deinitMaps();
}

bool GameObject::isWorld()
{
    return false;
}

GameObject * GameObjectId::get() {

    GameObject *ret = NULL;

    std::map<int, GameObject *>::iterator iter;
    iter = GameObject::id_to_go->find(id);

    if (iter != GameObject::id_to_go->end()) {
        ret = (*iter).second;
    }

    return ret;
}

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


GameObject::GameObject(int id): visible(false), id(id), node(NULL) {
    //location_ = Vector3(Real(750.0), Real(0.0), Real(750.0));
    location_ = Vector3::ZERO;
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

bool GameObject::isDirty() const { return dirty; }
void GameObject::setDirty(bool state) { dirty = state; }
bool GameObject::isVisible() const { return visible; }
void GameObject::setVisible(bool state) { visible = state; }
bool GameObject::isPlayer() const { return player; }
void GameObject::setPlayer(bool state) { player = state; }

bool GameObject::isWorld() const { return false; }

Ogre::SceneNode *GameObject::getSceneNode() const { return node; }
void GameObject::setSceneNode(Ogre::SceneNode *node) { this->node = node; }

void GameObject::setLocation(Vector3 location) { this->location_ = location; }
Vector3 GameObject::getLocation() const { return this->location_; }


GameObject * GameObjectId::get() {

    GameObject *ret = NULL;

    std::map<int, GameObject *>::iterator iter;
    iter = GameObject::id_to_go->find(id);

    if (iter != GameObject::id_to_go->end()) {
        ret = (*iter).second;
    }

    return ret;
}

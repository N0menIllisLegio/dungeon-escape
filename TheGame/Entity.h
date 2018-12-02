#ifndef ENTITY_H
#define ENTITY_H

#include <Windows.h>
#include "BlockType.h"

class Entity
{
public:
    Entity(bool isDrawable, double x, double y);
    virtual ~Entity();
    
    double x;
    double y;
    
    BlockType resource;
    const bool IsDrawable;
};
#endif
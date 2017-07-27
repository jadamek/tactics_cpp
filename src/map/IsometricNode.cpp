#include "IsometricNode.h"

IsometricNode::IsometricNode() :
    dirty_(false){}

IsometricNode::~IsometricNode()
{

}

void IsometricNode::update()
{
    dirty_ = true;
}
/*
  ==============================================================================

    Parallel.cpp
    Created: 29 Apr 2021 4:48:17pm
    Author:  Stef

  ==============================================================================
*/

#include "Parallel.h"

Parallel::Parallel() : ThreePortComponent("par.svg")
{
    
}

wdfTreeNode * Parallel::createWDFComponent() {
    if(leftChild != nullptr && rightChild != nullptr){
        treeNode.reset(new wdfTerminatedParallel(leftChild->createWDFComponent(),rightChild->createWDFComponent()));
    }
    else return nullptr;
    return treeNode.get();
}

ComponentType Parallel::getComponentType() {
    return ComponentType::A_PAR;
}


/*
  ==============================================================================

    Series.cpp
    Created: 29 Apr 2021 4:48:09pm
    Author:  Stef

  ==============================================================================
*/

#include "Series.h"

Series::Series() : ThreePortComponent("series.svg")
{
    
}

wdfTreeNode * Series::createWDFComponent() {
    if(leftChild != nullptr && rightChild != nullptr){
        treeNode.reset(new wdfTerminatedSeries(leftChild->createWDFComponent(),rightChild->createWDFComponent()));
    }
    else return nullptr;
    return treeNode.get();
}

ComponentType Series::getComponentType() {
    return ComponentType::A_SER;
}


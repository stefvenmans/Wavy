/*
  ==============================================================================

    RNodeRootComponent.h
    Created: 29 Apr 2021 4:50:18pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "CircuitComponent.h"
#include "AdaptedLeafComponent.h"

class RNodeRootComponent : public CircuitComponent
{
public:
    RNodeRootComponent();
    void paint (juce::Graphics& g) override;
    void connect(CircuitComponent* c) override ;
    int getCollums();
    int getRows();
    int getNumChilds();
    std::vector<wdfTreeNode*> getChildsWDFTreeNodes ();
    void mouseDoubleClick(const juce::MouseEvent & e) override;
    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    int getXGrid(int x);
    int getYGrid(int y);
    mat calculateScatteringMatrix(); //Redundant -> to be removed
    mat calculateScatteringMatrix(double *Rp);
    void createNullorStamps();
    ComponentType getComponentType() override;
private:
    std::vector<AdaptedLeafComponent*> childs;
    AdaptedLeafComponent** childsA;
    int rowsY;
    int collumsX;
    bool nodeDrawView = false;
    
    juce::OwnedArray<juce::Path> wires;
    juce::Point<float> startPoint;
    juce::Point<float> endPoint;
    juce::Point<float> startOfWire;
    juce::Point<float> * lastStartPoint;
    juce::OwnedArray<juce::Point<float>> interconnectionPoints;
    bool isDrawingWire = false;
    bool isDragging = false;
    int collums;
    int rows;
    int borderOffset = 32;
    int gridSize;
    juce::OwnedArray<juce::Point<float>> portConnectionPoints;
    std::vector<int> resistorStampIndexes;
    std::vector<int> voltageSourceStampIndexes;
    std::vector<int> nullorStampIndexes;
    std::vector<int> wireNodeIndexes;
    int numberOfNodes = 0;
    juce::TextButton printIndexButton;
    mat Smat;
    int xyOff = 11;
    bool isCreatingNullor = false;
    enum NullorNodeSelectionState {I, J, K, L};
    NullorNodeSelectionState nullorNodeSelectionState;
};

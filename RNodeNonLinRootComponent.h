/*
  ==============================================================================

    RNodeNonLinRootComponent.h
    Created: 2 May 2021 10:02:16pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "CircuitComponent.h"
#include "AdaptedLeafComponent.h"
#include "NonLinearComponent.h"

class RNodeNonLinRootComponent : public CircuitComponent
{
public:
    RNodeNonLinRootComponent();
    void paint (juce::Graphics& g) override;
    int connect(CircuitComponent* c) override ;
    int getCollums() override;
    int getRows() override;
    int getNumChilds();
    int getNumRoots();
    std::vector<wdfTreeNode*> getChildsWDFTreeNodes ();
    std::vector<int> getNLlist();
    void mouseDoubleClick(const juce::MouseEvent & e) override;
    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseMove(const juce::MouseEvent& e) override;
    int getXGrid(int x);
    int getYGrid(int y);
    mat calculateScatteringMatrix(); //Redundant -> to be removed
    mat calculateScatteringMatrix(matData* rootMatrixData, double *Rp);
    void createNullorStamps();
    ComponentType getComponentType() override;
    int getIndexOfPortOrientation(int o);
private:
    std::vector<AdaptedLeafComponent*> childs;
    std::vector<NonLinearComponent*> roots;
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
    mat Smat;
    int xyOff = 11;
    bool isCreatingNullor = false;
    enum NullorNodeSelectionState {I, J, K, L};
    NullorNodeSelectionState nullorNodeSelectionState;
};

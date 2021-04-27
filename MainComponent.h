#pragma once

#include <JuceHeader.h>
#include <functional>
#include <stdlib.h>
#include "../rt-wdf_lib/Libs/rt-wdf/rt-wdf.h"


enum ComponentType {
    L_RES,L_CAP,L_IND,L_VOL,L_CUR,A_INV,A_SER,A_PAR,SR_VOL,SR_CUR,SR_SHC,NOT_SET
};



class WdfEnvironment : public wdfTree
{
public:
    WdfEnvironment(){
        subtreeCount = 1;
        subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
    }
    void setInputValue( double signalIn ){
        
    }
    double getOutputValue( ){
        
    }
    int setRootMatrData( matData* rootMatrixData, double *Rp ){
        for ( unsigned int ii = 0; ii < Smat.n_cols; ++ii ) {
                for ( unsigned int jj = 0; jj < Smat.n_rows; ++jj ) {
                    rootMatrixData->Smat.at(ii, jj) = Smat.at(ii,jj);
                }
        }
    }
    const char* getTreeIdentifier( ){
        
    }
    void setParam( size_t paramID, double paramValue ){
        
    }
    
    void setSubtreeEntryNodes(wdfTreeNode * node){
        subtreeEntryNodes[0] = node;
    }
    
    void setSubtreeEntryNodes(std::vector<wdfTreeNode*> nodes){
        subtreeCount = nodes.size();
        subtreeEntryNodes = new wdfTreeNode*[subtreeCount];
        for(auto i=0; i<nodes.size(); i++){
            subtreeEntryNodes[i] = nodes[i];
        }
        root.reset(new wdfRootRtype(subtreeCount));
        Rp = new double[subtreeCount]();
    }
    
    void setRoot(wdfRootNode * r){
        root.reset(new wdfRootSimple(r));
        Rp = new double[subtreeCount]();
    }
    
    void addParam(paramData p){
        params.push_back(p);
    }
    
    void setSMat(mat newSMat){
        Smat = newSMat;
    }
private:
    mat Smat;
};


// Poly //
class CircuitComponent : public juce::Component
{
public:
    CircuitComponent(juce::String svgFileName) : svgFileName{svgFileName}
    {
        auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
        svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
        if (svgDrawable != nullptr)
        {
            if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
                svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
            }
        }
    }
    
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
    }
    
    void paintSVG(juce::Graphics& g)
    {
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        
        svgDrawable->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    }
    
    void resized() override
    {
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    
    void mouseDown(const juce::MouseEvent& e) override
    {
        lastX = getX();
        lastY = getY();
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
            
            for(auto &o: portOrientations){
                o++;
                if(o > 3) o = 0;
            }
            
        }
        else if(e.mods.isCommandDown()){
            shooldDrag = false;
            if(propertyPanelCallback != nullptr){
                propertyPanelCallback(this);
            }
        }
        
        else dragger.startDraggingComponent (this, e);
    }
    
    void mouseDrag(const juce::MouseEvent& e) override
    {
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
        // should check if component moved
        if(getX()!=lastX || getY()!=lastY){
            for(auto c: isConnected){
                c = false;
            }
            // get all child components and diconnect them
            std::cout << "disconnect all" << std::endl;
            repaint();
        }
        
    }
    
    void mouseUp(const juce::MouseEvent& e) override
    {
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
        
        
        
        if(callBack != NULL){
            if(callBack(this)){
                //isConnected = true;
            }
        }
        
        
    }
    
    void addHandler(std::function<bool(CircuitComponent* c)> clbk)
    {
        callBack = clbk;
    }
    
    float getRotationX()
    {
        return std::cos(-angle);
    }
    
    float getRotationY()
    {
        return std::sin(angle);
    }
    
    virtual void connect(CircuitComponent* c){
        
    }
    
    bool hasOrientation(int orientation){
        for(auto &o: portOrientations){
            if(o == orientation) return true;
        }
        return false;
    }
    
    virtual ComponentType getComponentType(){
        return ComponentType::NOT_SET;
    }
    
    void setPropertyPanelCallback(std::function<void(CircuitComponent* c)> callbackFunction){
        propertyPanelCallback = callbackFunction;
    }
    
protected:
    juce::String svgFileName;
    std::unique_ptr<juce::Drawable> svgDrawable;
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    std::function<bool(CircuitComponent* c)> callBack;
    std::function<void(CircuitComponent* c)> propertyPanelCallback;
    std::vector<int> portOrientations;
    int sizeX;
    int sizeY;
    std::vector<bool> isConnected;
    int lastX;
    int lastY;
    
};


class AdaptedLeafComponent : public CircuitComponent
{
public:
    AdaptedLeafComponent(juce::String svgFileName) : CircuitComponent(svgFileName)
    {
        
        
    }
    
    
    virtual wdfTreeNode* createWDFComponent(){
        return treeNode.get();
    }
    virtual wdfTreeNode* getWDFComponent(){
        return treeNode.get();
    }
    
    
    
protected:
    std::unique_ptr<wdfTreeNode> treeNode;
    bool isAdapted = true;
};

class OnePortComponent : public AdaptedLeafComponent
{
public:
    OnePortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName)
    {
        portOrientations.push_back(1);
        isConnected.push_back(false);
    }
     
    void paint(juce::Graphics& g) override
    {
        paintSVG(g);
        if(isAdapted){
            wBLine = juce::Line<float>(80,20, 90+ 10.0f, 20);
        }
        else{
            wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
        }
        auto wALine = juce::Line<float>(100,80, 90, 80);
        auto wBAdapted = juce::Line<float>(80,15,80,25);
        
        wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wBAdapted.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        g.drawArrow(wBLine, 1.5, 10,4);
        g.drawArrow(wALine, 1.5, 10,4);
        g.drawLine(wBAdapted,1.5);
        
        for(auto c : isConnected){
            if(c){
                g.setColour(juce::Colours::red);
                g.drawRect(0,0,100,100,5);
            }
        }
        
        //std::cout << "port orientation" << portOrientations[0] << std::endl;
    }
    
    void connect(CircuitComponent* c) override {
        //Check if at right side
        auto index = 0;
        bool connectSuccesfull = false;
        for(auto &o : portOrientations){
            switch(o){
                case 0:
                    //Check if component is above + has orientation 2
                    if(c->getY() + c->getHeight() == getY() && c->hasOrientation(2)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 1:
                    //Check if component is right + has orientation 3
                    if(getX() + getWidth() == c->getX() && c->hasOrientation(3)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 2:
                    //Check if component is under + has orientation 0
                    if(getY() + getHeight() == c->getY() && c->hasOrientation(0)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 3:
                    //Check if component is left + has orientation 1
                    if(c->getX() + c->getWidth() == getX() && c->hasOrientation(1)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
            }
            if(connectSuccesfull){
                isConnected[index] = true;
                return;
            }
            index++;
        }
    }
    
protected:
    juce::Line<float> wBLine;
    
    
};

class Resistor_ : public OnePortComponent
{
public:
    Resistor_() : OnePortComponent("res.svg")
    {
        R = 1;
    }
    
    wdfTreeNode* createWDFComponent() override{
        treeNode.reset(new wdfTerminatedRes(R));
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::L_RES;
    }
    
    void setR(double newR){
        R = newR;
    }
    
    double getR(){
        return R;
    }
    
private:
    double R;
};

class Capacitor_ : public OnePortComponent
{
public:
    Capacitor_() : OnePortComponent("cap.svg")
    {
        C = 1;
        T = 1;
    }
    
    wdfTreeNode* createWDFComponent() override{
        treeNode.reset(new wdfTerminatedCap(C,T));
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::L_CAP;
    }
    
    void setC(double newC){
        C = newC;
    }
    
    double getC(){
        return C;
    }
    
    void setT(double newT){
        T = newT;
    }
    
    double getT(){
        return T;
    }
    
private:
    double C;
    double T;
    
};

class VoltageSource_ : public OnePortComponent
{
public:
    VoltageSource_() : OnePortComponent("vol.svg")
    {
        Vs = 1;
        Rs = 1;
    }
    
    wdfTreeNode* createWDFComponent() override{
        treeNode.reset(new wdfTerminatedResVSource(Vs,Rs));
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::L_VOL;
    }
    
    void setVs(double newVs){
        Vs = newVs;
    }
    
    double getVs(){
        return Vs;
    }
    
    void setRs(double newRs){
        Rs = newRs;
    }
    
    double getRs(){
        return Rs;
    }
    
private:
    double Vs;
    double Rs;
    
};

class TwoPortComponent : public AdaptedLeafComponent
{
public:
    TwoPortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName), child(nullptr)
    {
        portLines.push_back(juce::Line<float>(10,80, 0, 80));
        portLines.push_back(juce::Line<float>(0,20, 10.0f, 20));
        portLines.push_back(juce::Line<float>(100,80, 90, 80));
        portLines.push_back(juce::Line<float>(90,20, 90+ 10.0f, 20));
        
        portOrientations.push_back(1);
        portOrientations.push_back(3);
        
        isConnected.push_back(false);
        isConnected.push_back(false);
    }
    
    void paint(juce::Graphics& g) override
    {
        paintSVG(g);
        
        for(auto l : portLines){
            l.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
            g.drawArrow(l, 1.5, 10,4);
           
         }
        
    }
    
    void connectChild(AdaptedLeafComponent* newChild){
        child = newChild;
    }
    
    void disconnectChild(){
        child = nullptr;
    }
    
    void connect(CircuitComponent* c) override {
        //Check if at right side
        auto index = 0;
        bool connectSuccesfull = false;
        for(auto &o : portOrientations){
            switch(o){
                case 0:
                    //Check if component is above + has orientation 2
                    if(c->getY() + c->getHeight() == getY() && c->hasOrientation(2)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 1:
                    //Check if component is right + has orientation 3
                    if(getX() + getWidth() == c->getX() && c->hasOrientation(3)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 2:
                    //Check if component is under + has orientation 0
                    if(getY() + getHeight() == c->getY() && c->hasOrientation(0)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 3:
                    //Check if component is left + has orientation 1
                    if(c->getX() + c->getWidth() == getX() && c->hasOrientation(1)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
            }
            if(connectSuccesfull){
                isConnected[index] = true;
                if(index==0){
                    child = (AdaptedLeafComponent*)c;
                    std::cout << "child set" << std::endl;
                }
                return;
            }
            index++;
        }
    }
    
protected:
    AdaptedLeafComponent* child;
    std::vector<juce::Line<float>> portLines;
    
};

class Inverter_ : public TwoPortComponent
{
public:
    Inverter_() : TwoPortComponent("inv.svg")
    {
        
    }
    
    wdfTreeNode* createWDFComponent() override{
        treeNode.reset(new wdfInverter(child->createWDFComponent()));
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::A_INV;
    }
};

class YParameterAmp : public TwoPortComponent
{
public:
    YParameterAmp() : TwoPortComponent("y-par-amp.svg")
    {
        auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
        svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
        if (svgDrawable != nullptr)
        {
            if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
                svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 180.0f, 80.0f });
            }
        }
    }
    
    wdfTreeNode* createWDFComponent() override{
        treeNode.reset(new wdfInverter(child->createWDFComponent()));
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::A_INV;
    }
    
    void paint(juce::Graphics& g)  override
    {
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
            this->setSize(getHeight(),getWidth());
            auto svgFile = juce::File::getCurrentWorkingDirectory().getChildFile(svgFileName);
            svgDrawable = juce::Drawable::createFromSVGFile(svgFile);
            if (svgDrawable != nullptr)
            {
                if (auto svgDrawableComposite = dynamic_cast<juce::DrawableComposite*> (svgDrawable.get())){
                    svgDrawableComposite->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), static_cast<float>(getWidth()-20), static_cast<float>(getHeight()-20) });
                }
            }
        }
        
        //this->setTransform(juce::AffineTransform::rotation(angle));   //getTransform().rotated(angle, getWidth()/2, getHeight()/2);
        svgDrawable->draw (g, getAlpha(),getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        std::cout << "paint called" << std::endl;
    }
};

class ThreePortComponent : public AdaptedLeafComponent
{
public:
    ThreePortComponent(juce::String svgFileName) : AdaptedLeafComponent(svgFileName), leftChild(nullptr), rightChild(nullptr)
    {
        portLines.push_back(juce::Line<float>(10,80, 0, 80));
        portLines.push_back(juce::Line<float>(0,20, 10.0f, 20));
        portLines.push_back(juce::Line<float>(80,0, 80, 10));
        portLines.push_back(juce::Line<float>(20,10, 20, 0));
        portLines.push_back(juce::Line<float>(100,80, 90, 80));
        portLines.push_back(juce::Line<float>(90,20, 90+ 10.0f, 20));
        
        portOrientations.push_back(1);
        portOrientations.push_back(3);
        portOrientations.push_back(0);
        
        isConnected.push_back(false);
        isConnected.push_back(false);
        isConnected.push_back(false);
    }
    
    void paint(juce::Graphics& g) override
    {
        paintSVG(g);
        
        for(auto l : portLines){
            l.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
            g.drawArrow(l, 1.5, 10,4);
           
         }
    }
    
    void connectLeftChild(AdaptedLeafComponent* newLeftChild){
        leftChild = newLeftChild;
    }
    
    void connectRightChild(AdaptedLeafComponent* newRightChild){
        rightChild = newRightChild;
    }
    
    void disconnectLeftChild(){
        leftChild = nullptr;
    }
    
    void disconnectRightChild(){
        rightChild = nullptr;
    }
    
    void connect(CircuitComponent* c) override {
        //Check if at right side
        auto index = 0;
        bool connectSuccesfull = false;
        for(auto &o : portOrientations){
            switch(o){
                case 0:
                    //Check if component is above + has orientation 2
                    if(c->getY() + c->getHeight() == getY() && c->hasOrientation(2)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 1:
                    //Check if component is right + has orientation 3
                    if(getX() + getWidth() == c->getX() && c->hasOrientation(3)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 2:
                    //Check if component is under + has orientation 0
                    if(getY() + getHeight() == c->getY() && c->hasOrientation(0)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 3:
                    //Check if component is left + has orientation 1
                    if(c->getX() + c->getWidth() == getX() && c->hasOrientation(1)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
            }
            if(connectSuccesfull){
                isConnected[index] = true;
                if(index==0){
                    rightChild = (AdaptedLeafComponent*)c;
                    std::cout << "right child set" << std::endl;
                }
                else if(index==1){
                    leftChild = (AdaptedLeafComponent*)c;
                    std::cout << "left child set" << std::endl;
                }
                return;
            }
            index++;
        }
    }
    
protected:
    AdaptedLeafComponent* leftChild;
    AdaptedLeafComponent* rightChild;
    std::vector<juce::Line<float>> portLines;
};

class Series_ : public ThreePortComponent
{
public:
    Series_() : ThreePortComponent("series.svg")
    {
        
    }
    
    wdfTreeNode * createWDFComponent() override{
        if(leftChild != nullptr && rightChild != nullptr){
            treeNode.reset(new wdfTerminatedSeries(leftChild->createWDFComponent(),rightChild->createWDFComponent()));
        }
        else return nullptr;
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::A_SER;
    }
};

class Parallel_ : public ThreePortComponent
{
public:
    Parallel_() : ThreePortComponent("par.svg")
    {
        
    }

    wdfTreeNode * createWDFComponent() override{
        if(leftChild != nullptr && rightChild != nullptr){
            treeNode.reset(new wdfTerminatedParallel(leftChild->createWDFComponent(),rightChild->createWDFComponent()));
        }
        else return nullptr;
        return treeNode.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::A_PAR;
    }
};


class SimpleRootComponent : public CircuitComponent
{
public:
    SimpleRootComponent(juce::String svgFileName) : CircuitComponent(svgFileName), child(nullptr)
    {
        portOrientations.push_back(1);
        isConnected.push_back(false);
    }
    
    void paint(juce::Graphics& g) override
    {
        paintSVG(g);
//        if(isAdapted){
//            wBLine = juce::Line<float>(80,20, 90+ 10.0f, 20);
//        }
//        else{
//            wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
//        }
        wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
        
        auto wALine = juce::Line<float>(100,80, 90, 80);
        auto wBAdapted = juce::Line<float>(80,15,80,25);
        
        wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wBAdapted.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        g.drawArrow(wBLine, 1.5, 10,4);
        g.drawArrow(wALine, 1.5, 10,4);
        //g.drawLine(wBAdapted,1.5);
        
        for(auto c : isConnected){
            if(c){
                g.setColour(juce::Colours::red);
                g.drawRect(0,0,100,100,5);
            }
        }
        
        //std::cout << "port orientation" << portOrientations[0] << std::endl;
    }
    
    virtual wdfRootNode * createWDFComponent() {
        return root.get();
    }
    
    void connect(CircuitComponent* c) override {
        //Check if at right side
        auto index = 0;
        bool connectSuccesfull = false;
        for(auto &o : portOrientations){
            switch(o){
                case 0:
                    //Check if component is above + has orientation 2
                    if(c->getY() + c->getHeight() == getY() && c->hasOrientation(2)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 1:
                    //Check if component is right + has orientation 3
                    if(getX() + getWidth() == c->getX() && c->hasOrientation(3)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 2:
                    //Check if component is under + has orientation 0
                    if(getY() + getHeight() == c->getY() && c->hasOrientation(0)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
                case 3:
                    //Check if component is left + has orientation 1
                    if(c->getX() + c->getWidth() == getX() && c->hasOrientation(1)){
                        std::cout << "circuit will be able to connect to this side" << std::endl;
                        connectSuccesfull = true;
                    }
                    break;
            }
            if(connectSuccesfull){
                isConnected[index] = true;
                std::cout << "child set" << std::endl;
                child = (AdaptedLeafComponent*)c;
                return;
            }
            index++;
        }
    }
    
    AdaptedLeafComponent* getChildComponent(){
        return child;
    }
    
protected:
    AdaptedLeafComponent* child;
    std::unique_ptr<wdfRootNode> root;
    
private:
    juce::Line<float> wBLine;
    
    
};

class IdealVoltageSource_ : public SimpleRootComponent
{
public:
    IdealVoltageSource_() : SimpleRootComponent("idealvol.svg")
    {
        Vs = 1;
    }
    
    wdfRootNode * createWDFComponent() override{
        if(child != nullptr){
            root.reset(new wdfIdealVSource(Vs) );
        }
        else return nullptr;
        return root.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::SR_VOL;
    }
    
    void setVs(double newVs){
        Vs = newVs;
    }
    
    double getVs(){
        return Vs;
    }
    
private:
    double Vs;
};

class ShortCircuit_ : public SimpleRootComponent
{
public:
    ShortCircuit_() : SimpleRootComponent("shortcir.svg")
    {
        
    }
    
    wdfRootNode * createWDFComponent() override{
        if(child != nullptr){
            root.reset(new wdfShortCircuit() );
        }
        else return nullptr;
        return root.get();
    }
    
    ComponentType getComponentType() override{
        return ComponentType::SR_SHC;
    }
    
};

class NonLinearComponent : public CircuitComponent
{
    
};

class RNode_ : public CircuitComponent
{
public:
    RNode_() : CircuitComponent(""){
        portOrientations.push_back(0);
        portOrientations.push_back(0);
        portOrientations.push_back(2);
        portOrientations.push_back(2);
        portOrientations.push_back(3);
        portOrientations.push_back(3);
        isConnected.push_back(false);
        isConnected.push_back(false);
        isConnected.push_back(false);
        isConnected.push_back(false);
        isConnected.push_back(false);
        isConnected.push_back(false);
        
        //childs.reserve(6);
        childs.push_back(nullptr);
        childs.push_back(nullptr);
        childs.push_back(nullptr);
        childs.push_back(nullptr);
        childs.push_back(nullptr);
        childs.push_back(nullptr);
        
        collumsX = 2;
        rowsY = 2;
        
        childsA = new AdaptedLeafComponent*[6];
    }
    
    void paint (juce::Graphics& g) override
    {
        //g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        
        //this->setTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        ///g.setFont(juce::Font("Lucida Calligraphy",60.0f,2));
        g.setFont(juce::Font("Lucida Handwriting",60.0f,2));
        g.drawText ("R", getWidth()/2-30, getHeight()/2-30, 60, 60, juce::Justification::centred);
        
        //auto rec = juce::Rectangle<float>(getWidth()/2-30, getHeight()/2-30, 60, 60);
        
        //rec.transformedBy(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        //g.drawText ("text", rec, juce::Justification::centred);
        g.setColour(juce::Colours::black);
        g.drawRect(10,10,180,180,2);
    }
    
    void connect(CircuitComponent* c) override {
        //Check if at right side
        auto index = 0;
        bool connectSuccesfull = false;
        for(auto &o : portOrientations){
            switch(o){
                case 0:
                    //Check if component is above + has orientation 2
                    for(auto i=0 ; i<collumsX; i++){
                        if(c->getY() + c->getHeight() == getY() && c->getX() == getX() + i*(getWidth()/collumsX) && c->hasOrientation(2)){
                            std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i << std::endl;
                            connectSuccesfull = true;
                            childs.insert(childs.begin()+i,(AdaptedLeafComponent*)c);
                            childsA[i] = (AdaptedLeafComponent*)c;
                            //std::cout << "inserted at index: " << childs.begin()+i << std::endl;
                            return;
                        }
                    }
                    break;
                case 1:
                    //Check if component is right + has orientation 3
                    for(auto i=0; i<rowsY; i++){
                        if(getX() + getWidth() == c->getX() && c->getY() == getY() + i*(getHeight()/rowsY) && c->hasOrientation(3)){
                            std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i << std::endl;
                            connectSuccesfull = true;
                            //childs.insert(childs.begin()+i + 2,c);
                            return;
                        }
                    }
                    break;
                case 2:
                    //Check if component is under + has orientation 0
                    for(auto i=0 ; i<collumsX; i++){
                        if(getY() + getHeight() == c->getY() && c->getX() == getX() + i*(getWidth()/collumsX) && c->hasOrientation(0)){
                            std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i + 2 << std::endl;
                            connectSuccesfull = true;
                            childs.insert(childs.begin()+i + 2,(AdaptedLeafComponent*)c);
                            childsA[i + 2] = (AdaptedLeafComponent*)c;
                            return;
                        }
                    }
                    break;
                case 3:
                    //Check if component is left + has orientation 1
                    for(auto i=0; i<rowsY; i++){
                        if(c->getX() + c->getWidth() == getX() && c->getY() == getY() + i*(getHeight()/rowsY) && c->hasOrientation(1)){
                            std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i + 4 << std::endl;
                            connectSuccesfull = true;
                            childs.insert(childs.begin()+i + 4,(AdaptedLeafComponent*)c);
                            childsA[i + 4] = (AdaptedLeafComponent*)c;
                            return;
                        }
                    }
                    break;
            }
            if(connectSuccesfull){
                isConnected[index] = true;
//                for(auto child : childs){
//                    std::cout << child << std::endl;
//                }
                
                return;
            }
            index++;
        }
    }
    
    int getCollums(){
        return collumsX;
    }
    int getRows(){
        return rowsY;
    }
    
//    wdfRootNode * createWDFComponent(){
//        bool allChildsSet = true;
//        for(auto &c: childs){
//            if(c == nullptr) allChildsSet = false;
//        }
//        if(allChildsSet != false){
//            //root.reset(new wdfRootRtype(childs.size()) );
//        }
//        else return nullptr;
//        return root.get();
//    }
    
    int getNumChilds(){
        return childs.size();
    }
    
    std::vector<wdfTreeNode*> getChildsWDFTreeNodes (){
        std::vector<wdfTreeNode*> childsWDFTreeNodes;
        for(auto i=0; i<6; i++){
            childsWDFTreeNodes.push_back(childsA[i]->createWDFComponent());
        }
        return childsWDFTreeNodes;
    }
private:
    std::vector<AdaptedLeafComponent*> childs;
    AdaptedLeafComponent** childsA;
    int rowsY;
    int collumsX;
    
};














class Resistor : public juce::Component
{
public:
    Resistor(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("res.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
                
        }
        setName("R");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        if(isAdapted){
            wBLine = juce::Line<float>(80,20, 90+ 10.0f, 20);
        }
        else{
            wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
        }
        auto wALine = juce::Line<float>(100,80, 90, 80);
        auto wBAdapted = juce::Line<float>(80,15,80,25);
        
        wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wBAdapted.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine, 1.5, 10,4);
        g.drawArrow(wALine, 1.5, 10,4);
        g.drawLine(wBAdapted,1.5);
        //g.drawLine(line);
        
        
    }
    
    wdfTreeNode * createWDFComponent(double Rp){
        r.reset(new wdfTerminatedRes(Rp));
        return r.get();
    }
    wdfTreeNode * getWDFComponent(){
        return r.get();
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
        
        if(callBack != NULL){
            if(callBack(this)){
                //isConnected = true;
            }
        }
        
    }
    
    void addHandler(std::function<bool(juce::Component* c)> clbk){
        callBack = clbk;
    }
    
    int getRotationX(){
        std::cout << angle << std::endl;
        return -std::cos(angle);
    }
    
    int getRotationY(){
        std::cout << angle << std::endl;
        return -std::sin(angle);
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedRes> r;
    bool rotate = false;
    bool shooldDrag = true;
    bool isAdapted = true;
    float angle = 0;
    juce::Line<float> wBLine;
    
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    
    std::function<bool(Component*c)> callBack;
};

class Capacitor : public juce::Component
{
public:
    Capacitor(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("cap.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
        }
        setName("C");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        
        auto wBLine = juce::Line<float>(90,20, 90+ 10.0f, 20);
        auto wALine = juce::Line<float>(100,80, 90, 80);
        wBLine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine, 2, 10,4);
        g.drawArrow(wALine, 2, 10,4);
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    wdfTreeNode * createWDFComponent(double C){
        c.reset(new wdfTerminatedCap(C,1.0));
        return c.get();
    }
    wdfTreeNode * getWDFComponent(){
        return c.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedCap> c;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};

class Series : public juce::Component
{
public:
    Series(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("series.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()+10), static_cast<float>(getY()+10), 80.0f, 80.0f });
        }
        setName("Ser");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        auto wBLine0 = juce::Line<float>(10,80, 0, 80);
        auto wALine0 = juce::Line<float>(0,20, 10.0f, 20);
        
        auto wBLine1 = juce::Line<float>(80,0, 80, 10);
        auto wALine1 = juce::Line<float>(20,10, 20, 0);
        
        auto wBLine2 = juce::Line<float>(100,80, 90, 80);
        auto wALine2 = juce::Line<float>(90,20, 90+ 10.0f, 20);
        
        wBLine0.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine0.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine0, 2, 10,4);
        g.drawArrow(wALine0, 2, 10,4);
        
        wBLine1.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine1.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine1, 2, 10,4);
        g.drawArrow(wALine1, 2, 10,4);
        
        wBLine2.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        wALine2.applyTransform(getTransform().rotated(angle, getWidth()/2, getHeight()/2));
        g.drawArrow(wBLine2, 2, 10,4);
        g.drawArrow(wALine2, 2, 10,4);
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(wdfTreeNode * left, wdfTreeNode * right){
        //ser.reset(new wdfTerminatedSeries(left, right));
        ser.reset(new wdfTerminatedSeries(((Resistor*)(children[0]))->getWDFComponent(), ((Resistor*)(children[1]))->getWDFComponent()));
    }
    wdfTreeNode * getWDFComponent(){
        return ser.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
    }
    
    void setChild(Component* c){
        if(children.size()<2){
            children.push_back(c);
            if(children.size()==2) createWDFComponent(nullptr, nullptr);
        }
        
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedSeries> ser;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    
    std::vector<Component*> children;
};

class Parallel : public juce::Component
{
public:
    Parallel(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("series.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()), static_cast<float>(getY()), 100.0f, 100.0f });
        }
        setName("Par");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(wdfTreeNode * left, wdfTreeNode * right){
        par.reset(new wdfTerminatedParallel(left, right));
    }
    wdfTreeNode * getWDFComponent(){
        return par.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfTerminatedParallel> par;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};

class IdealVSource : public juce::Component
{
public:
    IdealVSource(){
        auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("idealvol.svg");
        svgSymbol = juce::Drawable::createFromSVGFile(file);
        if (svgSymbol != nullptr)
        {
            if (auto comp = dynamic_cast<juce::DrawableComposite*> (svgSymbol.get()))
                comp->setBoundingBox ({ static_cast<float>(getX()), static_cast<float>(getY()), 100.0f, 100.0f });
        }
        setName("Vsource");
    }
    void paint (juce::Graphics& g) override{
        g.fillAll(juce::Colours::white);
        if(rotate){
            angle += juce::MathConstants<float>::halfPi;
            if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
            rotate = false;
        }
        svgSymbol->draw (g, getAlpha(), getTransform().rotated(angle, getWidth()/2, getHeight()/2));
    }
    
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
        
    }
    
    void createWDFComponent(double Vs){
        u.reset(new wdfIdealVSource(Vs));
    }
    wdfRootNode * getWDFComponent(){
        return u.get();
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown() && e.mods.isShiftDown()){
            rotate = true;
            shooldDrag = false;
            repaint();
        }
        else dragger.startDraggingComponent (this, e);

        
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(shooldDrag == true) dragger.dragComponent (this, e, &constrainer);
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        shooldDrag = true;
    }
    
private:
    std::unique_ptr<juce::Drawable> svgSymbol;
    std::unique_ptr<wdfIdealVSource> u;
    bool rotate = false;
    bool shooldDrag = true;
    float angle = 0;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
};



class RNode : public juce::Component
{
public:
    RNode(){
//        wires.add(new juce::Path());
//        wires.getLast()->startNewSubPath(19,20);
//        wires.getLast()->lineTo(100,20);
    }
    
    void paint (juce::Graphics& g) override{
        auto area = getLocalBounds().reduced (2);

        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (area.toFloat(), 10.0f, 2.0f);

        g.setColour (findColour (juce::TextButton::textColourOffId));
        g.drawFittedText ("R1", area, juce::Justification::centred, 1);
        
        for(auto w: wires){
            float lineThickness = 2.0f;
            g.strokePath (*w, juce::PathStrokeType(lineThickness));
        }
        
        if(drawLine == true){
//            auto mousePoint = getMouseXYRelative();
//            endPoint = juce::Point<float>((float)(mousePoint.getX()),(float)mousePoint.getY());
            
            //endPoint = juce::Point<float>(getX(),getY());
            
            
        }
        g.setColour(juce::Colours::black);
        
//        if(startPoint.getX() >= 50){
//            startPoint.setX(80);
//        }
//        else startPoint.setX(20);
//
//        if(startPoint.getY() >= 50){
//            startPoint.setY(80);
//        }
//        else startPoint.setY(20);
//
//        if(endPoint.getX() >= 50){
//            endPoint.setX(80);
//        }
//        else endPoint.setX(20);
//
//        if(endPoint.getY() >= 50){
//            endPoint.setY(80);
//        }
//        else endPoint.setY(20);
//
//
//        for(auto i=0; i<linePoints.size()-1 ; i++){
//            if(i==0){
//                g.drawLine(juce::Line<float>(*linePoints.getFirst(),*linePoints.getUnchecked(1)),1.5);
//            }
//            else{ g.drawLine(juce::Line<float>(*linePoints.getUnchecked(i),*linePoints.getUnchecked(i+1)),1.5);
//            }
//        }
        
        //g.drawLine(juce::Line<float>(startPoint, endPoint), 3);
    }
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown()){
            dragger.startDraggingComponent (this, e);
            isDragging = true;
        }
        else{
            drawLine = true;
            startPoint = juce::Point<float>((float)e.getPosition().getX(),(float)e.getPosition().getY());
            
            
            float x = (float)e.getPosition().getX();
            float y = (float)e.getPosition().getY();
            
            if(x>=50){
                if(x>=100){
                    if(x>=150){
                        x = 180;
                    }
                    else x = 120;
                }
                else x = 80;
                
            }
            else x = 20;
            
            if(y>=50){
                if(y>=100){
                    if(y>=150){
                        y = 180;
                    }
                    else y = 120;
                }
                else y = 80;
                
            }
            else y = 20;
            
//            if(linePoints.isEmpty()){
//                linePoints.add(new juce::Point<float>(x, y));
//            }
//            linePoints.add(new juce::Point<float>(x, y));
            //lines.getLast->add(new juce::Point<float>(x, y));
            
            
            // New path method
            x = (float)e.getPosition().getX();
            y = (float)e.getPosition().getY();
            
            if(x>=50){
                if(x>=100){
                    if(x>=150){
                        x = 180;
                    }
                    else x = 120;
                }
                else x = 80;
                
            }
            else x = 20;
            
            if(y>=50){
                if(y>=100){
                    if(y>=150){
                        y = 180;
                    }
                    else y = 120;
                }
                else y = 80;
                
            }
            else y = 20;
            
            // is wire being drawed?
            if(isDrawingWire){
                bool lineOnPath = false;
//                for(auto w: wires){
//                    std::cout << "check if poin x: " << x << "  y: " << y << std::endl;
//                    //std::cout << "endpoint : " << w->get
//                    if(w->contains(x,y,1)){
//                        //std::cout << "point is on path" << std::endl;
//                    }
//                    if(w->intersectsLine(juce::Line<float>(x-1,y-1,x+1,y+1),0)){
//
//                        //wires.add(new juce::Path());
//                        wires.getLast()->lineTo(x,y);
//                        std::cout << "line is on path" << std::endl;
//                        lineOnPath = true;
//                        isDrawingWire = false;
//                        std::cout << "wire connected to other wire" << std::endl;
//                    }
//                }
                auto p = juce::Point<float>(x,y);
                for(auto lines: linesArray){
                    for(auto l: *lines){
                        if(p==l->findNearestPointTo(p)){
                            linesArray.getLast()->getLast()->setEnd(x,y);
                            lineOnPath = true;
                            isDrawingWire = false;
                        }
                    }
                }
                if(lineOnPath != true){
                    for(auto i=0; i<3; i++){
                        if((x==20 && y==portPoints[i]) || (x==portPoints[i] && y==20) || (x==180 && y==portPoints[i])){
                            //wires.getLast()->lineTo(x,y);
                            linesArray.getLast()->getLast()->setEnd(x,y);
                            lineOnPath = true;
                            isDrawingWire = false;
                            std::cout << "wire connected to other port" << std::endl;
                        }
                    }
                }
                if(lineOnPath != true){
                    std::cout << "makes wire longer" << std::endl;
                    linesArray.getLast()->getLast()->setEnd(x,y);
                    linesArray.getLast()->add(new juce::Line<float>(x,y,x,y));
                    //wires.getLast()->lineTo(x,y);
                }
            }
            else{
            //check if on path or if port + or -
            
            for(auto i=0; i<3; i++){
                if((x==20 && y==portPoints[i]) || (x==portPoints[i] && y==20) || (x==180 && y==portPoints[i])){
                    std::cout << "starting new wire" << std::endl;
//                    wires.add(new juce::Path());
//                    wires.getLast()->startNewSubPath(x,y);
                    linesArray.add(new juce::OwnedArray<juce::Line<float>>);
                    linesArray.getLast()->add(new juce::Line<float>(x,y,x,y));
                    isDrawingWire = true;
                }
            }
            
        }
        
    }
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        if(isDragging){
            dragger.dragComponent (this, e, &constrainer);
        }
        else{
            endPoint = juce::Point<float>((float)e.getPosition().getX(),(float)e.getPosition().getY());
            
            float x = (float)e.getPosition().getX();
            float y = (float)e.getPosition().getY();
            
            if(x>=50){
                if(x>=100){
                    if(x>=150){
                        x = 180;
                    }
                    else x = 120;
                }
                else x = 80;
                
            }
            else x = 20;
            
            if(y>=50){
                if(y>=100){
                    if(y>=150){
                        y = 180;
                    }
                    else y = 120;
                }
                else y = 80;
                
            }
            else y = 20;
            
            if(isDrawingWire){
                std::cout << "update wire" <<std::endl;
                wires.getLast()->lineTo(x,y);
                repaint();
            }
            
            //linePoints.getLast()->setXY(x,y);
            repaint();
        }
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        drawLine = true;
        isDragging = false;
    }
    
    void mouseMove(const juce::MouseEvent& e) override{
        std::cout << "dbg";
    }
    
    void mouseDoubleClick(const juce::MouseEvent & e) override{
        std::cout << "mouse dubble clicked" <<std::endl;
    }
private:
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    bool drawLine = false;
    juce::Point<float> startPoint;
    juce::Point<float> endPoint;
    juce::OwnedArray<juce::Point<float>> linePoints;
    juce::OwnedArray<juce::OwnedArray<juce::Line<float>>> linesArray;
    bool isDragging = false;
    juce::OwnedArray<juce::Path> wires;
    int portPoints[4] = {20, 80, 120, 180};
    bool isDrawingWire = false;
    
};

class RNodeNew : public juce::Component
{
public:
    RNodeNew(){
//        wires.add(new juce::Path());
//        wires.getLast()->startNewSubPath(19,20);
//        wires.getLast()->lineTo(100,20);
        collums = 2;
        rows = 2;
        setSize(200,200);
        
        gridSize = (getHeight()-2*borderOffset)/(rows*2-1);
        //North
        for(auto i=0; i<collums*2 ; i++){
            portConnectionPoints.add(new juce::Point<float>(getXGrid(borderOffset+i*gridSize),0));
        }
        //East
        for(auto i=0; i<rows*2 ; i++){
            portConnectionPoints.add(new juce::Point<float>(getWidth(),getYGrid(borderOffset+i*gridSize)));
        }
        //South
        for(auto i=0; i<collums*2 ; i++){
            portConnectionPoints.add(new juce::Point<float>(getXGrid(borderOffset+(collums*2-1-i)*gridSize),getHeight()));
        }
        //West
        for(auto i=0; i<rows*2 ; i++){
            portConnectionPoints.add(new juce::Point<float>(0,getYGrid(borderOffset+(rows*2-1-i)*gridSize)));
        }
        
        for(auto p: portConnectionPoints){
            std::cout << " point x : " << p->getX() << " y : " <<p->getY() << std::endl;
        }
        
        //Create stamp indexes (4 sides)
        for(auto i=0; i<(collums*2)*4 ; i++){
            resistorStampIndexes.push_back(-1);
            voltageSourceStampIndexes.push_back(-1);
        }
        
        addAndMakeVisible(&printIndexButton);
        printIndexButton.setBounds(30,30,10,10);
        printIndexButton.onClick = [this](){
            for(auto i=0; i<resistorStampIndexes.size()/2; i++){
                std::cout << "R" << i << " i:" << resistorStampIndexes[i*2] << " j:" << resistorStampIndexes[i*2+1] << std::endl;
            }
        };
    
    }
    
    void paint (juce::Graphics& g) override{
        auto area = getLocalBounds().reduced (2);

        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (area.toFloat(), 10.0f, 2.0f);

        g.setColour (findColour (juce::TextButton::textColourOffId));
        g.drawFittedText ("R", area, juce::Justification::centred, 1);
        
        for(auto p: portConnectionPoints){
            g.drawEllipse(p->getX()-2, p->getY()-2, 4, 4, 3);
        }
        
        g.setColour(juce::Colours::black);
        
        if(isDrawingWire){
            g.drawLine(juce::Line<float>(startPoint,endPoint),2);
        }
        
        for(auto w: wires){
            g.strokePath(*w, juce::PathStrokeType(2));
        }
        
        for(auto p: interconnectionPoints){
            g.drawEllipse(p->getX()-2, p->getY()-2, 4, 4, 3);
        }
        
    }
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
        if(e.mods.isCommandDown()){
            dragger.startDraggingComponent (this, e);
            isDragging = true;
        }
        else{
            auto x = getXGrid(e.getPosition().getX());
            auto y = getYGrid(e.getPosition().getY());
            if(isDrawingWire){
                //Check if it can connect to another wire
                for(auto w : wires){
                    if(w->intersectsLine(juce::Line<float>(x-1,y-1,x+1,y+1),0)){
                        wires.getLast()->addLineSegment(juce::Line<float>(startPoint, endPoint),0);
                        interconnectionPoints.add(new juce::Point<float>(x,y));
                        isDrawingWire = false;
                        //Add component stamp index of start startOfWire
                        
                        //Get index of startPoint
                        
                        if((portConnectionPoints.indexOf(lastStartPoint) % 2) != 0){ // Add resistor stamp
                            std::cout << "point index is : " << portConnectionPoints.indexOf(lastStartPoint) << std::endl;
                            std::cout << "F1 : component connected to node : " << wireNodeIndexes[wires.indexOf(w)] << std::endl;
                            resistorStampIndexes[portConnectionPoints.indexOf(lastStartPoint)] = wireNodeIndexes[wires.indexOf(w)];
                        }
                        else { // Add voltage source stamp
                            voltageSourceStampIndexes[portConnectionPoints.indexOf(lastStartPoint)] = wireNodeIndexes[wires.indexOf(w)];
                        }
                        wireNodeIndexes.push_back(wireNodeIndexes[wires.indexOf(w)]);
                        return;
                    }
                }
                //Check if connected to other port
                if(x == 0 || x == getWidth() || y==0 || y == getHeight()){
                    for(auto p: portConnectionPoints){
                        if(p->getX() == x && p->getY() == y){
                            wires.getLast()->addLineSegment(juce::Line<float>(startPoint, endPoint),0);
                            isDrawingWire = false;
                            //Add component stamp index (new) of startOfWire and endPoint
                            if((portConnectionPoints.indexOf(p) % 2) != 0){ // Add resistor stamp
                                std::cout << "F2 : component connected to node : " << numberOfNodes << std::endl;
                                resistorStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            }
                            else { // Add voltage source stamp
                                voltageSourceStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            }
                            wireNodeIndexes.push_back(numberOfNodes);
                            numberOfNodes++;
                            return;
                        }
                    }
                }
                else{ // Make wire longer
                    wires.getLast()->addLineSegment(juce::Line<float>(startPoint, endPoint),0);
                    startPoint.setXY(x,y);
                    endPoint.setXY(x,y);
                    return;
                }
            }
            else{
                //Check if one of the ports
                if(x == 0 || x == getWidth() || y==0 || y == getHeight()){
                    for(auto p: portConnectionPoints){
                        if(p->getX() == x && p->getY() == y){
                            isDrawingWire = true;
                            startPoint.setXY(x,y);
                            lastStartPoint = p;
                            endPoint.setXY(x,y);
                            wires.add(new juce::Path());
                            startOfWire.setXY(x,y);
                            
                            //Check if even -> (-) Uneven -> (+)
                            if((portConnectionPoints.indexOf(p) % 2) != 0){ // Add resistor stamp
                                resistorStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            }
                            else { // Add voltage source stamp
                                voltageSourceStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            }
                        }
                    }
                }
            }
            
        }
    }
    
    void mouseDrag(const juce::MouseEvent& e) override{
        if(isDragging){
            dragger.dragComponent (this, e, &constrainer);
        }
    }
    void mouseUp(const juce::MouseEvent& e) override{
        isDragging = false;
        repaint();
    }
    
    void mouseMove(const juce::MouseEvent& e) override{
        if(isDrawingWire){
            auto x = getXGrid(e.getPosition().getX());
            auto y = getYGrid(e.getPosition().getY());
            endPoint.setXY(x,y);
            repaint();
        }
    }
    
    void mouseDoubleClick(const juce::MouseEvent & e) override{
        
    }
    
    int getXGrid(int x){
        gridSize = (getWidth()-2*borderOffset)/(collums*2-1);
        //Check limits
        if(x<=borderOffset){
            if(x<borderOffset/2){
                x = 0;
            }
            else x = borderOffset;
        }
        // - 1 for roundoff
        else if(x>= getWidth() - borderOffset - 1){
            if(x>getWidth() - borderOffset/2){
                x = getWidth();
            }
            else{
                x = getWidth() - borderOffset;
            }
        }
        else if((x-borderOffset) % gridSize <= gridSize/2){
            x -= ((x-borderOffset) % gridSize);
        }
        else{
            x = x - ((x-borderOffset) % gridSize) + gridSize;
            // Check for round off fault
            if(x >= (getWidth() - borderOffset - 1)){
                x = getWidth() - borderOffset;
            }
            
        }
        return x;
    }
    
    int getYGrid(int y){
        gridSize = (getHeight()-2*borderOffset)/(rows*2-1);
        //Check limits
        if(y<=borderOffset){
            if(y<borderOffset/2){
                y = 0;
            }
            else y = borderOffset;
        }
        else if(y>= getHeight() - borderOffset - 1){
            if(y>getHeight() - borderOffset/2){
                y = getHeight();
            }
            else y = getHeight() - borderOffset;
        }
        else if((y-borderOffset) % gridSize <= gridSize/2){
            y -= ((y-borderOffset) % gridSize);
        }
        else{
            y = y - ((y-borderOffset) % gridSize) + gridSize;
            // Check for round off fault
            if(y >= (getHeight() - borderOffset - 1)){
                y = getHeight() - borderOffset;
            }
        }
        return y;
    }
private:
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
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
    int borderOffset = 20;
    int gridSize;
    juce::OwnedArray<juce::Point<float>> portConnectionPoints;
    std::vector<int> resistorStampIndexes;
    std::vector<int> voltageSourceStampIndexes;
    std::vector<int> wireNodeIndexes;
    int numberOfNodes = 0;
    juce::TextButton printIndexButton;
};


class FrontPanel : public juce::Component
{
public:
    FrontPanel(){
        
    }
    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::grey);
        g.fillAll(juce::Colours::grey);
    }
    void addNewComponent(juce::Component* c){
        int x = 0;
        ((juce::Slider*)(c))->setSliderStyle(juce::Slider::SliderStyle::Rotary);
        if(components.isEmpty()) x = 15;
        else x += components.getLast()->getX() + 115;
        ((juce::Slider*)(c))->setBounds(x,15,100,100);
        ((juce::Slider*)(c))->setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true,50,20);
        
        addAndMakeVisible(components.add(c));
    }
    void resized() override{
        
    }
    
private:
    juce::OwnedArray<juce::Component> components;
};

class DraggableComp : public juce::Component{
public:
    DraggableComp(){
        
    }

    void paint (juce::Graphics& g) override{
        auto area = getLocalBounds().reduced (2);

        g.setColour (juce::Colours::orange);
        g.drawRoundedRectangle (juce::Rectangle<float>(10,10,80,80), 10.0f, 2.0f);

        g.setColour (findColour (juce::TextButton::textColourOffId));
        g.drawFittedText ("R1", area, juce::Justification::centred, 1);
        
        g.setColour(juce::Colours::black);
        if(isConnected){
            g.drawEllipse(7.5,20,5,5,5);
            g.drawEllipse(7.5,80,5,5,5);
        }
    }
    void resized() override{
        constrainer.setMinimumOnscreenAmounts (getHeight(), getWidth(),
        getHeight(), getWidth());
    }
    void mouseDown(const juce::MouseEvent& e) override{
        dragger.startDraggingComponent (this, e);
    }
    void mouseDrag(const juce::MouseEvent& e) override{
        dragger.dragComponent (this, e, &constrainer);
        isConnected = false;
    }
    void mouseUp(const juce::MouseEvent& e) override{
        const int width = getWidth();
        const int height = getHeight();
        const int x = getX();
        const int y = getY();
        if(x % width >= width/2){
            if(getY() % height >= height/2){
                setBounds(x + width - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x + width - x % width, y - y % height, width, height);
            }
        }
        else{
            if(getY() % height >= height/2){
                setBounds(x - x % width, y + height - y % height, width, height);
            }
            else{
                setBounds(x - x % width, y - y % height, width, height);
            }
        }
        
        if(callBack != NULL){
            if(callBack(getX(),getY())){
                isConnected = true;
            }
        }
        
    
        
    }
    
    void addHandler(std::function<bool(int,int)> clbk){
        callBack = clbk;
    }
    
private:
    juce::ComponentBoundsConstrainer constrainer;
    juce::ComponentDragger dragger;
    //bool (*callback)(int,int) = NULL;
    std::function<bool(int,int)> callBack;
    bool isConnected = false;
    
    
};





class Schematic : public juce::Component
{
public:
    Schematic(){
        l1.setText("Dummy text",juce::NotificationType::dontSendNotification);
        addAndMakeVisible(l1);
        l1.setBounds((820*5)/2,(580*5)/2,500,80);
        
        addAndMakeVisible(dragComp);
        dragComp.setBounds((820*5)/2+1000,(580*5)/2, 100,100);
        
        
        
        
    }
    
    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::grey);
        g.fillAll(juce::Colours::lightgrey);
        g.drawRect(juce::Rectangle<float>(5000/2-300,5000/2,50,50),15);
        
        
        float x = 1000;
        float y = 1000;
        juce::Path sineGraph;
        
        sineGraph.startNewSubPath(x, y);
    
        auto numberOfPoints = 1000;
        for (int i = 1; i < numberOfPoints; ++i)
        {
     // calculate or optain the coordinates x and y of the current point
            
            x += 0.1;
            y = 1000.0+100.0*std::sin(x*(juce::MathConstants<double>::pi*2.0)/100.0);
            sineGraph.lineTo(x, y);
        }

        float lineThickness = 0.75f;
        g.strokePath (sineGraph, juce::PathStrokeType(lineThickness));
        
        
    }
    
    void mouseDown(const juce::MouseEvent& e) override{
           std::cout << "mouse down was called" << std::endl;
       }
    
    
    
private:
    juce::Label l1;
    DraggableComp dragComp;
};

class PropertyPanel :   public juce::Component,
                        public juce::Label::Listener,
                        public juce::Button::Listener
{
public:
    PropertyPanel()
    {
        addAndMakeVisible(componentName);
        componentName.setBounds(5,0,100,20);
        
        addChildComponent(prop1);
        prop1.setEditable(true);
        prop1.setBounds(5,30,100,20);
        prop1.addListener(this);
        
        addChildComponent(prop2);
        prop2.setEditable(true);
        prop2.setBounds(5,60,100,20);
        prop2.addListener(this);
        
        addAndMakeVisible(setOutput);
        setOutput.setBounds(5,100,25,25);
        setOutput.addListener(this);
        
        addAndMakeVisible(setInput);
        setInput.setBounds(50,100,25,25);
        setInput.addListener(this);
    }
    
    void paint (juce::Graphics& g) override{
        g.setColour(juce::Colours::darkgrey);
        g.fillAll();
    }
    
    void setPropertiesForComponent(CircuitComponent* c){
        std::cout << "propertyPanel called for : " << c->getComponentType() << std::endl;
        componentLastSelected = c;
        switch(c->getComponentType()){
            case L_RES:
                std::cout << " i am a resistor" << std::endl;
                componentName.setText("Resistor",juce::NotificationType::dontSendNotification);
                prop1.setText(juce::String(((Resistor_*)c)->getR()),juce::NotificationType::dontSendNotification);
                prop1.setVisible(true);
                break;
            case L_CAP:
                std::cout << " i am a capacitor" << std::endl;
                componentName.setText("Capacitor",juce::NotificationType::dontSendNotification);
                prop1.setText(juce::String(((Capacitor_*)c)->getC()),juce::NotificationType::dontSendNotification);
                prop1.setVisible(true);
                break;
            case L_VOL:
                componentName.setText("Resistive Voltage Source",juce::NotificationType::dontSendNotification);
                prop1.setText(juce::String(((VoltageSource_*)c)->getVs()),juce::NotificationType::dontSendNotification);
                prop1.setVisible(true);
                prop2.setText(juce::String(((VoltageSource_*)c)->getRs()),juce::NotificationType::dontSendNotification);
                prop2.setVisible(true);
                break;
            case A_INV:
                componentName.setText("Inverter",juce::NotificationType::dontSendNotification);
                prop1.setVisible(false);
                break;
            case A_SER:
                componentName.setText("Serries",juce::NotificationType::dontSendNotification);
                prop1.setVisible(false);
                break;
            case A_PAR:
                componentName.setText("Parallel",juce::NotificationType::dontSendNotification);
                prop1.setVisible(false);
                break;
                
            case SR_VOL:
                std::cout << " i am a ROOT : voltage source" << std::endl;
                componentName.setText("ROOT Voltage Source",juce::NotificationType::dontSendNotification);
                prop1.setText(juce::String(((IdealVoltageSource_*)c)->getVs()),juce::NotificationType::dontSendNotification);
                prop1.setVisible(true);
                break;
        }
        
    }
    
    void labelTextChanged (juce::Label *labelThatHasChanged) override{
        switch(componentLastSelected->getComponentType()){
            case L_RES:
                ((Resistor_*)componentLastSelected)->setR(prop1.getText().getDoubleValue());
                break;
            case L_CAP:
                ((Capacitor_*)componentLastSelected)->setC(prop1.getText().getDoubleValue());
                break;
            case L_VOL:
                if (labelThatHasChanged == &prop1){
                    ((VoltageSource_*)componentLastSelected)->setVs(prop1.getText().getDoubleValue());
                }
                else if (labelThatHasChanged == &prop2){
                    ((VoltageSource_*)componentLastSelected)->setRs(prop2.getText().getDoubleValue());
                }
                break;
            case A_INV:
                
                break;
            case A_SER:
                
                break;
            case A_PAR:
                
                break;
            case SR_VOL:
                ((IdealVoltageSource_*)componentLastSelected)->setVs(prop1.getText().getDoubleValue());
                break;
                
        }
    }
    
    void setOutputCallbackFunction(std::function<void(CircuitComponent* c)> callback){
        setOutputOfCircuit = callback;
    }
    
    void setInputCallbackFunction(std::function<void(CircuitComponent*cc)> callback){
        setInputOfCircuit = callback;
    }
    
    void buttonClicked (juce::Button * b) override {
        //std::cout << "togle state changed" << std::endl;
        if(b == &setOutput){
            if(setOutput.getToggleState()==true){
                std::cout << "output will be set" << std::endl;
                setOutputOfCircuit(componentLastSelected);
            }
        }
        else if(b == &setInput){
            if(setInput.getToggleState()==true){
                std::cout << "input will be set" << std::endl;
                setInputOfCircuit(componentLastSelected);
            }
        }
    }
    
    
    
private:
    juce::Label componentName;
    juce::Label prop1;
    juce::Label prop2;
    CircuitComponent* componentLastSelected = nullptr;
    juce::ToggleButton setOutput;
    juce::ToggleButton setInput;
    std::function<void(CircuitComponent* c)> setOutputOfCircuit;
    std::function<void(CircuitComponent* c)> setInputOfCircuit;
};




//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    //Component* getComponentToConnect(Component* child);
    bool isConnectable(int x, int y);
    bool wantsToConnect(juce::Component* c);
    bool wantsToConnect_(CircuitComponent* c);
    
    static void printSomething(){
        std::cout << "hello world" << std::endl;
    }
    
    void mouseMagnify (const juce::MouseEvent &event, float scaleFactor) override;
    
    void openPropertyPanelForComponent(CircuitComponent* c);
    
    
    void readAudio();
    void processAudio();
    void writeAudio();
    
private:
    //==============================================================================
    // Your private member variables go here...
    Resistor r1;
    Resistor r2;
    Series s1;
    IdealVSource v1;
    WdfEnvironment wdfEnvironment;
    
    FrontPanel frontPanel;
    Schematic schematic;
    
    Capacitor c1;
    Parallel p1;
    //DraggableComp draggableComp;
    juce::OwnedArray<juce::Component> components;
    juce::OwnedArray<AdaptedLeafComponent> leafComponents;
    
    juce::TextButton textButton;
    juce::Slider res1Slider;
    juce::ComboBox componentSelector;
    juce::TextButton showLibraryButton;
    juce::TextButton calculateMatButton;
    
    juce::Viewport viewPort;
    //juce::BlockField blockField;
    
   
    
    
    PropertyPanel propertyPanel;
    juce::Viewport propertyPanelViewPort;
    bool propertyPanelShowHide = false;
    
    void setOutput(CircuitComponent* c);
    void setInput(CircuitComponent* c);
    
    int outputIndex = -1;
    int inputIndex = -1;
    
    float scale = 1.0f;
    paramData res1Val;
    
    juce::SidePanel sidePanel{"Library",200,true,nullptr,false};
    
    
    Resistor_ circuitComponent{};
    std::unique_ptr<SimpleRootComponent> simpleRoot;
    std::unique_ptr<RNode_> rNode;
    std::unique_ptr<RNodeNew> rnode;
    
    juce::AudioFormatManager formatManager;
    juce::AudioSampleBuffer buffer_in;
    juce::AudioSampleBuffer buffer_out;
    juce::AudioFormatReader *reader;
    
    mat Smat;
    //Smat.set_size(6, 6);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};



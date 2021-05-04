/*
  ==============================================================================

    RNodeNonLinRootComponent.cpp
    Created: 2 May 2021 10:02:16pm
    Author:  Stef

  ==============================================================================
*/

#include "RNodeNonLinRootComponent.h"

RNodeNonLinRootComponent::RNodeNonLinRootComponent() : CircuitComponent(""){
    
    collums = 2;
    rows = 1;
    setSize(collums*componentWidth,rows*componentHeight);
    
    //North
    for(auto i=0; i<collums; i++){
        portOrientations.push_back(0);
        isConnected.push_back(false);
        roots.push_back(nullptr);
        //Create connection points
        for(auto j=0; j<2 ; j++){
            gridSize = (getWidth()-2*borderOffset)/(collums*2-1);
            portConnectionPoints.add(new juce::Point<float>(getXGrid(borderOffset+j*gridSize + i*componentWidth),11));
            resistorStampIndexes.push_back(-1);
            voltageSourceStampIndexes.push_back(-1);
        }
    }
    //East
    for(auto i=0; i<rows; i++){
        portOrientations.push_back(1);
        isConnected.push_back(false);
        childs.push_back(nullptr);
        //Create connection points
        for(auto j=0; j<2 ; j++){
            gridSize = (getHeight()-2*borderOffset)/(rows*2-1);
            portConnectionPoints.add(new juce::Point<float>(getWidth()-11,getYGrid(borderOffset+j*gridSize + i*componentHeight)));
            resistorStampIndexes.push_back(-1);
            voltageSourceStampIndexes.push_back(-1);
        }

    }
    //South
//    for(auto i=0; i<collums; i++){
//        portOrientations.push_back(2);
//        isConnected.push_back(false);
//        childs.push_back(nullptr);
//        //Create connection points
//        for(auto j=0; j<2 ; j++){
//            gridSize = (getWidth()-2*borderOffset)/(collums*2-1);
//            std::cout << " ! " << borderOffset+(collums*2-1-j)*gridSize << std::endl;
//            portConnectionPoints.add(new juce::Point<float>(getXGrid(borderOffset+(collums*2-1-j)*gridSize - i*componentWidth),getHeight()-11));
//            resistorStampIndexes.push_back(-1);
//            voltageSourceStampIndexes.push_back(-1);
//        }
//
//    }
    //West
    for(auto i=0; i<rows; i++){
        portOrientations.push_back(3);
        isConnected.push_back(false);
        childs.push_back(nullptr);
        //Create connection points
        for(auto j=0; j<2 ; j++){
            gridSize = (getHeight()-2*borderOffset)/(rows*2-1);
            portConnectionPoints.add(new juce::Point<float>(11,getYGrid(borderOffset+(rows*2-1-j)*gridSize - i*componentHeight)));
            resistorStampIndexes.push_back(-1);
            voltageSourceStampIndexes.push_back(-1);
        }
        
    }
    
    for(auto p: portConnectionPoints){
        std::cout << "p :   " << p->getX() << "-" << p->getY() << std::endl;
    }
    
}

void RNodeNonLinRootComponent::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colours::white);
    if(rotate){
        angle += juce::MathConstants<float>::halfPi;
        if(angle >= juce::MathConstants<float>::twoPi) angle = 0;
        rotate = false;
    }
    
    if(nodeDrawView){
        g.setColour(juce::Colours::black);
        for(auto p: portConnectionPoints){
            g.drawEllipse(p->getX()-1, p->getY()-1, 2, 2, 2);
        }
        
        if(isDrawingWire){
            g.drawLine(juce::Line<float>(startPoint,endPoint),2);
        }
        
        for(auto w: wires){
            g.strokePath(*w, juce::PathStrokeType(2));
        }
        
        for(auto p: interconnectionPoints){
            g.drawEllipse(p->getX()-2, p->getY()-2, 4, 4, 4);
        }
    }
    else{
        g.setFont(juce::Font("Lucida Handwriting",60.0f,2));
        g.drawText ("R", getWidth()/2-30, getHeight()/2-30, 60, 60, juce::Justification::centred);
    }
    
    
    g.setColour(juce::Colours::black);
    g.drawRect(10,10,collums*100-20,rows*100-20,2);
}

int RNodeNonLinRootComponent::getIndexOfPortOrientation(int o){
    std::vector<int>::iterator it = std::find(portOrientations.begin(), portOrientations.end(), o);
    if (it != portOrientations.end()){
        return std::distance(portOrientations.begin(), it);
    }
    else{
        return -1;
    }
}

void RNodeNonLinRootComponent::connect(CircuitComponent* c) {
    //Check if at right side
    auto index = 0;
    bool connectSuccesfull = false;
    for(auto &o : portOrientations){
        switch(o){
            case 0:
                //Check if component is above + has orientation 2
                for(auto i=0 ; i<collums; i++){
                    if(c->getY() + c->getHeight() == getY() && c->getX() == getX() + i*(getWidth()/collums) && c->hasOrientation(2)){
                        if(isRootOrNonLin() == 2){
                            std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i << std::endl;
                            connectSuccesfull = true;
                            roots[i] = (NonLinearComponent*)c;
            
                            return;
                        }
                    }
                }
                break;
            case 1:
                //Check if component is right + has orientation 3
                for(auto i=0; i<rows; i++){
                    if(getX() + getWidth() == c->getX() && c->getY() == getY() + i*(getHeight()/rows) && c->hasOrientation(3)){
                        std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i << std::endl;
                        connectSuccesfull = true;
                        childs[i+getIndexOfPortOrientation(1)] = (AdaptedLeafComponent*)c;
                        
                        return;
                    }
                }
                break;
            case 2:
                //Check if component is under + has orientation 0
                for(auto i=0 ; i<collums; i++){
                    if(getY() + getHeight() == c->getY() && c->getX() == getX() + i*(getWidth()/collums) && c->hasOrientation(0)){
                        std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i + 2 << std::endl;
                        connectSuccesfull = true;
                        childs[getIndexOfPortOrientation(2) +(collums-i-1)] = (AdaptedLeafComponent*)c;
                        
                        return;
                    }
                }
                break;
            case 3:
                //Check if component is left + has orientation 1
                for(auto i=0; i<rows; i++){
                    if(c->getX() + c->getWidth() == getX() && c->getY() == getY() + i*(getHeight()/rows) && c->hasOrientation(1)){
                        std::cout << "circuit will be able to connect to this side : " << o << "with index : " << i + 4 << std::endl;
                        connectSuccesfull = true;
                        childs[getIndexOfPortOrientation(3) +(rows-i-1)] = (AdaptedLeafComponent*)c;
                        
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

int RNodeNonLinRootComponent::getCollums(){
    return collums;
}
int RNodeNonLinRootComponent::getRows(){
    return rows;
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

int RNodeNonLinRootComponent::getNumChilds(){
    return childs.size();
}

std::vector<wdfTreeNode*> RNodeNonLinRootComponent::getChildsWDFTreeNodes (){
    std::vector<wdfTreeNode*> childsWDFTreeNodes;
    for(auto c : childs){
        childsWDFTreeNodes.push_back(c->createWDFComponent());
    }
    return childsWDFTreeNodes;
}

void RNodeNonLinRootComponent::mouseDoubleClick(const juce::MouseEvent & e){
    nodeDrawView = !nodeDrawView;
    repaint();
}

void RNodeNonLinRootComponent::mouseDown(const juce::MouseEvent &e){
    if(nodeDrawView != true){
        isDragging = true;
        this->CircuitComponent::mouseDown(e);
    }
    else if(isCreatingNullor){
        //Get node information
        
        auto x = getXGrid(e.getPosition().getX());
        auto y = getYGrid(e.getPosition().getY());
        for(auto w : wires){
            if(w->intersectsLine(juce::Line<float>(x-1,y-1,x+1,y+1),0)){
                
                switch(nullorNodeSelectionState){
                    case I:
                        nullorStampIndexes[0] = wireNodeIndexes[wires.indexOf(w)];
                        std::cout << "I set : " << wireNodeIndexes[wires.indexOf(w)] << std::endl;
                        nullorNodeSelectionState = J;
                        break;
                    case J:
                        nullorStampIndexes[1] = wireNodeIndexes[wires.indexOf(w)];
                        std::cout << "J set : " << wireNodeIndexes[wires.indexOf(w)] << std::endl;
                        nullorNodeSelectionState = K;
                        break;
                    case K:
                        nullorStampIndexes[2] = wireNodeIndexes[wires.indexOf(w)];
                        std::cout << "K set : " << wireNodeIndexes[wires.indexOf(w)] << std::endl;
                        nullorNodeSelectionState = L;
                        break;
                    case L:
                        nullorStampIndexes[3] = wireNodeIndexes[wires.indexOf(w)];
                        std::cout << "L set : " << wireNodeIndexes[wires.indexOf(w)] << std::endl;
                        nullorNodeSelectionState = I;
                        break;
                }
                return;
            }
        }
        
        //Check if unconnected node
        if(x == 11 || x == getWidth() -11 || y==11 || y == getHeight()-11){
            for(auto p: portConnectionPoints){
                if(p->getX() == x && p->getY() == y){
                    if((portConnectionPoints.indexOf(p) % 2) != 0){ // Add resistor stamp
                        if(resistorStampIndexes[portConnectionPoints.indexOf(p)] == -1){
                            resistorStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            switch(nullorNodeSelectionState){
                                case I:
                                    nullorStampIndexes[0] = numberOfNodes;
                                    std::cout << "I set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = J;
                                    break;
                                case J:
                                    nullorStampIndexes[1] = numberOfNodes;
                                    std::cout << "J set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = K;
                                    break;
                                case K:
                                    nullorStampIndexes[2] = numberOfNodes;
                                    std::cout << "K set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = L;
                                    break;
                                case L:
                                    nullorStampIndexes[3] = numberOfNodes;
                                    std::cout << "L set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = I;
                                    break;
                            }
                            
                            wireNodeIndexes.push_back(numberOfNodes);
                            numberOfNodes++;
                        }
                    }
                    else { // Add voltage source stamp
                        if(voltageSourceStampIndexes[portConnectionPoints.indexOf(p)] == -1){
                            voltageSourceStampIndexes[portConnectionPoints.indexOf(p)] = numberOfNodes;
                            switch(nullorNodeSelectionState){
                                case I:
                                    nullorStampIndexes[0] = numberOfNodes;
                                    std::cout << "I set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = J;
                                    break;
                                case J:
                                    nullorStampIndexes[1] = numberOfNodes;
                                    std::cout << "J set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = K;
                                    break;
                                case K:
                                    nullorStampIndexes[2] = numberOfNodes;
                                    std::cout << "K set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = L;
                                    break;
                                case L:
                                    nullorStampIndexes[3] = numberOfNodes;
                                    std::cout << "L set : " << numberOfNodes << std::endl;
                                    nullorNodeSelectionState = I;
                                    break;
                            }
                            wireNodeIndexes.push_back(numberOfNodes);
                            numberOfNodes++;
                        }
                        
                    }
                }
            }
        }
        
        
    }
    else{
        auto x = getXGrid(e.getPosition().getX());
        auto y = getYGrid(e.getPosition().getY());
        if(isDrawingWire){
            //Check if it can connect to another wire
            for(auto w : wires){
                if(w->intersectsLine(juce::Line<float>(x-1,y-1,x+1,y+1),0) && w != wires.getLast()){
                    wires.getLast()->addLineSegment(juce::Line<float>(startPoint, endPoint),0);
                    interconnectionPoints.add(new juce::Point<float>(x,y));
                    isDrawingWire = false;
                    //Add component stamp index of start startOfWire
                    
                    //Get index of startPoint
                    
                    if((portConnectionPoints.indexOf(lastStartPoint) % 2) != 0){ // Add resistor stamp
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
            if(x == 11 || x == getWidth() -11 || y==11 || y == getHeight()-11){
                for(auto p: portConnectionPoints){
                    if(p->getX() == x && p->getY() == y){
                        wires.getLast()->addLineSegment(juce::Line<float>(startPoint, endPoint),0);
                        isDrawingWire = false;
                        //Add component stamp index (new) of startOfWire and endPoint
                        if((portConnectionPoints.indexOf(p) % 2) != 0){ // Add resistor stamp
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
            if(x == 11 || x == getWidth()-11 || y==11 || y == getHeight()-11){
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
            
            //Check if nullor k-l
            
            //Check if nullor i-j
        }
        
    }
}

void RNodeNonLinRootComponent::mouseDrag(const juce::MouseEvent& e) {
    if(isDragging){
        this->CircuitComponent::mouseDrag(e);
    }
}
void RNodeNonLinRootComponent::mouseUp(const juce::MouseEvent& e) {
    if(isDragging){
        this->CircuitComponent::mouseUp(e);
        isDragging = false;
    }
        
    repaint();
}

void RNodeNonLinRootComponent::mouseMove(const juce::MouseEvent& e) {
    if(isDrawingWire){
        auto x = getXGrid(e.getPosition().getX());
        auto y = getYGrid(e.getPosition().getY());
        endPoint.setXY(x,y);
        repaint();
    }
}

int RNodeNonLinRootComponent::getXGrid(int x){
    gridSize = (getWidth()-2*borderOffset)/(collums*2-1);
    //Check limits
    if(x<=borderOffset){
        if(x<borderOffset/2){
            x = xyOff;
        }
        else x = borderOffset;
    }
    // - 1 for roundoff
    else if(x>= getWidth() - borderOffset - 1){
        if(x>getWidth() - borderOffset/2){
            x = getWidth()-xyOff;
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

int RNodeNonLinRootComponent::getYGrid(int y){
    gridSize = (getHeight()-2*borderOffset)/(rows*2-1);
    //Check limits
    if(y<=borderOffset){
        if(y<borderOffset/2){
            y = xyOff;
        }
        else y = borderOffset;
    }
    else if(y>= getHeight() - borderOffset - 1){
        if(y>getHeight() - borderOffset/2){
            y = getHeight()-xyOff;
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

mat RNodeNonLinRootComponent::calculateScatteringMatrix(){
    if(wireNodeIndexes.size() == 0) return nullptr;
    
    auto n = (collums+rows)*2;
    auto nInR = *(std::max_element(wireNodeIndexes.begin(),wireNodeIndexes.end())) + 1;
    auto nTotal = n + nInR;
    
    for(auto i=0; i<resistorStampIndexes.size()/2; i++){
        resistorStampIndexes[i*2] = i+nInR;
        voltageSourceStampIndexes[i*2+1] = i+nInR;
    }
    
    double R_val[n];
    double G_val[n];
    
    auto wdfChildComp = getChildsWDFTreeNodes();
    
    for(auto i=0; i<n; i++){
        R_val[i] = wdfChildComp[i]->calculateUpRes(1.0/44100.0);
        G_val[i] = 1/(wdfChildComp[i]->calculateUpRes(1.0/44100.0));
    }
    
    mat I = eye(n, n);
    mat R(n,n,fill::zeros);
    for ( unsigned int ii = 0; ii < n; ++ii ) {
            R.at(ii, ii) = R_val[ii];
        }
    mat Z1(n,n+nInR-1,fill::zeros);
    Z1 = join_rows(Z1,I);
    mat Z2 = trans(Z1);
    
    
    mat Y(nTotal,nTotal,fill::zeros);
    mat A(nTotal,n,fill::zeros);
    mat B(n,nTotal,fill::zeros);
    mat D(n,n,fill::zeros);
    
    //resistor stamp index j - i
    //j = resistorStampIndexes[i*2]
    //i = resistorStampIndexes[i*2+1]
    
    for(auto i=0; i<n; i++){
        //Resistor stamps
        Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2+1]) = Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2+1]) + G_val[i];
        Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2]) = Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2]) - G_val[i];
        Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2+1]) = Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2+1]) - G_val[i];
        Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2]) = Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2]) + G_val[i];
        
        //VolSource stamps
        A.at(voltageSourceStampIndexes[i*2+1],i) = A.at(voltageSourceStampIndexes[i*2+1],i) + 1;
        A.at(voltageSourceStampIndexes[i*2],i) = A.at(voltageSourceStampIndexes[i*2],i) - 1;
        B.at(i,voltageSourceStampIndexes[i*2+1]) = B.at(i,voltageSourceStampIndexes[i*2+1]) + 1;
        B.at(i,voltageSourceStampIndexes[i*2]) = B.at(i,voltageSourceStampIndexes[i*2]) - 1;
    }
    Y.print();
    A.print();
    B.print();
    
    mat X1 = join_rows(Y,A);
    mat X2 = join_rows(B,D);
    mat X = join_cols(X1, X2);
    
    X.print();
    
    X = X.submat(1, 1, nTotal+n-1, nTotal+n-1);
    
    X.print();
    
    mat S = I + 2*R*Z1*inv(X)*Z2*I;
    S.print();
    Smat = S;
    return S;
}

mat RNodeNonLinRootComponent::calculateScatteringMatrix(double *Rp){
    if(wireNodeIndexes.size() == 0) return nullptr;
    

    auto nNullors = nullorStampIndexes.size()/4;
    auto n = getNumChilds();
    auto nInR = *(std::max_element(wireNodeIndexes.begin(),wireNodeIndexes.end())) + 1;
    auto nTotal = n + nInR;
    
    for(auto i=0; i<resistorStampIndexes.size()/2; i++){
        resistorStampIndexes[i*2] = i+nInR;
        voltageSourceStampIndexes[i*2+1] = i+nInR;
    }
    
    double R_val[n];
    double G_val[n];
    
    for(auto i=0; i<n; i++){
        R_val[i] = Rp[i];
        G_val[i] = 1/Rp[i];
    }
    
    mat I = eye(n, n);
    mat R(n,n,fill::zeros);
    for ( unsigned int ii = 0; ii < n; ++ii ) {
            R.at(ii, ii) = R_val[ii];
        }
    mat Z1(n,n+nInR-1,fill::zeros);
    Z1 = join_rows(Z1,I);
    if(nNullors != 0){
        mat Z1N(n,nNullors, fill::zeros);
        Z1 = join_rows(Z1,Z1N);
    }
    mat Z2 = trans(Z1);
    
    
    mat Y(nTotal,nTotal,fill::zeros);
    mat A(nTotal,n+nNullors,fill::zeros);
    mat B(n+nNullors,nTotal,fill::zeros);
    mat D(n+nNullors,n+nNullors,fill::zeros);
    
    //resistor stamp index j - i
    //j = resistorStampIndexes[i*2]
    //i = resistorStampIndexes[i*2+1]
    
    for(auto i=0; i<n; i++){
        //Resistor stamps
        Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2+1]) = Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2+1]) + G_val[i];
        Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2]) = Y.at(resistorStampIndexes[i*2+1],resistorStampIndexes[i*2]) - G_val[i];
        Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2+1]) = Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2+1]) - G_val[i];
        Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2]) = Y.at(resistorStampIndexes[i*2],resistorStampIndexes[i*2]) + G_val[i];
        
        //VolSource stamps
        A.at(voltageSourceStampIndexes[i*2+1],i) = A.at(voltageSourceStampIndexes[i*2+1],i) + 1;
        A.at(voltageSourceStampIndexes[i*2],i) = A.at(voltageSourceStampIndexes[i*2],i) - 1;
        B.at(i,voltageSourceStampIndexes[i*2+1]) = B.at(i,voltageSourceStampIndexes[i*2+1]) + 1;
        B.at(i,voltageSourceStampIndexes[i*2]) = B.at(i,voltageSourceStampIndexes[i*2]) - 1;
        
        
    }
    
    for (auto i=0; i<nNullors; i++){
        //Nullor stamps
        A.at(nullorStampIndexes[2+i*4],n+i) = A.at(nullorStampIndexes[2+i*4],n+i) + 1;
        A.at(nullorStampIndexes[3+i*4],n+i) = A.at(nullorStampIndexes[3+i*4],n+i) - 1;
        B.at(n+i,nullorStampIndexes[0+i*4]) = B.at(n+i,nullorStampIndexes[0+i*4]) + 1;
        B.at(n+i,nullorStampIndexes[1+i*4]) = B.at(n+i,nullorStampIndexes[1+i*4]) - 1;
    }
    
    Y.print();
    A.print();
    B.print();
    
    mat X1 = join_rows(Y,A);
    mat X2 = join_rows(B,D);
    mat X = join_cols(X1, X2);
    
    X.print();
    
    X = X.submat(1, 1, nTotal+n+nNullors-1, nTotal+n+nNullors-1);
    
    X.print();
    
    mat S = I + 2*R*Z1*inv(X)*Z2*I;
    S.print();
    Smat = S;
    return S;
}

void RNodeNonLinRootComponent::createNullorStamps(){
    isCreatingNullor = true;
    for(auto i=0; i<4; i++){
        nullorStampIndexes.push_back(-1);
    }
    nullorNodeSelectionState = I;
}

ComponentType RNodeNonLinRootComponent::getComponentType() {
    return ComponentType::R_RNODE;
}


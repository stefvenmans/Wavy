#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    formatManager.registerBasicFormats();
    auto file = juce::File::getCurrentWorkingDirectory().getChildFile ("input.wav");
    reader = formatManager.createReaderFor(file);
    
    
    //GUI
    
    addAndMakeVisible(controlPanel);
    
    //TODO: set size in settings window
    addAndMakeVisible(schematic);
    schematic.centreWithSize(820*5, 580*5);
    
    viewPort.setViewedComponent(&schematic, false);
    addAndMakeVisible(&viewPort);
    viewPort.setBounds(0, 0, 820, 420);
    viewPort.setScrollBarsShown(true,true);
    
    addAndMakeVisible(propertyPanel);
    propertyPanelViewPort.setViewedComponent(&propertyPanel, false);
    addChildComponent(propertyPanelViewPort);
    propertyPanelViewPort.setScrollBarsShown(true, false);
    propertyPanel.setBounds(0, 0, 250, 1000);
    propertyPanel.setOutputCallbackFunction(std::bind(&MainComponent::setOutput,this,std::placeholders::_1));
    propertyPanel.setInputCallbackFunction(std::bind(&MainComponent::setInput,this,std::placeholders::_1));
    
    addAndMakeVisible(frontPanel);
    frontPanel.setBounds(0,420,820,580-420);
    
    addAndMakeVisible(calculateMatButton);
    calculateMatButton.setButtonText("Mat");
    calculateMatButton.onClick = [this](){
        
    };
    
    addAndMakeVisible(textButton);
    textButton.onClick = [this](){
        
        if(simpleRoot != nullptr){
            wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
            wdfEnvironment.setRoot(simpleRoot->createWDFComponent());
        }
        else if(rNode != nullptr){

            wdfEnvironment.setSMat(rNode->calculateScatteringMatrix());
            
            wdfEnvironment.setSubtreeEntryNodes(rNode->getChildsWDFTreeNodes());
        }

        wdfEnvironment.initTree();
        wdfEnvironment.adaptTree();

        std::cout << "Tree will be calculated for input:  " << inputIndex << "  and output :  " << outputIndex << std::endl;
        
        readAudio();
        processAudio();
        writeAudio();
        
//        for(auto i=0; i<500; i++){
//            wdfEnvironment.cycleWave();
//            if(outputIndex != -1 && outputIndex < leafComponents.size()){
//                std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
//            }
//            else std::cout << "No output set!" << std::endl;
//        }
    };
    
    addAndMakeVisible(showLibraryButton);
    showLibraryButton.setBounds(600, 400, 40, 40);
    showLibraryButton.onClick = [this] () {
//        std::cout << sidePanel.Component::isShowing() << std::endl;
//        if(sidePanel.isPanelShowing() == false){
//            sidePanel.showOrHide(true);
//            setSize(1000,getHeight());
//            viewPort.setBounds(sidePanel.getWidth(), 0, getWidth()-sidePanel.getWidth(), getHeight()-160);
//        frontPanel.setBounds(sidePanel.getWidth(),getHeight()-160,getWidth()-sidePanel.getWidth(),580-420);
//        }
//        else{
//            viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
//            frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420);
//            sidePanel.showOrHide(false);
//        }
        propertyPanelShowHide = false;
        
        schematic.repaint();
        
        resized();
    };
    
    
//    addAndMakeVisible(sidePanel);
//    sidePanel.setBounds(300,300,200,300);
    
    componentSelector.addItem("Resistor", 1);
    componentSelector.addItem("Capacitor",2);
    componentSelector.addItem("Inverter", 3);
    componentSelector.addItem("Y-Parameter Amplifier",4);
    componentSelector.addItem("Series", 5);
    componentSelector.addItem("Parallel",6);
    componentSelector.addItem("IdealVolSource",7);
    componentSelector.addItem("R Node",8);
    componentSelector.addItem("Resistive Voltage Source",9);
    componentSelector.addItem("Short Circuit",10);
    componentSelector.addItem("-",11);
    componentSelector.addItem("Old R Node", 12);
    componentSelector.addItem("Diode", 13);
    componentSelector.addItem("Transistor",14);
    componentSelector.addItem("R Node Non Lin", 15);
    addAndMakeVisible(componentSelector);
    
    componentSelector.onChange = [this](){
        switch(componentSelector.getSelectedId()){
            case 1:
                schematic.addAndMakeVisible(leafComponents.add(new Resistor()));
                leafComponents.getLast()->setName("R1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 2:
                schematic.addAndMakeVisible(leafComponents.add(new Capacitor()));
                leafComponents.getLast()->setName("C1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 3:
                schematic.addAndMakeVisible(leafComponents.add(new Inverter()));
                leafComponents.getLast()->setName("I1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 4:
                schematic.addAndMakeVisible(leafComponents.add(new YParameterAmp()));
                leafComponents.getLast()->setName("y-1");
                leafComponents.getLast()->setBounds(20,20,200,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 5:
                schematic.addAndMakeVisible(leafComponents.add(new Series()));
                leafComponents.getLast()->setName("S1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 6:
                schematic.addAndMakeVisible(leafComponents.add(new Parallel()));
                leafComponents.getLast()->setName("P1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 7:
                simpleRoot = std::make_unique<IdealVoltageSource>();
                simpleRoot->setName("V1");
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 8:
                rNode = std::make_unique<RNodeRootComponent>();
                schematic.addAndMakeVisible(rNode.get());
                rNode->setCentrePosition(200, 200);
                rNode->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                rNode->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 9:
                schematic.addAndMakeVisible(leafComponents.add(new VoltageSource()));
                leafComponents.getLast()->setName("V1");
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 10:
                simpleRoot = std::make_unique<ShortCircuit>();
                simpleRoot->setName("Shrt1");
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 11:
                break;
            case 13:
                schematic.addAndMakeVisible(nonLinearComponents.add(new Diode()));
                nonLinearComponents.getLast()->setName("D1");
                nonLinearComponents.getLast()->setBounds(20,20,100,100);
                nonLinearComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                nonLinearComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 14:
                schematic.addAndMakeVisible(nonLinearComponents.add(new Transistor()));
                nonLinearComponents.getLast()->setName("Q1");
                nonLinearComponents.getLast()->setBounds(20,20,200,100);
                nonLinearComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                nonLinearComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 15:
                rNodeRootNonLin = std::make_unique<RNodeNonLinRootComponent>();
                rNodeRootNonLin->setName("Rn");
                schematic.addAndMakeVisible(rNodeRootNonLin.get());
                rNodeRootNonLin->setCentrePosition(200, 200);
                rNodeRootNonLin->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                rNodeRootNonLin->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
        }
    };
    
    
    //WDF param
    res1Val.name = "Resistor1";
    res1Val.ID = 0;
    res1Val.type = doubleParam;
    res1Val.value = 1000;
    res1Val.units = "ohms";
    res1Val.lowLim = 1e-3;
    res1Val.highLim = 1e4;
    
    freqSlider.setRange(100, 2000);
    freqSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    freqSlider.setValue(440);
    addAndMakeVisible(freqSlider);
    freqSlider.onDragEnd = [this](){
        osc.setFrequency(freqSlider.getValue());
    };
    
    wdfEnvironment.addParam(res1Val);
    
    
    // Library
    addAndMakeVisible(&sidePanel);
    sidePanel.setShadowWidth(0);
    setSize (820, 580);
    sidePanel.showOrHide(false);
    
    addAndMakeVisible(calculateRaw);
    calculateRaw.onClick = [this] () {
        if(simpleRoot != nullptr || rNode != nullptr){
            if(simpleRoot != nullptr){
                    wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
                    wdfEnvironment.setRoot(simpleRoot->createWDFComponent());
                    wdfEnvironment.initTree();
                    wdfEnvironment.adaptTree();
                }
            else if(rNode != nullptr){
                //auto sMatrix = rNode->calculateScatteringMatrix();
                wdfEnvironment.setSubtreeEntryNodes(rNode->getChildsWDFTreeNodes());
                
                //wdfEnvironment.adaptTree();
                wdfEnvironment.getRNodeMatLambda = [this](double *Rp){
                    return rNode->calculateScatteringMatrix(Rp);
                };
                
                wdfEnvironment.initTree();
                wdfEnvironment.adaptTree();
                
                }
            
            if(inputIndex != -1){
                readAudio();
                processAudio();
                writeAudio();
            }
            else {
                for(auto i=0; i<500; i++){
                wdfEnvironment.cycleWave();
                if(outputIndex != -1 && outputIndex < leafComponents.size()){
                    std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                }
                else std::cout << "No output set!" << std::endl;
                }
            }
            
        }
    };
    
    addAndMakeVisible(runSimulationButton);
    runSimulationButton.onClick = [this](){
        runSimulation = !runSimulation;
        (runSimulation) ? (std::cout << "Running") : (std::cout << "Not running") ;
        std::cout << std::endl;
    };
    runSimulationButton.setButtonText("Run");


    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        
        setAudioChannels (2, 2);
    }
}

void MainComponent::openPropertyPanelForComponent(CircuitComponent* c){
    propertyPanelShowHide = true;
    resized();
    propertyPanel.setPropertiesForComponent(c);
}

bool MainComponent::wantsToConnect_(CircuitComponent* c)
{
    auto cX = c->getX();
    auto cY = c->getY();
    auto cW = c->getWidth();
    auto cH = c->getHeight();
    
    for(auto i: leafComponents){
        //if(i->getX() + c->getRotationX()*100 == c->getX() && i->getY() + c->getRotationY()*100 == c->getY()){
        auto iX = i->getX();
        auto iY = i->getY();
        auto iW = i->getWidth();
        auto iH = i->getHeight();
        
        if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
            std::cout << "circuit could connect"<< std::endl;
            c->connect(i);
            i->connect(c);
            
            c->repaint();
            i->repaint();
            //return true;
        }
    }
    
    if(simpleRoot != nullptr && c!=rNode.get()){
        auto iX = simpleRoot.get()->getX();
        auto iY = simpleRoot.get()->getY();
        auto iW = simpleRoot.get()->getWidth();
        auto iH = simpleRoot.get()->getHeight();
        
        if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
            std::cout << "a root want could connect to this component" << std::endl;
            c->connect(simpleRoot.get());
            simpleRoot->connect(c);
            
            c->repaint();
            simpleRoot->repaint();
        }
    }
    
    if(rNode != nullptr && c!=simpleRoot.get()){
        
        
        for(auto i=0; i<rNode->getCollums(); i++){
            for(auto j=0; j<rNode->getRows(); j++){
                auto iX = rNode.get()->getX() + (rNode.get()->getWidth()/rNode->getCollums())*(i);
                auto iY = rNode.get()->getY() + (rNode.get()->getHeight()/rNode->getRows())*(j);
                auto iW = rNode.get()->getWidth();
                auto iH = rNode.get()->getHeight();
                
                if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
                    std::cout << "a r node want could connect to this component" << std::endl;
                    c->connect(rNode.get());
                    rNode->connect(c);
                    
                    c->repaint();
                    rNode->repaint();
                }
            }
        }
    }
    
    if(rNodeRootNonLin != nullptr && c!=simpleRoot.get() && c!=rNode.get()){
        
        
        for(auto i=0; i<rNodeRootNonLin->getCollums(); i++){
            for(auto j=0; j<rNodeRootNonLin->getRows(); j++){
                auto iX = rNodeRootNonLin.get()->getX() + (rNodeRootNonLin.get()->getWidth()/rNodeRootNonLin->getCollums())*(i);
                auto iY = rNodeRootNonLin.get()->getY() + (rNodeRootNonLin.get()->getHeight()/rNodeRootNonLin->getRows())*(j);
                auto iW = rNodeRootNonLin.get()->getWidth();
                auto iH = rNodeRootNonLin.get()->getHeight();
                
                if(((iX+iW)==cX && iY==cY) || ((iY+iH)==cY && iX==cX) || ((cX+cW)==iX && cY==iY) || ((cY+cH)==iY && cX==iX)){
                    std::cout << "a r node root want could connect to this component" << std::endl;
                    
                    c->connect(rNodeRootNonLin.get());
                    rNodeRootNonLin->connect(c);
                    
                    c->repaint();
                    rNodeRootNonLin->repaint();
                }
            }
        }
    }
    
}

void MainComponent::setOutput(CircuitComponent* c){
    outputIndex = leafComponents.indexOf((AdaptedLeafComponent*)c);
}

void MainComponent::setInput(CircuitComponent* c){
    inputIndex = leafComponents.indexOf((AdaptedLeafComponent*)c);
}


MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    delete reader;
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlockExpected;
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    
    osc.prepare(spec);
    
    std::cout << "samples per block" << samplesPerBlockExpected << std::endl;
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
    //for(auto j=0; j<bufferToFill.buffer->getNumChannels() ; j++){
    for(auto j=0; j<1 ; j++){
        auto bufferIn = bufferToFill.buffer->getReadPointer(j);
        auto bufferOut = bufferToFill.buffer->getWritePointer(j);
        if(runSimulation){
            
            for(auto i=0; i<bufferToFill.numSamples ;i++){
                
                    //output[i] = wdf->getOutputValue();
                    
                    if(inputIndex != -1 && inputIndex < leafComponents.size()){
                        ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = 0.1*osc.processSample( 0);//bufferIn[i];
                    }
                    wdfEnvironment.cycleWave();
                    
                    if(outputIndex != -1 && outputIndex < leafComponents.size()){
                        //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                        bufferOut[i] = -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage();
            //        std::cout << wdf->getOutputValue() << std::endl;
            //        std::cout << input[i] << std::endl;
                    }
                }
        }
        else {
            
            for(auto i=0; i<bufferToFill.numSamples ;i++){
                bufferOut[i] = 0.1*osc.processSample( 0);
            }
        }
    }
    
    //bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colours::white);
    
    

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
//    r1.setBounds(20,20, 100, 100);
//    r2.setBounds(220,20, 100, 100);
//    s1.setBounds(120,20,100,100);
//    v1.setBounds(120,120,100,100);
//
//    c1.setBounds(320,320,100,100);
//    p1.setBounds(420,320,100,100);
    
    
//    juce::Grid grid;
//
//    grid.setGap((juce::Grid::Px)20);
//    using Track = juce::Grid::TrackInfo;
//
//    grid.templateRows = { Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1) };
//    grid.templateColumns = { Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1), Track ((juce::Grid::Fr)1) };
//
//    grid.autoColumns = Track ((juce::Grid::Fr)(1));
//    grid.autoRows    = Track ((juce::Grid::Fr)(1));
//    grid.autoFlow = juce::Grid::AutoFlow::column;
    
//    grid.items.addArray ({  juce::GridItem (components[0]),///.withArea (2, 2, 4, 4)
//                            juce::GridItem(components[1]),
 //                           juce::GridItem(components[2])
//                               GridItem (items[1]),
//                               GridItem (items[2]).withArea ({}, 3),
//                               GridItem (items[3]),
//                               GridItem (items[4]).withArea (GridItem::Span (2), {}),
//                               GridItem (items[5]),
//                               GridItem (items[6]),
//                               GridItem (items[7]),
//                               GridItem (items[8]),
//                               GridItem (items[9]),
//                               GridItem (items[10]),
//                               GridItem (items[11])
  //                          });

//    grid.performLayout (getLocalBounds());
    
    //draggableComp.setBounds(300, 300, 100, 100);
    
    int numX = 5;
    int numY = 5;
    
    auto stepX = getWidth()/numX;
    auto stepY = getHeight()/numY;
    
    textButton.setBounds(getWidth()-350,40,50,50);
    showLibraryButton.setBounds(getWidth()-350,40*2+80,50,50);
    freqSlider.setBounds(680,40,80,200);
    componentSelector.setBounds(getWidth()-350,40+80,100,30);
    calculateMatButton.setBounds(getWidth()-350,40*2+80+60,50,50);
    calculateRaw.setBounds(getWidth()-350, 40*2+80+60+70, 50, 50);
    runSimulationButton.setBounds(getWidth()-350, 40*2+80+60+70+ 60, 50, 50);
    
//    for (auto x = 0; x < numX; ++x)
//        {
//            for (auto y = 0; y < numY; ++y)
//            {
//                // creates the rectangle     (x,         y,         width, height)
//                juce::Rectangle<int> elementBounds (x * stepX, y * stepY, stepX, stepY);
//
//                // set the size and position of the Toggle light to this rectangle.
//                components[x + numX * y].setBounds (elementBounds);
//            }
//        }
    
    
    controlPanel.setBounds(0, getHeight()-30, getWidth(), 30);
    frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420 - 30);
    
    if(propertyPanelShowHide){
        viewPort.setBounds(0, 0, getWidth()-250, getHeight()-160);
        propertyPanelViewPort.setBounds(getWidth()-250,0,250,getHeight()-160);
        propertyPanelViewPort.setVisible(true);
    }
    else{
        viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
        propertyPanelViewPort.setVisible(false);
    }
    
}

void MainComponent::mouseMagnify (const juce::MouseEvent &event, float scaleFactor){
    scale -= (1-scaleFactor)/2.0;
    
    if(scale <= (getWidth()-10)/(5.0*820.0)) scale = (getWidth()-10)/(5.0*820.0);
    else if(scale >= 2) scale = 2;
    schematic.setTransform(getTransform().scale(scale, scale,event.getPosition().getX(), event.getPosition().getY()));
}


void MainComponent::readAudio(){
    buffer_in.setSize(1, (int)reader->lengthInSamples);
    reader->read(&buffer_in, 0, (int)reader->lengthInSamples, 0, true, false);
}

void MainComponent::processAudio(){
    auto *input = buffer_in.getReadPointer(0);
    buffer_out.setSize(buffer_in.getNumChannels(), buffer_in.getNumSamples());
    auto *output = buffer_out.getWritePointer(0);
    for(auto i=0; i<buffer_in.getNumSamples(); i++){
        //output[i] = wdf->getOutputValue();
        
        if(inputIndex != -1 && inputIndex < leafComponents.size()){
            ((wdfTerminatedResVSource*)(leafComponents[inputIndex]->getWDFComponent()))->Vs = input[i];
        }
        wdfEnvironment.cycleWave();
        
        if(outputIndex != -1 && outputIndex < leafComponents.size()){
            //std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
            output[i] = -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage();
//        std::cout << wdf->getOutputValue() << std::endl;
//        std::cout << input[i] << std::endl;
        }
    }
}

void MainComponent::writeAudio(){
    juce::WavAudioFormat format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    auto file_out = juce::File::getCurrentWorkingDirectory().getChildFile ("output.wav");
    file_out.deleteFile();
    writer.reset(format.createWriterFor(new juce::FileOutputStream(file_out), 44100, buffer_out.getNumChannels(), 24, {}, 0));
    
    if(writer != nullptr){
        std::cout << "audio file written" << std::endl;
        writer->writeFromAudioSampleBuffer(buffer_out, 0, buffer_out.getNumSamples());
    }
}

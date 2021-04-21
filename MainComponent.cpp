#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    
    //GUI
    
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
    
    addAndMakeVisible(frontPanel);
    frontPanel.setBounds(0,420,820,580-420);
    
    addAndMakeVisible(textButton);
    textButton.onClick = [this](){
        wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
        wdfEnvironment.setRoot(simpleRoot->createWDFComponent());

        wdfEnvironment.initTree();
        wdfEnvironment.adaptTree();

        for(auto i=0; i<500; i++){
            wdfEnvironment.cycleWave();
            if(outputIndex != -1 && outputIndex < leafComponents.size()){
                std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
            }
            else std::cout << "No output set!" << std::endl;
        }
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
    addAndMakeVisible(componentSelector);
    
    componentSelector.onChange = [this](){
        switch(componentSelector.getSelectedId()){
            case 1:
                schematic.addAndMakeVisible(leafComponents.add(new Resistor_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 2:
                schematic.addAndMakeVisible(leafComponents.add(new Capacitor_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 3:
                schematic.addAndMakeVisible(leafComponents.add(new Inverter_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 4:
                schematic.addAndMakeVisible(leafComponents.add(new YParameterAmp()));
                leafComponents.getLast()->setBounds(20,20,200,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 5:
                schematic.addAndMakeVisible(leafComponents.add(new Series_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
                
            case 6:
                schematic.addAndMakeVisible(leafComponents.add(new Parallel_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 7:
                simpleRoot = std::make_unique<IdealVoltageSource_>();
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 8:
                rNode = std::make_unique<RNode_>();
                schematic.addAndMakeVisible(rNode.get());
                rNode->setBounds(20, 20, 200 , 200);
                rNode->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                break;
            case 9:
                schematic.addAndMakeVisible(leafComponents.add(new VoltageSource_()));
                leafComponents.getLast()->setBounds(20,20,100,100);
                leafComponents.getLast()->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                leafComponents.getLast()->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 10:
                simpleRoot = std::make_unique<ShortCircuit_>();
                schematic.addAndMakeVisible(simpleRoot.get());
                simpleRoot->setBounds(20,20,100,100);
                simpleRoot->addHandler(std::bind(&MainComponent::wantsToConnect_,this,std::placeholders::_1));
                simpleRoot->setPropertyPanelCallback(std::bind(&MainComponent::openPropertyPanelForComponent,this,std::placeholders::_1));
                break;
            case 11:
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
    
    res1Slider.setRange(res1Val.lowLim, res1Val.highLim);
    res1Slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    res1Slider.setValue(res1Val.value);
    addAndMakeVisible(res1Slider);
    
    wdfEnvironment.addParam(res1Val);
    
    
    // Library
    addAndMakeVisible(&sidePanel);
    sidePanel.setShadowWidth(0);
    setSize (820, 580);
    sidePanel.showOrHide(false);

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
}

void MainComponent::setOutput(CircuitComponent* c){
    outputIndex = leafComponents.indexOf((AdaptedLeafComponent*)c);
}


MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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
    
    textButton.setBounds(getWidth()-200,40,50,50);
    showLibraryButton.setBounds(getWidth()-200,40*2+80,50,50);
    //res1Slider.setBounds(680,40,80,200);
    componentSelector.setBounds(getWidth()-200,40+80,100,30);
    
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
    
    
    
    frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420);
    
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

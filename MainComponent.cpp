#include "MainComponent.h"


//bool isConnectable(int x, int y)
//{
////    for(auto c: MainComponent::components){
////        if(c->getX() == x && c->getY() == y) return true;
////    }
//    std::cout << x << "  -  " << y << std::endl;
//    MainComponent::printSomething();
//    if(x>100){
//    return true;
//    }
//    return false;
//}


//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    
    //GUI
    
    
    
    //sidePanel.setBounds(0,0,100,580);
    
    addAndMakeVisible(schematic);
    schematic.centreWithSize(820*5, 580*5);
    
    viewPort.setViewedComponent(&schematic, false);
    addAndMakeVisible(&viewPort);
    viewPort.setBounds(0, 0, 820, 420);
    viewPort.setScrollBarsShown(true,true);
    
    addAndMakeVisible(r1);
    addAndMakeVisible(r2);
    addAndMakeVisible(s1);
    addAndMakeVisible(v1);
    
    addAndMakeVisible(c1);
    addAndMakeVisible(p1);
    
    addAndMakeVisible(frontPanel);
    frontPanel.setBounds(0,420,820,580-420);
    
    frontPanel.addNewComponent(new juce::Slider());
   
    
    
    
//    addAndMakeVisible(components.add(new Resistor()));
//    addAndMakeVisible(components.add(new Series()));
//    addAndMakeVisible(components.add(new DraggableComp()));
    
    //addAndMakeVisible(draggableComp);
    //draggableComp.setBounds(40,40,100,100);
    
    addAndMakeVisible(textButton);
    textButton.onClick = [this](){
        //draggableComp.setSize(50, 50);
//        scale -= .05;
//        schematic.setTransform(getTransform().scale(scale, scale));
        
        //((wdfTerminatedRes*)(r1.getWDFComponent()))->R = res1Slider.getValue();
        
        
        
        v1.createWDFComponent(10);
        wdfEnvironment.setSubtreeEntryNodes(((Series*)components[0])->getWDFComponent());
        
        wdfEnvironment.setRoot(v1.getWDFComponent());
            
        wdfEnvironment.initTree();
        wdfEnvironment.adaptTree();
        
        
        wdfEnvironment.adaptTree();
        //res1Val.value = res1Slider.getValue();
        for(auto i=0; i<500; i++){
            wdfEnvironment.cycleWave();
            std::cout << -((Resistor*)components[1])->getWDFComponent()->upPort->getPortVoltage() << std::endl;
        }
        //frontPanel.addNewComponent(new juce::Slider());
    };
    
    addAndMakeVisible(showLibraryButton);
    showLibraryButton.setBounds(600, 400, 40, 40);
    showLibraryButton.onClick = [this] () {
        std::cout << sidePanel.Component::isShowing() << std::endl;
        if(sidePanel.isPanelShowing() == false){
            sidePanel.showOrHide(true);
            setSize(1000,getHeight());
            viewPort.setBounds(sidePanel.getWidth(), 0, getWidth()-sidePanel.getWidth(), getHeight()-160);
        frontPanel.setBounds(sidePanel.getWidth(),getHeight()-160,getWidth()-sidePanel.getWidth(),580-420);
            
            
        }
        else{
            viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
            frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420);
            sidePanel.showOrHide(false);
        }
        
        
    };
    
    
//    addAndMakeVisible(sidePanel);
//    sidePanel.setBounds(300,300,200,300);
    
    componentSelector.addItem("Resistor", 1);
    componentSelector.addItem("Capacitor",2);
    componentSelector.addItem("Parallel Adapter",3);
    componentSelector.addItem("Series Adapter",4);
    componentSelector.addItem("Ideal Voltage Source", 5);
    componentSelector.addItem("R-Node",6);
    componentSelector.addItem("Dummy item",7);
    addAndMakeVisible(componentSelector);
    
    componentSelector.onChange = [this](){
        switch(componentSelector.getSelectedId()){
            case 1:
                schematic.addAndMakeVisible(components.add(new Resistor()));
                components.getLast()->setBounds(20,20,100,100);
                ((Resistor*)(components.getLast()))->addHandler(std::bind(&MainComponent::wantsToConnect,this, std::placeholders::_1));
                break;
            case 2:
                schematic.addAndMakeVisible(components.add(new Capacitor()));
                components.getLast()->setBounds(20,20,100,100);
                break;
            case 3:
                schematic.addAndMakeVisible(components.add(new Parallel()));
                components.getLast()->setBounds(20,20,100,100);
                break;
            case 4:
                schematic.addAndMakeVisible(components.add(new Series()));
                components.getLast()->setBounds(20,20,100,100);
                break;
            case 5:
                schematic.addAndMakeVisible(components.add(new IdealVSource()));
                components.getLast()->setBounds(20,20,100,100);
                break;
            case 6:
                schematic.addAndMakeVisible(components.add(new RNode()));
                components.getLast()->setBounds(20,20,200,200);
                break;
            case 7:
                schematic.addAndMakeVisible(components.add(new DraggableComp()));
                components.getLast()->setBounds(20,20,100,100);
                ((DraggableComp*)(components.getLast()))->addHandler(std::bind(&MainComponent::isConnectable,this, std::placeholders::_1, std::placeholders::_2));
                break;
        }
    };
    
    
    //WDF
    
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
    //s1.createWDFComponent(r1.createWDFComponent(wdfEnvironment.getParams()[0].value),r2.createWDFComponent(1000));
    //v1.createWDFComponent(10);
    //wdfEnvironment.setSubtreeEntryNodes(s1.getWDFComponent());
    //wdfEnvironment.setRoot(v1.getWDFComponent());
    
//    wdfEnvironment.initTree();
//    wdfEnvironment.adaptTree();
    
//    for(auto i=0; i<500; i++){
//        wdfEnvironment.cycleWave();
//        std::cout << -r1.getWDFComponent()->upPort->getPortVoltage() << std::endl;
//    }
    
    
    
    addAndMakeVisible(&sidePanel);
    sidePanel.setShadowWidth(0);
    setSize (820, 580);
    
    
    sidePanel.showOrHide(true);

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

bool MainComponent::isConnectable(int x, int y)
{
    for(auto c: MainComponent::components){
        if(c->getX() + 100 == x && c->getY() == y){
            std::cout << "component is connected" << std::endl;
            std::cout << c->getName() << std::endl;
            return true;
        }
    }
    std::cout << "component can't connect" << std::endl;
    return false;
}

bool MainComponent::wantsToConnect(juce::Component* c){
    std::cout << "size of components : " << components.size() << std::endl;
    for(auto i: MainComponent::components){
        if(i->getX() + ((Resistor*)c)->getRotationX()*100 == c->getX() && i->getY() + ((Resistor*)c)->getRotationY()*100 == c->getY()){
            std::cout << "component is connected" << std::endl;
            std::cout << c->getName() << std::endl;
            //std::cout << "rotation: " << ((Resistor*)c)->getRotation() << std::endl;
            
            if(i->getName() == "Ser"){
                //((Series*)(i))->getWDFComponent()->setLeftChild(((Resistor*)(c))->getWDFComponent());
                ((Resistor*)c)->createWDFComponent(1);
                ((Series*)i)->setChild(c);
            }
            
            
            return true;
        }
    }
    std::cout << "component can't connect" << std::endl;
    return false;
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
    
    textButton.setBounds(600,40,50,50);
    res1Slider.setBounds(680,40,80,200);
    componentSelector.setBounds(600,240,100,30);
    
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
    
    
    viewPort.setBounds(0, 0, getWidth(), getHeight()-160);
    frontPanel.setBounds(0,getHeight()-160,getWidth(),580-420);
    
}

void MainComponent::mouseMagnify (const juce::MouseEvent &event, float scaleFactor){
    scale -= (1-scaleFactor)/2.0;
    
    if(scale <= (getWidth()-10)/(5.0*820.0)) scale = (getWidth()-10)/(5.0*820.0);
    else if(scale >= 2) scale = 2;
    schematic.setTransform(getTransform().scale(scale, scale,event.getPosition().getX(), event.getPosition().getY()));
}

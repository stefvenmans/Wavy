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
        const double R_val[6] = {1.0, 1e6, (1.0/44100.0)/(2.0*0.015e-6), (1.0/44100.0)/(2.0*0.015e-6), 1e6, 53.8e3};
        
        mat I = eye(6, 6);
        mat R(6,6,fill::zeros);
        for ( unsigned int ii = 0; ii < 6; ++ii ) {
                R.at(ii, ii) = R_val[ii];
            }
        mat Z1(6,9,fill::zeros);
        Z1 = join_rows(Z1,I);
        mat Z2 = trans(Z1);
        
        
        const double Ga = 1.0/R_val[0];
        const double Gb = 1.0/R_val[1];
        const double Gc = 1.0/R_val[2];
        const double Gd = 1.0/R_val[3];
        const double Ge = 1.0/R_val[4];
        const double Gf = 1.0/R_val[5];
        
        
        mat Y({ {0,     0,      0,      0,      0,      0,      0,      0,      0,      0},
            {0,     Gd+Ge,  0,      0,      0,      0,      0,      -Gd,    -Ge,    0},
            {0,     0,      Ga+Gb,  0,      -Ga,    -Gb,    0,      0,      0,      0},
            {0,     0,      0,      Gc+Gf,  0,      0,      -Gc,    0,      0,      -Gf},
            {0,     0,      -Ga,    0,      Ga,     0,      0,      0,      0,      0},
            {0,     0,      -Gb,    0,      0,      Gb,     0,      0,      0,      0},
            {0,     0,      0,      -Gc,    0,      0,      Gc,     0,      0,      0},
            {0,     -Gd,    0,      0,      0,      0,      0,      Gd,     0,      0},
            {0,     -Ge,    0,      0,      0,      0,      0,      0,      Ge,     0},
            {0,     0,      0,      -Gf,    0,      0,      0,      0,      0,      Gf}});
        
        mat A({{-1,   0,  0,  0,  -1, -1},
            {0,   -1, 0,  0,  0,  0},
            {0,   0,  -1, 0,  0,  0},
            {0,   0,  0,  -1, 0,  0}});
        A = join_cols(A,I);
        
        mat B = -eye(4,4);
        mat addToB({{-1, 0, 0, 0}, {-1, 0, 0, 0}});
        B = join_cols(B,addToB);
        B = join_rows(B,I);
        
        mat D(6,6,fill::zeros);
        mat X1 = join_rows(Y,A);
        mat X2 = join_rows(B,D);
        mat X = join_cols(X1, X2);
        
        X = X.submat(1, 1, 15, 15);
        
        mat S = I + 2*R*Z1*inv(X)*Z2*I;
        S.print();
        Smat = S;
        
    };
    
    addAndMakeVisible(textButton);
    textButton.onClick = [this](){
        
        if(simpleRoot != nullptr){
            wdfEnvironment.setSubtreeEntryNodes(simpleRoot->getChildComponent()->createWDFComponent());
            wdfEnvironment.setRoot(simpleRoot->createWDFComponent());
        }
        else if(rNode != nullptr){
            //wdfEnvironment.setRoot(rNode->createWDFComponent());
            
//            const double scat[6][6] = {
//                {0.999961398551509, 3.06194644718824e-08, -3.85708290267018e-05, -1.93872246960703e-06, 1.96934193407962e-06, 3.66321065570946e-05},
//                {0.0306194644718891, -0.997006500327189, -1.96638703585530, -1.99401303127384, -0.00299346905334667, -0.0276259954185430},
//                {-0.0291540657798200, -0.00148630917298202, 0.969359625047198, -0.00297258919189813, 0.00148628001891631, 0.0276677857609040},
//                {-0.00146539869206916, -0.00150719049982899, -0.00297258919189822, 0.996985620465741, 0.00150718903443037, -4.17903423610279e-05},
//                {1.96934193407962, -0.00299346905334586, 1.96634846502627, 1.99401109255137, -0.997004561604719, 0.0276626275251001},
//                {1.97080733277169, -0.00148627855351745, 1.96932105421817, -0.00297452791436775, 0.00148824936085039, -0.972295582132539}
//            };
            
            
            
            
            
            
            
//            for ( unsigned int ii = 0; ii < 6; ++ii ) {
//                    for ( unsigned int jj = 0; jj < 6; ++jj ) {
//                        Smat.at(ii, jj) = scat[ii][jj];
//                    }
//
//                }
            //wdfEnvironment.setSMat(Smat);
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
                rNode->setCentrePosition(200, 200);
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
            case 12:
                rnode = std::make_unique<RNodeNew>();
                schematic.addAndMakeVisible(rnode.get());
                rnode->setCentrePosition(200, 200);
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
            
            
            for(auto i=0; i<500; i++){
                wdfEnvironment.cycleWave();
                if(outputIndex != -1 && outputIndex < leafComponents.size()){
                    std::cout << -leafComponents[outputIndex]->getWDFComponent()->upPort->getPortVoltage() << std::endl;
                }
                else std::cout << "No output set!" << std::endl;
            }
        }
    };

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
    calculateMatButton.setBounds(getWidth()-200,40*2+80+60,50,50);
    calculateRaw.setBounds(getWidth()-200, 40*2+80+60+70, 50, 50);
    
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
        writer->writeFromAudioSampleBuffer(buffer_out, 0, buffer_out.getNumSamples());
    }
}

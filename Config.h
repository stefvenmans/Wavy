/*
  ==============================================================================

    Config.h
    Created: 29 Apr 2021 5:41:29pm
    Author:  Stef

  ==============================================================================
*/

#pragma once
#include "../rt-wdf_lib/Libs/rt-wdf/rt-wdf.h"
#include <JuceHeader.h>

//#define C_WIDTH 100
//#define C_HEIGHT 100

enum ComponentType {
    L_RES,L_CAP,L_IND,L_VOL,L_CUR,A_INV,A_SER,A_PAR,SR_VOL,SR_CUR,SR_SHC,R_RNODE,NL_DIO,NL_TRAN,NOT_SET
};

const juce::String ComponentTypeString [] = {"L_RES","L_CAP","L_IND","L_VOL","L_CUR","A_INV","A_SER","A_PAR","SR_VOL","SR_CUR","SR_SHC","R_RNODE","NL_DIO","NL_TRAN","NOT_SET"};
const std::vector<juce::String> ComponentTypeStringVector = {"L_RES","L_CAP","L_IND","L_VOL","L_CUR","A_INV","A_SER","A_PAR","SR_VOL","SR_CUR","SR_SHC","R_RNODE","NL_DIO","NL_TRAN","NOT_SET"};

const juce::File appDataPath = juce::File::getSpecialLocation(juce::File::SpecialLocationType::currentApplicationFile).getParentDirectory();

static const int componentWidth = 100;
static const int componentHeight = 100;



/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Audioplug6AudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Audioplug6AudioProcessorEditor (Audioplug6AudioProcessor&);
    ~Audioplug6AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Audioplug6AudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Audioplug6AudioProcessorEditor)
};

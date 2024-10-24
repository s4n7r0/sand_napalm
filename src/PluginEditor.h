/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct AttachedSlider {

    AttachedSlider(NapalmAudioProcessor& p, juce::String paramid, std::vector<juce::String>& comps)
        : slider(), attachment(*p.apvts.getParameter(paramid), slider, &p.undo)
    {
        slider.setComponentID(paramid);
        comps.push_back(paramid);
    }

    juce::Slider slider;
    juce::SliderParameterAttachment attachment;
};

struct AttachedToggleButton {

    AttachedToggleButton(NapalmAudioProcessor& p, juce::String paramid, std::vector<juce::String>& comps)
        : button(p.apvts.getParameter(paramid)->getLabel()), attachment(*p.apvts.getParameter(paramid), button, &p.undo)
    {
        button.setComponentID(paramid);
        comps.push_back(paramid);
    }

    juce::ToggleButton button;
    juce::ButtonParameterAttachment attachment;

};

//==============================================================================
/**
*/
class NapalmAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NapalmAudioProcessorEditor (NapalmAudioProcessor&);
    ~NapalmAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    inline void show_or_hide();
    inline void set_font_size(juce::Graphics&, float);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NapalmAudioProcessor& audioProcessor;

    float text_size;
    juce::String invert_text;
    juce::String midi_text;
    juce::String amount_text;
    juce::String range_text;
    juce::String copies_text;
    juce::String contact_text;
    juce::String version_text;

    std::vector<juce::String> components;
    std::vector<juce::String> help_texts;

    AttachedToggleButton invert;
    AttachedToggleButton midi;
    juce::TextButton help;
    //juce::DrawableRectangle help_body;
    AttachedSlider delay_time;
    AttachedSlider time_multiplier;
    AttachedSlider copies;

    juce::Rectangle<int> amount_text_bounds;
    juce::Rectangle<int> multiplier_text_bounds;
    juce::Rectangle<int> copies_text_bounds;

    bool help_state;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NapalmAudioProcessorEditor)
};

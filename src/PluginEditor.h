/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "NapalmEditor.h"

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
    void show_or_hide();
    void set_font_size(juce::Graphics&, float);
    void set_scales();
    void draw_labels(juce::Graphics&);
    void draw_help(juce::Graphics&);
    void setup();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NapalmAudioProcessor& audioProcessor;

    //this is so i can scale gui with proportions of my screen
    const float my_screen_width{ 1920 };
    const float my_screen_height{ 1080 };

    float text_size;
    float x_scale;
    float y_scale;
    float abs_scale;
    float slider_width;

    std::vector<juce::String> components;
    std::vector<juce::String> help_texts;
    bool help_state;

    napalm::components::AttachedToggleButton invert;
    napalm::components::AttachedToggleButton midi;
    napalm::components::TextButtonBounds help;
    napalm::components::AttachedSlider amount;
    napalm::components::AttachedSlider range;
    napalm::components::AttachedSlider pitch;
    napalm::components::AttachedSlider pitchmax;
    napalm::components::AttachedSlider copies;

    napalm::NapalmBounds amount_text_bounds;
    napalm::NapalmBounds range_text_bounds;
    napalm::NapalmBounds copies_text_bounds;

    napalm::components::URLTimer url_timer;

    juce::TextButton hehe;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NapalmAudioProcessorEditor)
};

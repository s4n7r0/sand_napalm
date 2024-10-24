/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "NapalmEditor.h"

//==============================================================================
NapalmAudioProcessorEditor::NapalmAudioProcessorEditor (NapalmAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), help_state{false},
      delay_time(p, "amount", components),
      time_multiplier(p, "multiplier", components),
      copies(p, "copies", components),
      invert(p, "invert", components),
      midi(p, "midi", components)
{

    using SliderIds = juce::Slider::ColourIds;
    using TextButtonIds = juce::TextButton::ColourIds;

    // function alias
    auto temp_laf = std::mem_fn(&NapalmAudioProcessorEditor::getLookAndFeel);
    auto LAF = std::bind(temp_laf, this); 

    setSize (400, 200);
    setResizable(false, false);
    setRepaintsOnMouseActivity(true);

    LAF().setColour(SliderIds::backgroundColourId, napalm::colours::component_background);
    LAF().setColour(SliderIds::trackColourId, napalm::colours::slider_track);
    LAF().setColour(SliderIds::thumbColourId, napalm::colours::thumb);
    LAF().setColour(SliderIds::textBoxOutlineColourId, napalm::colours::invisible);
    LAF().setColour(TextButtonIds::buttonColourId, napalm::colours::component_background);

    tooltip_text = std::string(
        "Invert Phase: Inverts phase of every even copy\nMIDI: Uses midi input instead of given range\n,\nplease contact me on discord: .sandr0\n\Ver: ") + NAPALM_VER;

    help.setButtonText("?");
    help.setBounds(getWidth() - 35, 10, 25, 25);
    help.setColour(1, napalm::colours::component_background);
    help_text.setText(tooltip_text);
    help_text.setJustification(juce::Justification::horizontallyCentred);
    help_text.setBounds(50, 50, 200, 200);

    invert.button.setButtonText("invert phase");
    invert.button.setBounds(37, 10, 100, 25);

    midi.button.setButtonText("midi");
    midi.button.setBounds(137, 10, 100, 25);

    delay_time.slider.setBounds(25, 35, getWidth() / 1.25, 50);
    delay_time.slider.setHelpText("delay in samples");
    delay_time.slider.setNumDecimalPlacesToDisplay(3);
    delay_time.slider.setTextBoxStyle(delay_time.slider.getTextBoxPosition(), 0, 50, 25);

    delay_time.slider.setSkewFactor(2.f);
    delay_time.slider.setRange(juce::Range<double>(0, 1), 0.0001);

    time_multiplier.slider.setBounds(25, 85, getWidth() / 1.25, 50);
    time_multiplier.slider.setHelpText("multiplier");
    time_multiplier.slider.setNumDecimalPlacesToDisplay(0);
    time_multiplier.slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    time_multiplier.slider.setTextBoxStyle(time_multiplier.slider.getTextBoxPosition(), 0, 50, 25);

    time_multiplier.slider.setRange(juce::Range<double>(1, napalm::MAX_SAMPLES_IN_BUFFER), 1);
    time_multiplier.slider.setSkewFactor(.5f);

    copies.slider.setBounds(25, 135, getWidth() / 1.25, 50);
    copies.slider.setHelpText("copies");
    copies.slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    copies.slider.setRange(juce::Range<double>(1, 32), 1);
    copies.slider.setTextBoxStyle(copies.slider.getTextBoxPosition(), 0, 50, 25);

    amount_text_bounds = delay_time.slider.getBounds();
    multiplier_text_bounds = time_multiplier.slider.getBounds();
    copies_text_bounds = copies.slider.getBounds();

    amount_text_bounds.setY(amount_text_bounds.getY() - 20);
    amount_text_bounds.setX(amount_text_bounds.getX() - 20);
    multiplier_text_bounds.setY(multiplier_text_bounds.getY() - 20);
    multiplier_text_bounds.setX(multiplier_text_bounds.getX() - 20);
    copies_text_bounds.setY(copies_text_bounds.getY() - 20);
    copies_text_bounds.setX(copies_text_bounds.getX() - 20);

    addAndMakeVisible(delay_time.slider     , 0);
    addAndMakeVisible(time_multiplier.slider, 0);
    addAndMakeVisible(copies.slider         , 0);
    addAndMakeVisible(midi.button           , 0);
    addAndMakeVisible(invert.button         , 0);
    addAndMakeVisible(help                  , 0);
    addAndMakeVisible(amount_text           , 0);
    addAndMakeVisible(multiplier_text       , 0);
    addAndMakeVisible(copies_text           , 0);

}

NapalmAudioProcessorEditor::~NapalmAudioProcessorEditor()
{
}

//==============================================================================
void NapalmAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (napalm::colours::background);

    g.setColour (napalm::colours::text);
    g.setFont(15);
    g.drawFittedText(juce::String("amount"), amount_text_bounds, juce::Justification::right, 4, 0);
    g.drawFittedText(juce::String("range"), multiplier_text_bounds, juce::Justification::right, 4, 0);
    g.drawFittedText(juce::String("copies"), copies_text_bounds, juce::Justification::right, 4, 0);

    if (help.isMouseOver()) {

        if (!help_state) help_state = true;

        g.fillAll (napalm::colours::background);
        g.drawSingleLineText("NAPALM", getWidth() / 2, 25, juce::Justification::centred);
        g.drawFittedText(tooltip_text, juce::Rectangle<int>(25, 50, getWidth(), getHeight() / 1.5), juce::Justification::left, 10, 0);

    } else if (help_state) {
        help_state = false;
    }

    show_or_hide();
}

void NapalmAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

inline void NapalmAudioProcessorEditor::show_or_hide() {
    if (help_state) {
        for (auto component : components) {
            findChildWithID(component)->setVisible(!help_state);
        }
    }
}

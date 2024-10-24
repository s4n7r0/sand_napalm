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

    invert_text = juce::String("invert phase: Inverts phase of every even copy");
    midi_text = juce::String("MIDI:           Uses midi input instead of given range");
    amount_text = juce::String("amount:       Shift copies by an amount");
    range_text = juce::String("range:          Max range to which copies can be shifted to");
    copies_text = juce::String("copies:         How many copies to create");
    contact_text = juce::String("If you have any issues, please contact me on discord: .sandr0");
    version_text = juce::String(std::string("Ver:") + NAPALM_VER);

    help_texts.push_back(invert_text);
    help_texts.push_back(midi_text);
    help_texts.push_back(amount_text);
    help_texts.push_back(range_text);
    help_texts.push_back(copies_text);
    help_texts.push_back(contact_text);
    help_texts.push_back(version_text);

    help.setButtonText("?");
    help.setBounds(getWidth() - 35, 10, 25, 25);
    help.setColour(1, napalm::colours::component_background);

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
    set_font_size(g, 15);
    g.drawFittedText(juce::String("amount"), amount_text_bounds, juce::Justification::right, 4, 0);
    g.drawFittedText(juce::String("range"), multiplier_text_bounds, juce::Justification::right, 4, 0);
    g.drawFittedText(juce::String("copies"), copies_text_bounds, juce::Justification::right, 4, 0);

    if (help.isMouseOver()) {

        if (!help_state) help_state = true;

        g.fillAll (napalm::colours::background);
        set_font_size(g, 20);
        g.drawSingleLineText("NAPALM", getWidth() / 2, 25, juce::Justification::centred);
        set_font_size(g, 12);
        int text_y = 30;

        for (int i = 0; i < help_texts.size()-2; ++i) {
            g.drawFittedText(help_texts[i], juce::Rectangle<int>(25, text_y + ((text_y / 1.5) * i), getWidth(), 25), juce::Justification::left, 1, 1);
        }

        g.drawFittedText(contact_text,  juce::Rectangle<int>(25, getHeight()/1.4f, getWidth(), 25), juce::Justification::left, 1, 1);
        g.drawFittedText(version_text,  juce::Rectangle<int>(0, getHeight() - text_y, getWidth(), 25), juce::Justification::centred, 1, 1);

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
    for (auto component : components) {
        findChildWithID(component)->setVisible(!help_state);
    }
}

inline void NapalmAudioProcessorEditor::set_font_size(juce::Graphics& g, float new_size) {
    text_size = new_size;
    g.setFont(text_size);
}

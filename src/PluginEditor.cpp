/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NapalmAudioProcessorEditor::NapalmAudioProcessorEditor (NapalmAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), help_state{false},
      delay_time(p, "amount"),
      time_multiplier(p, "multiplier"),
      copies(p, "copies"),
      invert(p, "invert"),
      midi(p, "midi")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 200);
    setResizable(false, false);
    setRepaintsOnMouseActivity(true);
    getLookAndFeel().setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour(juce::uint32(0xff303030)));
    getLookAndFeel().setColour(juce::Slider::ColourIds::trackColourId, juce::Colour(juce::uint32(0xffbfbfbf)));
    getLookAndFeel().setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour(juce::uint32(0xffd0d0d0)));
    getLookAndFeel().setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour(juce::uint32(0x00000000)));
    getLookAndFeel().setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(juce::uint32(0xff303030)));

    tooltip_text = std::string(
        "Hello!\n\nThis is\nNot A Phaser ALthough Maybe\naka \"NAPALM\"\n\n\If you have any issues,\nplease contact me on discord: .sandr0\n\Ver: ") + NAPALM_VER;

    help.setButtonText("?");
    help.setBounds(getWidth() - 35, 10, 25, 25);
    help.setColour(29, juce::Colour(juce::uint32(0xff181818)));
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
    g.fillAll (juce::Colour(juce::uint32(0xff181818)));

    g.setColour (juce::Colours::whitesmoke);
    g.setFont(15);
    
    if (help.isMouseOver()) {

        if (!help_state) {
            help_state = true;
        }

        g.fillAll (juce::Colour(juce::uint32(0xff181818)));
        g.drawFittedText(tooltip_text, juce::Rectangle<int>(0, 25, getWidth(), getHeight() / 1.5), juce::Justification::centred, 10, 0);
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
    //change this to iterate thru a components vector
    delay_time.slider.setVisible(!help_state);
    time_multiplier.slider.setVisible(!help_state);
    copies.slider.setVisible(!help_state);
    midi.button.setVisible(!help_state);
    invert.button.setVisible(!help_state);
}

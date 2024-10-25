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
      pitch(p, "pitch", components),
      copies(p, "copies", components),
      invert(p, "invert", components),
      midi(p, "midi", components),
      help("?")
{

    using SliderIds = juce::Slider::ColourIds;
    using TextButtonIds = juce::TextButton::ColourIds;

    // function alias
    auto temp_laf = std::mem_fn(&NapalmAudioProcessorEditor::getLookAndFeel);
    auto LAF = std::bind(temp_laf, this); 

    setSize (size_width, size_height);
    setResizeLimits(size_width, size_height, size_width * 4, size_height * 4); //why would anyone want it this large...
    setResizable(true, true);
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

    help.set_bounds({ getWidth() - 35, 10, 25, 25 });
    help.button.setColour(1, napalm::colours::component_background);

    invert.button.setButtonText("invert phase");
    invert.set_bounds({ 37, 10, 100, 25 });

    midi.button.setButtonText("midi");
    midi.set_bounds({ 137, 10, 100, 25 });

    delay_time.set_bounds({ 25, 35, (int)(getWidth() / 1.25), 50 });
    delay_time.slider.setNumDecimalPlacesToDisplay(3);
    delay_time.slider.setTextBoxStyle(delay_time.slider.getTextBoxPosition(), 0, 50, 25);

    delay_time.slider.setRange({ napalm::bool_range.getStart(), napalm::bool_range.getEnd() }, 0.0001);
    //delay_time.slider.setSkewFactor(2.f);

    time_multiplier.set_bounds({ 25, 85, (int)(getWidth() / 1.25), 50 });
    time_multiplier.slider.setNumDecimalPlacesToDisplay(0);
    time_multiplier.slider.setTextBoxStyle(time_multiplier.slider.getTextBoxPosition(), 0, 50, 25);

    time_multiplier.slider.setRange({ napalm::multiplier_range.getStart(), napalm::multiplier_range.getEnd() }, 1);

    pitch.set_bounds({ 25, 85, (int)(getWidth() / 1.25), 50 });
    pitch.slider.setNumDecimalPlacesToDisplay(0);
    pitch.slider.setTextBoxStyle(time_multiplier.slider.getTextBoxPosition(), 0, 50, 25);
    
    pitch.slider.setRange({ napalm::pitch_range.getStart(), napalm::pitch_range.getEnd() }, 0.01f);
    pitch.slider.onDragEnd = [&]() {pitch.slider.setValue(0); };
    //time_multiplier.slider.setSkewFactor(.5f);

    copies.set_bounds({ 25, 135, (int)(getWidth() / 1.25), 50 });
    copies.slider.setRange({ napalm::copies_range.getStart(), napalm::copies_range.getEnd() }, 1);
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
    addChildComponent(pitch.slider          , 0);
    addChildComponent(copies.slider         , 0);
    addAndMakeVisible(midi.button           , 0);
    addAndMakeVisible(invert.button         , 0);
    addAndMakeVisible(help.button           , 0);

}

NapalmAudioProcessorEditor::~NapalmAudioProcessorEditor()
{
}

//==============================================================================
void NapalmAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    float x_scale = getWidth() / size_width;
    float y_scale = getHeight() / size_height;
    float abs_scale = std::log2f(std::fabs(((size_width - getWidth()) / size_width) * ((size_height - getHeight()) / size_height)) + 2.f);
    int slider_width = getWidth() - (size_width / 5) * abs_scale;

    g.fillAll (napalm::colours::background);
    g.setColour (napalm::colours::text);
    set_font_size(g, 15 * abs_scale);

    auto temp_bounds = amount_text_bounds;
    temp_bounds *= abs_scale;
    temp_bounds.setWidth(slider_width);
    g.drawFittedText(juce::String("amount"), temp_bounds, juce::Justification::right, 4, 0);

    temp_bounds = multiplier_text_bounds;
    temp_bounds *= abs_scale;
    temp_bounds.setWidth(slider_width);

    if (midi.button.getToggleState()) {
        g.drawFittedText(juce::String("pitch"), temp_bounds, juce::Justification::right, 4, 0);
    }
    else {
        g.drawFittedText(juce::String("range"), temp_bounds, juce::Justification::right, 4, 0);
    }

    temp_bounds = copies_text_bounds;
    temp_bounds *= abs_scale;
    temp_bounds.setWidth(slider_width);
    g.drawFittedText(juce::String("copies"), temp_bounds, juce::Justification::right, 4, 0);

    if (help.button.isMouseOver()) {

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

    if (!help_state) {
        if (midi.button.getToggleState()) {
            time_multiplier.slider.setVisible(false);
            pitch.slider.setVisible(true);
        }
        else {
            time_multiplier.slider.setVisible(true);
            pitch.slider.setVisible(false);
            //addAndMakeVisible(pitch.slider, 0);
        }
    }
}

void NapalmAudioProcessorEditor::resized()
{   
    float x_scale = getWidth() / size_width;
    float y_scale = getHeight() / size_height;
    float abs_scale = std::log2f(std::fabs(((size_width - getWidth()) / size_width) * ((size_height - getHeight()) / size_height)) + 2.f);
    int slider_width = getWidth() - (size_width / 5) * abs_scale;

    auto temp_bounds = help.original_bounds * abs_scale;
    temp_bounds.setX(getWidth() - 35 * abs_scale);
    help.button.setBounds(temp_bounds);

    temp_bounds = delay_time.original_bounds * abs_scale;
    temp_bounds.setWidth(slider_width);
    delay_time.slider.setBounds(temp_bounds);

    temp_bounds = time_multiplier.original_bounds * abs_scale;
    temp_bounds.setWidth(slider_width);
    time_multiplier.slider.setBounds(temp_bounds);

    temp_bounds = copies.original_bounds * abs_scale;
    temp_bounds.setWidth(slider_width);
    copies.slider.setBounds(temp_bounds);
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

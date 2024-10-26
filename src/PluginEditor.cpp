/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "NapalmEditor.h"

using namespace napalm;
//==============================================================================
NapalmAudioProcessorEditor::NapalmAudioProcessorEditor (NapalmAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), help_state{false}, url_timer(help.button),
      amount(p, "amount", components),
      range(p, "range", components),
      pitch(p, "pitch", components),
      pitchmax(p, "pitchmax", components),
      copies(p, "copies", components),
      invert(p, "invert", components),
      midi(p, "midi", components),
      help("?")
{

    setup();

    using namespace components;

    help.set_bounds(help_bounds);
    help.button.setColour(1, colours::component_background);

    help.button.onClick = [&]() { 
            if (my_site.launchInDefaultBrowser()) {
                help.button.setVisible(false);
                url_timer.startTimer(1000);
            } 
        };

    invert.set_bounds(invert_bounds);
    invert.button.setButtonText("invert phase");

    midi.set_bounds(midi_bounds);
    midi.button.setButtonText("midi");
    midi.button.onClick = [&]() {repaint(); };

    amount.set_bounds(amount_bounds);
    amount.slider.setNumDecimalPlacesToDisplay(3);
    amount.slider.setTextBoxStyle(amount.slider.getTextBoxPosition(), 0, 50, 25);
    amount.slider.setRange({ bool_range.getStart(), bool_range.getEnd() }, 0.0001);

    range.set_bounds(range_bounds);
    range.slider.setNumDecimalPlacesToDisplay(0);
    range.slider.setTextBoxStyle(range.slider.getTextBoxPosition(), 0, 50, 25);
    range.slider.setRange({ range_range.getStart(), range_range.getEnd() }, 1);

    pitch.set_bounds(pitch_bounds);
    pitch.slider.setRange({ pitch_range.getStart(), pitch_range.getEnd() }, 0.0001f);
    pitch.slider.setNumDecimalPlacesToDisplay(0);
    pitch.slider.setTextBoxStyle(pitch.slider.getTextBoxPosition(), 1, 50, 25);
    pitch.slider.setTextBoxIsEditable(false);
    pitch.slider.setColour(juce::Slider::textBoxTextColourId, napalm::colours::invisible);
    pitch.slider.onDragEnd = [&]() {pitch.slider.setValue(0); };

    pitchmax.set_bounds(pitchmax_bounds);
    pitchmax.slider.setRange({ pitchmax_range.getStart(), pitchmax_range.getEnd() }, 1.f);
    pitchmax.slider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
    pitchmax.slider.setSliderSnapsToMousePosition(false);
    pitchmax.slider.setTextValueSuffix(" st");
    pitchmax.slider.setColour(juce::Slider::trackColourId, napalm::colours::invisible);
    pitchmax.slider.setColour(juce::Slider::backgroundColourId, napalm::colours::thumb);

    copies.set_bounds(copies_bounds);
    copies.slider.setRange({ copies_range.getStart(), copies_range.getEnd() }, 1);
    copies.slider.setTextBoxStyle(copies.slider.getTextBoxPosition(), 0, 50, 25);

    amount_text_bounds = NapalmBounds(amount.slider.getBounds(), -20);
    range_text_bounds = NapalmBounds(range.slider.getBounds(), -20);
    copies_text_bounds = NapalmBounds(copies.slider.getBounds(), -20);

    addAndMakeVisible(amount.slider     , 0);
    addAndMakeVisible(range.slider, 0);
    addChildComponent(pitch.slider          , 0);
    addChildComponent(pitchmax.slider       , 0);
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
    g.fillAll (colours::background);
    g.setColour (colours::text);
    
    g.setFont(monospace_font);
    set_font_size(g, 15 * abs_scale);

    draw_labels(g);

    draw_help(g);

    show_or_hide();
}

void NapalmAudioProcessorEditor::resized()
{   
    set_scales();

    IRec temp_bounds;
    int abs_slider = int(slider_width / abs_scale);

    midi.button.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));
    invert.button.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));

    temp_bounds = help.original_bounds * abs_scale;
    temp_bounds.setX(getWidth() - 35 * abs_scale);
    help.button.setBounds(temp_bounds);

    temp_bounds = amount.original_bounds;
    temp_bounds.setWidth(abs_slider);
    amount.slider.setBounds(temp_bounds);
    amount.slider.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));

    temp_bounds = range.original_bounds;
    temp_bounds.setWidth(abs_slider);

    range.slider.setBounds(temp_bounds);
    range.slider.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));

    pitch.slider.setBounds(temp_bounds);
    pitch.slider.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));

    temp_bounds = pitchmax.original_bounds;
    pitchmax.slider.setCentrePosition(0, 0);
    pitchmax.slider.setBounds(temp_bounds);
    //i struggled way too much with this...
    pitchmax.slider.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale).translated(getWidth() - 400 * abs_scale, 0));

    temp_bounds = copies.original_bounds;
    temp_bounds.setWidth(abs_slider);
    copies.slider.setBounds(temp_bounds);
    copies.slider.setTransform(juce::AffineTransform::scale(abs_scale, abs_scale));
}

inline void NapalmAudioProcessorEditor::setup() {

    using SliderIds = juce::Slider::ColourIds;
    using TextButtonIds = juce::TextButton::ColourIds;

    setSize(size_width, size_height);
    setResizeLimits(size_width, size_height, size_width * 4, size_height * 4); //why would anyone want it this large...
    setResizable(true, true);
    setRepaintsOnMouseActivity(true);

    getLookAndFeel().setColour(SliderIds::backgroundColourId, colours::component_background);
    getLookAndFeel().setColour(SliderIds::trackColourId, colours::slider_track);
    getLookAndFeel().setColour(SliderIds::thumbColourId, colours::thumb);
    getLookAndFeel().setColour(SliderIds::textBoxOutlineColourId, colours::invisible);
    getLookAndFeel().setColour(TextButtonIds::buttonColourId, colours::component_background);

    help_texts.push_back(invert_text);
    help_texts.push_back(midi_text);
    help_texts.push_back(amount_text);
    help_texts.push_back(range_text);
    help_texts.push_back(pitch_text);
    help_texts.push_back(copies_text);

    // easter egg...
    hehe.setBounds( { 0, (int)(size_height * 4) - 20, 20, 20 } );
    hehe.setButtonText("?");
    hehe.setColour(juce::TextButton::ColourIds::buttonColourId, napalm::colours::invisible);
    hehe.setColour(juce::ComboBox::ColourIds::outlineColourId, napalm::colours::invisible);
    hehe.onClick = []() {juce::URL("https://i.kym-cdn.com/photos/images/newsfeed/002/564/498/74a.jpg").launchInDefaultBrowser(); };
    
    addChildComponent(hehe);
}

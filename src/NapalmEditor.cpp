/*
  ==============================================================================

    NapalmEditor.cpp
    Created: 25 Oct 2024 7:20:10pm
    Author:  MSI

  ==============================================================================
*/

#include "PluginEditor.h"

using namespace napalm;

void NapalmAudioProcessorEditor::show_or_hide() {

	for (auto component : components) {
		findChildWithID(component)->setVisible(!help_state);
	}

	if (!help_state) {
		if (midi.button.getToggleState()) {
			time_multiplier.slider.setVisible(false);
			pitch.slider.setVisible(true);
			pitchmax.slider.setVisible(true);
		}
		else {
			time_multiplier.slider.setVisible(true);
			pitch.slider.setVisible(false);
			pitchmax.slider.setVisible(false);
		}
	}

}

void NapalmAudioProcessorEditor::set_font_size(juce::Graphics& g, float new_size) {
	text_size = new_size;
	g.setFont(text_size);
}

void NapalmAudioProcessorEditor::set_scales() {
	x_scale = getWidth() / size_width;
	y_scale = getHeight() / size_height;
	abs_scale = std::log2f(std::fabs(((size_width - getWidth()) / size_width) * ((size_height - getHeight()) / size_height)) + 2.f);
	slider_width = getWidth() - (size_width / 5) * abs_scale;
}

void NapalmAudioProcessorEditor::draw_labels(juce::Graphics& g) {

	using namespace napalm;

	IRec temp_bounds = amount_text_bounds.bounds;
	temp_bounds *= abs_scale;
	temp_bounds.setWidth(slider_width);
	g.drawFittedText(juce::String("amount"), temp_bounds, juce::Justification::right, 4, 0);

	temp_bounds = multiplier_text_bounds.bounds;
	temp_bounds *= abs_scale;
	temp_bounds.setWidth(slider_width);

	IRec pitch_bounds = IRec({ 25, 85, 50, 50 }) *= abs_scale;
	float pitch_st = pitchmax.slider.getValue() * pitch.slider.getValue() + 0.001; // so it doesn't show - with 0

	if (midi.button.getToggleState()) {
		g.drawFittedText(juce::String("pitch"), temp_bounds, juce::Justification::right, 4, 0);
		set_font_size(g, 13 * abs_scale);
		g.drawFittedText(juce::String().formatted("%2.1f", pitch_st), pitch_bounds, juce::Justification::centred, 1, 1.f);
		set_font_size(g, 15 * abs_scale);
	}
	else {
		g.drawFittedText(juce::String("range"), temp_bounds, juce::Justification::right, 4, 0);
	}

	temp_bounds = copies_text_bounds.bounds;
	temp_bounds *= abs_scale;
	temp_bounds.setWidth(slider_width);
	g.drawFittedText(juce::String("copies"), temp_bounds, juce::Justification::right, 4, 0);
}

void NapalmAudioProcessorEditor::draw_help(juce::Graphics& g) {

	if (help.button.isMouseOver()) {

		if (!help_state) help_state = true;

		g.fillAll(colours::background);
		set_font_size(g, 12 * abs_scale);

		int text_y = 30 * abs_scale;

		for (int i = 0; i < help_texts.size(); ++i) {
			g.drawFittedText(help_texts[i], juce::Rectangle<int>(25, text_y - 25 + ((text_y / 1.5) * i), getWidth(), 25), juce::Justification::left, 1, 1);
		}

		IRec contact_text_bounds = juce::Rectangle<int>(0, getHeight() - text_y * 2, getWidth(), 25);
		IRec version_text_bounds = juce::Rectangle<int>(0, getHeight() - text_y, getWidth(), 25);

		g.drawFittedText(contact_text, contact_text_bounds, juce::Justification::centred, 1, 1);
		g.drawFittedText(version_text, version_text_bounds, juce::Justification::centred, 1, 1);

	}
	else if (help_state) {
		help_state = false;
	}
}
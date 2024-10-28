#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "NapalmProcessor.h"

void napalm::Processor::fill_buffer(juce::AudioBuffer<float>& input_buffer) {

	int num_samples = input_buffer.getNumSamples();

	for (auto channel = 0; channel < input_buffer.getNumChannels(); ++channel) {
		jassert(buffer_pos == buffer[channel].size());
		buffer[channel].insertArray(-1, input_buffer.getReadPointer(channel), num_samples);
	}

	buffer_pos += num_samples;

	// even though max range is wayyy below max samples in buffer, we still need space for very low midi notes
	if (buffer_pos >= napalm::MAX_SAMPLES_IN_BUFFER * 2) {
		for (auto channel = 0; channel < input_buffer.getNumChannels(); ++channel) {
			buffer[channel].removeRange(0, napalm::MAX_SAMPLES_IN_BUFFER);
			buffer_pos = buffer[channel].size();
		}
	}
}


void napalm::Processor::process(juce::AudioBuffer<float>& input_buffer,const napalm::APVTS& apvts) {

	if (buffer_pos <= amount) return;

	set_amount(*apvts.getRawParameterValue("amount"));
	set_copies(*apvts.getRawParameterValue("copies"));
	set_range(*apvts.getRawParameterValue("range"));
	const int invert_phase = *apvts.getRawParameterValue("invert"); //0 to 1 -> -1 to 1
	const int num_samples = input_buffer.getNumSamples();

	amount = smooth_amount.skip(num_samples);
	//higher sample rates means there are more samples stored in buffer (duh)
	amount *= smooth_range.skip(num_samples) * adjust_for_sample_rate;
	delay_copies = smooth_copies.skip(num_samples);
	midi_note_length = smooth_pitch.skip(num_samples);

	jassert(midi_note_length < MAX_SAMPLES_IN_BUFFER);

	// area between <buffer_pos - num_samples , buffer_pos> is current input block we don't care about
	const int prev_samples = buffer_pos - num_samples; 
	const float gain = 1.f / delay_copies;
	float invert_gain = 1;

	for (int channel = 0; channel < input_buffer.getNumChannels(); ++channel) {

		input_buffer.applyGain(channel, 0, num_samples, gain);

		//go to the end of our buffer
		auto cur_channel = buffer[channel].getRawDataPointer() + prev_samples; 

		//creates n copies down from buffer_pos up to zero, think of it like a delay
		for (int i = 1; i < delay_copies; ++i) {

			if (invert_phase) invert_gain = (((i % 2) - 0.5) * -2);

			//divide into equally spaced parts
			int offset = std::round((amount / delay_copies) * i);
			input_buffer.addFrom(channel, 0, cur_channel - offset, num_samples, gain * invert_gain);
		}
	}
}

inline void napalm::Processor::set_amount(float value) {
	smooth_amount.setTargetValue(value);
}

inline void napalm::Processor::set_range(float value = 0) {
	if (midi_input) {
		smooth_range.setTargetValue(midi_note_length);
	} else {
		smooth_range.setTargetValue(value);
	}
}

inline void napalm::Processor::set_copies(int value) {
	smooth_copies.setTargetValue(value);
}

void napalm::Processor::midi_switch(bool value) {
	midi_input = value;

	if (midi_input) {
		smooth_reset(0);
	}
	else {
		smooth_reset(smooth_target);
	}
}

void napalm::Processor::midi_set_note(juce::MidiMessage midi) {
	midi_note = midi.getNoteNumber();
}

void napalm::Processor::midi_set_length(double pitch) {
	const double hz = mtod(midi_note + pitch);
	const double length = (1 / hz) * sample_rate / adjust_for_sample_rate; //convert frequency to length of one cycle in samples

	smooth_pitch.setTargetValue(length);
	set_range();
}

inline void napalm::Processor::smooth_reset(float target) {
	smooth_range.reset(sample_rate, target);
	smooth_amount.reset(sample_rate, target);
	smooth_copies.reset(sample_rate, target / 10);
	smooth_pitch.reset(sample_rate, target / 10);
}
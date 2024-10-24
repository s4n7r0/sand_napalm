#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "NapalmProcessor.h"

napalm::Processor::Processor() {}

void napalm::Processor::fill_buffer(juce::AudioBuffer<float>& input_buffer) {

	int num_samples = input_buffer.getNumSamples();

	for (auto channel = 0; channel < input_buffer.getNumChannels(); ++channel) {
		buffer[channel].insertArray(-1, input_buffer.getReadPointer(channel), num_samples);
	}

	buffer_pos += num_samples;

	if (buffer_pos >= napalm::MAX_SAMPLES_IN_BUFFER * 2) {
		for (auto channel = 0; channel < input_buffer.getNumChannels(); ++channel) {
			buffer[channel].removeRange(0, napalm::MAX_SAMPLES_IN_BUFFER);
			buffer_pos = buffer[channel].size();
		}
	}
}


void napalm::Processor::process(juce::AudioBuffer<float>& input_buffer,const napalm::APVTS& apvts) {

	set_delay(*apvts.getRawParameterValue("amount"));
	set_copies(*apvts.getRawParameterValue("copies"));
	set_multiplier(*apvts.getRawParameterValue("multiplier"));
	const int invert_phase = *apvts.getRawParameterValue("invert"); //0 to 1 -> -1 to 1

	delay_time = smooth_time.getNextValue();
	delay_time *= smooth_multiplier.getNextValue();

	if (buffer_pos <= delay_time) return;
	if (delay_counter >= buffer_pos) delay_counter = 0;

	const int num_samples = input_buffer.getNumSamples();
	const int prev_samples = buffer_pos - num_samples;
	const float gain = 1.f / delay_copies;
	float invert_gain = 1;

	for (int channel = 0; channel < input_buffer.getNumChannels(); ++channel) {

		input_buffer.applyGain(channel, 0, num_samples, gain);

		auto* channelData = input_buffer.getWritePointer(channel);
		auto cur_channel = buffer[channel].getRawDataPointer() + prev_samples;

		for (int i = 1; i < delay_copies; ++i) {
			//creates n copies in between buffer_pos and delay_time
			if (invert_phase) invert_gain = (((i % 2) - 0.5) * -2);

			int offset = std::roundl((delay_time / delay_copies) * i);
			input_buffer.addFrom(channel, 0, cur_channel - offset, num_samples, gain * invert_gain);
		}
	}
}

inline void napalm::Processor::set_delay(float value) {
	smooth_time.setTargetValue(value);
}

void napalm::Processor::set_multiplier(float value = 0) {
	if (midi_input) {
		smooth_multiplier.setTargetValue(midi_note_length);
	} else {
		smooth_multiplier.setTargetValue(value);
	}
}

inline void napalm::Processor::set_copies(int value) {
	delay_copies = value;
}

void napalm::Processor::midi_switch(bool value) {
	midi_input = value;
	if (midi_input) {
		smooth_multiplier.reset(sample_rate, 0);
		smooth_time.reset(sample_rate, 0);
	}
	else {
		smooth_multiplier.reset(sample_rate, 0.001);
		smooth_time.reset(sample_rate, 0.001);
	}
}

void napalm::Processor::midi_set_length(double period) {
	midi_note_length = period;
	set_multiplier();
}
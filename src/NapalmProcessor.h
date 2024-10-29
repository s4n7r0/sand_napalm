#pragma once
#include "NapalmParams.h"

namespace napalm {

	const float smooth_target = 0.01;

	class Processor {
	public:
		Processor() {};

		void fill_buffer(juce::AudioBuffer<float>& buffer);

		void process(juce::AudioBuffer<float>& buffer, const napalm::APVTS&);
		inline void set_amount(float);
		inline void set_copies(int);
		inline void set_range(float);

		void midi_switch(bool);
		void midi_set_length(double);
		void midi_set_note(juce::MidiMessage);

		//thank you Ben Vining
		double mtod(double midi) {
			return 440 * std::pow(2, ((midi - 69) / 12));
		}

		void smooth_reset(float);

		bool midi_input{ false };
		float midi_note{ 0 };

		double sample_rate{ 44100 }; //who would even use 22050 hz?
		float adjust_for_sample_rate{ 1 };

	private:

		std::array<juce::Array<float>, 2> buffer;
		int buffer_pos{ 0 };

		float amount{ 0 };
		int delay_counter{ 0 };
		float delay_copies{ 1 };
		float amount_range{ 1 };

		juce::LinearSmoothedValue<float> smooth_amount;
		juce::LinearSmoothedValue<float> smooth_range;
		juce::LinearSmoothedValue<float> smooth_copies;
		juce::LinearSmoothedValue<double> smooth_pitch;

		double midi_note_length{ 0 };

	};
}
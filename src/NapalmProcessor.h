#pragma once
#include "NapalmParams.h"

#define NAPALM_VER "0.8.0"

namespace napalm {

	const float smooth_target = 0.05;

	class Processor {
	public:
		Processor();

		void fill_buffer(juce::AudioBuffer<float>& buffer);

		void process(juce::AudioBuffer<float>& buffer, const napalm::APVTS&);
		inline void set_delay(float);
		inline void set_copies(int);
		inline void set_multiplier(float);

		void midi_switch(bool);
		void midi_set_length(double);

		juce::LinearSmoothedValue<float> smooth_time;
		juce::LinearSmoothedValue<float> smooth_multiplier;
		juce::LinearSmoothedValue<float> smooth_copies;
		inline void smooth_reset(float);

		bool midi_input{ false };
		float sample_rate;

	private:

		std::array<juce::Array<float>, 2> buffer;
		int buffer_pos{ 0 };

		float delay_time{ 0 };
		int delay_counter{ 0 };
		float delay_copies{ 1 };
		float delay_time_multiplier{ 1 };

		double midi_note_length{ 0 };

	};
}
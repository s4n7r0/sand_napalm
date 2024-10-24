#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

//thank you Beats Basteln

namespace napalm
{
	const int MAX_SAMPLES_IN_BUFFER = 4096 * 4;

	using APVTS = juce::AudioProcessorValueTreeState;
	using Layout = APVTS::ParameterLayout;
	using P = APVTS::Parameter;
	using UniqueP = std::unique_ptr<P>;
	using UniquePVector = std::vector<UniqueP>;
	using NRange = juce::NormalisableRange<float>;
	using Attributes = juce::AudioProcessorValueTreeStateParameterAttributes;
	using paramID = juce::ParameterID;

	inline void add_params(UniquePVector& params) {

		const auto string_from_val = [](float value, int max_length) {
			return juce::String(value, max_length);
			};

		const auto val_from_string = [](juce::String value) {
			return std::stod(value.toStdString());
			};

		params.push_back( std::make_unique<APVTS::Parameter>(paramID{"amount", 0}, "Amount", NRange{0.f, 1.f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "multiplier", 1 }, "Multiplier", NRange{ 0.f, napalm::MAX_SAMPLES_IN_BUFFER }, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "copies", 2 }, "Copies", NRange{1, 32, 1}, 1,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "invert", 3 }, "Invert Phase", NRange{0, 1, 1}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
				.withBoolean(true)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "midi", 4 }, "MIDI Input", NRange{0, 1, 1}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
				.withBoolean(true)
			)
		);
	}

	inline Layout create_layout() {
		UniquePVector params;

		add_params(params);

		return { params.begin(), params.end() };
	}
}
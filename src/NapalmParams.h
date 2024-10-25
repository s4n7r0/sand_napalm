#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

//thank you Beats Basteln

namespace napalm
{
	const int MAX_SAMPLES_IN_BUFFER = 4096 * 4;

	const juce::Range<float> bool_range({ 0, 1 });
	const juce::Range<float> a_range({ 0, 1 });
	const juce::Range<float> multiplier_range({ 0, MAX_SAMPLES_IN_BUFFER - 1 }); //lol
	const juce::Range<float> copies_range({ 1, 32 });
	const juce::Range<float> pitch_range({ -1, 1 });
	const juce::Range<float> pitchmax_range({ 1, 48 });

	using APVTS = juce::AudioProcessorValueTreeState;
	using Layout = APVTS::ParameterLayout;
	using P = APVTS::Parameter;
	using UniqueP = std::unique_ptr<P>;
	using UniquePVector = std::vector<UniqueP>;
	using NRange = juce::NormalisableRange<float>;
	using Attributes = juce::AudioProcessorValueTreeStateParameterAttributes;
	using paramID = juce::ParameterID;

	enum PARAMS_IDS {amount, multiplier, pitch, pitchmax, copies, invert, midi};

	inline void add_params(UniquePVector& params) {

		const auto string_from_val = [](float value, int max_length) {
			return juce::String(value, max_length);
			};

		const auto val_from_string = [](juce::String value) {
			return std::stod(value.toStdString());
			};

		params.push_back( std::make_unique<APVTS::Parameter>(paramID{"amount", amount}, "Amount", 
			NRange{ bool_range.getStart(), bool_range.getEnd(), 0.0000001f, 2}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "multiplier", multiplier }, "Multiplier", 
			NRange{multiplier_range.getStart(), multiplier_range.getEnd(), 0.1f, 0.5f}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "pitch", pitch }, "Pitch", NRange{pitch_range, 0.0001f}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);
		
		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "pitchmax", pitchmax }, "Pitch Range", NRange{pitchmax_range, 1.f}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "copies", copies }, "Copies", NRange{copies_range, 1}, 1,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "invert", invert }, "Invert Phase", NRange{bool_range, 1}, 0,
			Attributes()
				.withStringFromValueFunction(string_from_val)
				.withValueFromStringFunction(val_from_string)
				.withBoolean(true)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "midi", midi }, "MIDI Input", NRange{bool_range, 1}, 0,
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
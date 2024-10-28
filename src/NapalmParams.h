#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

//thank you Beats Basteln

namespace napalm
{
	const int MAX_SAMPLES_RANGE = 4096 * 4 + 1; //adding one so it doesn't click when range is set to max
	const int MAX_SAMPLES_IN_BUFFER = MAX_SAMPLES_RANGE * 8; // (2 << 16) + 4
	const float size_width = 400;
	const float size_height = 200;

	const juce::Range<float> bool_range({ 0, 1 });
	const juce::Range<float> range_range({ 0, MAX_SAMPLES_RANGE - 1 }); //lol
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

	enum PARAMS_IDS {amount, range, pitch, pitchmax, copies, invert, midi, help}; //also components ids

	inline void add_params(UniquePVector& params) {

		//this is stupid, idk of any better way
		const auto string_from_val_0d = [](float value, int max_length) { return juce::String(value, 0); };
		const auto string_from_val_2d = [](float value, int max_length) { return juce::String(value, 2); };
		const auto string_from_val_4d = [](float value, int max_length) { return juce::String(value, 4); };

		const auto val_from_string = [](juce::String value) {
			return std::stod(value.toStdString());
			};

		params.push_back( std::make_unique<APVTS::Parameter>(paramID{"amount", amount}, "Amount", 
			NRange{ bool_range.getStart(), bool_range.getEnd(), 0.0000001f, 2.f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_4d)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "range", range, }, "Range", 
			NRange{range_range.getStart(), range_range.getEnd(), 0.1f, 0.5f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_0d)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "pitch", pitch }, "Pitch", NRange{pitch_range, 0.0001f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_2d)
				.withValueFromStringFunction(val_from_string)
			)
		);
		
		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "pitchmax", pitchmax }, "Pitch Range", NRange{pitchmax_range, 1.f}, 1.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_0d)
				.withValueFromStringFunction(val_from_string)
			)
		);

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "copies", copies }, "Copies", NRange{copies_range, 1.f}, 1.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_0d)
				.withValueFromStringFunction(val_from_string)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "invert", invert }, "Invert Phase", NRange{bool_range, 1.f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_0d)
				.withValueFromStringFunction(val_from_string)
				.withBoolean(true)
			)
		);		

		params.push_back(std::make_unique<APVTS::Parameter>(paramID{ "midi", midi }, "MIDI Input", NRange{bool_range, 1.f}, 0.f,
			Attributes()
				.withStringFromValueFunction(string_from_val_0d)
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
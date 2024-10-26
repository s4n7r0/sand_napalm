#pragma once

#include "PluginEditor.h"

namespace napalm {

	const juce::String NAPALM_VER = "0.9.9";

	using IRec = juce::Rectangle<int>;

	const float size_width = 400;
	const float size_height = 200;

	namespace colours {

		using Colour = juce::Colour;
		using ARGB = juce::uint32;

		const Colour background(Colour(ARGB(0xff181818))); // #181818
		const Colour component_background(Colour(ARGB(0xff303030))); // #303030
		const Colour thumb(Colour(ARGB(0xffd0d0d0))); // #d0d0d0
		const Colour slider_track(Colour(ARGB(0xffbfbfbf))); // #bfbfbf
		const Colour invisible(Colour(ARGB(0x0))); 
		const Colour text(Colour(ARGB(0xfff0f0f0))); // #f0f0f0

	}

	namespace components {

		struct ComponentBounds {

			virtual void set_bounds(juce::Rectangle<int> input) { original_bounds = input; };
			juce::Rectangle<int> get_bounds() { return original_bounds; }

			juce::Rectangle<int> original_bounds;
		};

		struct TextButtonBounds : ComponentBounds {

			TextButtonBounds(juce::String text) : button{ juce::TextButton(text) } { }

			void set_bounds(juce::Rectangle<int> input) override { original_bounds = input; button.setBounds(input); };

			juce::TextButton button;
		};

		struct AttachedSlider : ComponentBounds {

			AttachedSlider(NapalmAudioProcessor& p, juce::String paramid, std::vector<juce::String>& comps)
				: slider(), attachment(*p.apvts.getParameter(paramid), slider, &p.undo)
			{
				slider.setComponentID(paramid);
				slider.setColour(juce::ToggleButton::ColourIds::textColourId, colours::text);
				comps.push_back(paramid);
			}

			void set_bounds(juce::Rectangle<int> input) override { original_bounds = input; slider.setBounds(input); };

			juce::Slider slider;
			juce::SliderParameterAttachment attachment;
		};

		struct AttachedToggleButton : ComponentBounds {

			AttachedToggleButton(NapalmAudioProcessor& p, juce::String paramid, std::vector<juce::String>& comps)
				: button(p.apvts.getParameter(paramid)->getLabel()), attachment(*p.apvts.getParameter(paramid), button, &p.undo)
			{
				button.setComponentID(paramid);
				button.setColour(juce::ToggleButton::ColourIds::textColourId, colours::text);
				comps.push_back(paramid);
			}

			void set_bounds(juce::Rectangle<int> input) override { original_bounds = input; button.setBounds(input); };

			juce::ToggleButton button;
			juce::ButtonParameterAttachment attachment;

		};

		class URLTimer : public juce::Timer {
			public:

				URLTimer(juce::Component& x) : component{ x } {}

				void timerCallback() override {
					component.setVisible(true);
				}

			private:
				juce::Component& component;
		};

		const IRec help_bounds({ (int)size_width - 35, 10, 25, 25 });
		const IRec invert_bounds({ 37, 10, 100, 25 });
		const IRec midi_bounds({ 137, 10, 100, 25 });
		const IRec amount_bounds({ 25, 35, (int)(size_width / 1.25), 50 });
		const IRec range_bounds({ 25, 85, (int)(size_width / 1.25), 50 });
		const IRec pitch_bounds({ 25, 85, (int)(size_width / 1.25), 50});
		const IRec pitchmax_bounds({(int)(size_width / 1.175), 85, 50, 50 });
		const IRec copies_bounds({ 25, 135, (int)(size_width / 1.25), 50 });

	}

	struct NapalmBounds {
		NapalmBounds() : bounds() {}
		NapalmBounds(IRec x) : bounds(x) {}

		// Add xy to both x and y
		NapalmBounds(IRec x, int xy) 
			: bounds(x) { bounds.setX(bounds.getX() + xy); bounds.setY(bounds.getY() + xy); };

		IRec get() { return bounds; }
		IRec set(IRec x) { bounds = x; }

		IRec bounds;
	};

	//call set_font after using
	const juce::Font monospace_font(juce::Font::Font(juce::Font::getDefaultMonospacedFontName(), 0, juce::Font::plain));

	const juce::String invert_text		("invert phase: Inverts every even copy");
	const juce::String midi_text		("midi:         Uses midi input instead of range");
	const juce::String amount_text		("amount:		     Shift copies by an amount"); //idk why this one doesn't want to behave
	const juce::String range_text		("range:        Max shift range (in samples)");
	const juce::String pitch_text		("pitch:        Shifts copies by semitones");
	const juce::String copies_text		("copies:       How many copies to create");
	const juce::String contact_text		("If you have any issues, please contact me on \ndiscord: .sandr0");
	const juce::String version_text		("Ver: " + NAPALM_VER);
	const juce::URL    my_site		    ("https://www.github.com/s4n7r0/sand_napalm");

}


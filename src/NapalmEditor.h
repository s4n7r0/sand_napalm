#pragma once

#include "PluginEditor.h"

namespace napalm {
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

}
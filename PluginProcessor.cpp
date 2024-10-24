/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NapalmAudioProcessor::NapalmAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : buffer_size{ 0 }, undo(), 
    apvts(*this, &undo, "PARAMETERS", napalm::create_layout()),
    AudioProcessor(BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

NapalmAudioProcessor::~NapalmAudioProcessor()
{
}

//==============================================================================
const juce::String NapalmAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NapalmAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NapalmAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NapalmAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NapalmAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NapalmAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NapalmAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NapalmAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NapalmAudioProcessor::getProgramName (int index)
{
    return {};
}

void NapalmAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NapalmAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    napalm_processor.sample_rate = sampleRate;
    napalm_processor.smooth_time.reset(sampleRate, 0.001);
    napalm_processor.smooth_multiplier.reset(sampleRate, 0.001);
}

void NapalmAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NapalmAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NapalmAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    napalm_processor.fill_buffer(buffer);
    napalm_processor.midi_switch((bool)*apvts.getRawParameterValue("midi"));

    if (napalm_processor.midi_input && midiMessages.data.size() > 0) {
        juce::MidiMessage message(0xf0);
        juce::MidiBuffer::Iterator i(midiMessages);
        int frame = 0;

        while (i.getNextEvent(message, frame)) {
            if (message.isNoteOnOrOff()) {
                double hz = message.getMidiNoteInHertz(message.getNoteNumber());
                double length = (1 / hz) * getSampleRate(); //convert frequency to length of one cycle in samples
                napalm_processor.midi_set_length(length);
            }
        }
    }

    napalm_processor.process(buffer, apvts);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
    }
}

//==============================================================================
bool NapalmAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NapalmAudioProcessor::createEditor()
{
    return new NapalmAudioProcessorEditor (*this);
}

//==============================================================================
void NapalmAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NapalmAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NapalmAudioProcessor();
}

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TVRATremoloAudioProcessor::TVRATremoloAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    mSpeedParameter = std::make_unique<AudioParameterFloat>("Speed", "Speed", 0.01f, 20.f, 10.f);
    addParameter(mSpeedParameter.get());

    period = 0.f;
    smoothSpeedParam = mSpeedParameter->get();

}

TVRATremoloAudioProcessor::~TVRATremoloAudioProcessor()
{
}

//==============================================================================
const juce::String TVRATremoloAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TVRATremoloAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TVRATremoloAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TVRATremoloAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TVRATremoloAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TVRATremoloAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TVRATremoloAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TVRATremoloAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TVRATremoloAudioProcessor::getProgramName (int index)
{
    return {};
}

void TVRATremoloAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TVRATremoloAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    period = 0.f;
}

void TVRATremoloAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TVRATremoloAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void TVRATremoloAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto* channelLeft = buffer.getWritePointer(0);
    auto* channelRight = buffer.getWritePointer(1);

    for (size_t i = 0; i < buffer.getNumSamples(); i++)
    {
        smoothSpeedParam = smoothSpeedParam + 0.001 * (*mSpeedParameter - smoothSpeedParam);
    
        period += juce::MathConstants<float>::twoPi * smoothSpeedParam / getSampleRate();

        float lfo = sin(period);
        float lfoMapped = jmap(lfo, -1.f, 1.f, 0.f, 1.f);

        channelLeft[i] *= lfoMapped;
        channelRight[i] *= lfoMapped;
    }
}

//==============================================================================
bool TVRATremoloAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TVRATremoloAudioProcessor::createEditor()
{
    return new TVRATremoloAudioProcessorEditor (*this);
}

//==============================================================================
void TVRATremoloAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TVRATremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TVRATremoloAudioProcessor();
}

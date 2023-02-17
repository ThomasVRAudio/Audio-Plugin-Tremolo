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

    mDryWetParameter = std::make_unique<AudioParameterFloat>("DryWet", "DryWet", 0.0f, 1.0f, 0.5f);
    addParameter(mDryWetParameter.get());

    mDepthParameter = std::make_unique<AudioParameterFloat>("Depth", "Depth", 0.0f, 1.0f, 0.5f);
    addParameter(mDepthParameter.get());

    mShapeParameter = std::make_unique<AudioParameterInt>("Shape", "Shape", 0, 2, 0);
    addParameter(mShapeParameter.get());

    mSyncParameter = std::make_unique<AudioParameterInt>("Sync", "Sync", 0, 10, 0);
    addParameter(mSyncParameter.get());
    
    period = 0.0;
    time = 0.0;
    smoothSpeedParam = mSpeedParameter->get();
    smoothLFO = 0.0;
    BPM = 0.0f;
    syncSpeed = 0.0f;

    mSyncToggle = false;
}

TVRATremoloAudioProcessor::~TVRATremoloAudioProcessor()
{
    delete[] mLfoPositions;
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
    period = 0.0;
    smoothLFO = 0.0;

    samplesInMinutes = sampleRate * 60;
    mPpqPositions.resize(samplesPerBlock); //block of 480
    
    if (mLfoPositions == nullptr) {
        mLfoPositions = new float[samplesPerBlock];
        zeromem(mLfoPositions, samplesPerBlock);
    }
 
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

    // ---- new stuff (start)
    auto ppqPerSample = GetPPQPerSample();
    double offset = 0.0;
    // ---- new stuff (end)


    for (size_t i = 0; i < buffer.getNumSamples(); i++)
    {
        if (mSyncToggle)
            setSyncAmount();

        float speed = mSyncToggle ? syncSpeed : *mSpeedParameter;
        smoothSpeedParam = smoothSpeedParam + 0.001 * (speed - smoothSpeedParam);

        period += juce::MathConstants<float>::twoPi * smoothSpeedParam / getSampleRate();

        float lfo;
        float sine = sin(period + phaseOffset); 
        float square = sine > 0 ? 1 : -1;

        time += smoothSpeedParam / getSampleRate();

        switch (*mShapeParameter) {
        case 0:
            lfo = sine;
            break;
        case 1:
            lfo = sine > 0 ? 1: -1;
            break;
        case 2:
            lfo = 4 * abs(time - floor(time + 1 / 2)) -1;
            break;
        default:
            jassertfalse;
            break;
        }

        smoothLFO = smoothLFO + 0.01 * (lfo - smoothLFO);
        float lfoMapped = jmap((float)smoothLFO, -1.f, 1.f, 0.99f - (float)*mDepthParameter, 0.99f);


        updateCurrentTimeInfoFromHost();
        // --- new stuff (begin)
        offset += ppqPerSample;
        mPpqPositions[i] = mPlayHeadInfo.ppqPosition + offset;

        auto relativePosition = fmod(mPpqPositions[i], 1.0); 

        if (relativePosition <= ppqPerSample) { 
            phaseOffset = -period; 
            //quarterNotePosition.store(mPlayHeadInfo.ppqPosition);
        }

        

        // --- new stuff (end)

        float leftOut = buffer.getSample(0, i) * (1 - *mDryWetParameter) + (buffer.getSample(0, i) * lfoMapped) * *mDryWetParameter;
        float rightOut = buffer.getSample(1, i) * (1 - *mDryWetParameter) + (buffer.getSample(1, i) * lfoMapped) * *mDryWetParameter;

        buffer.setSample(0, i, leftOut);
        buffer.setSample(1, i, rightOut);
    }
}

void TVRATremoloAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (AudioPlayHead* ph = getPlayHead()) {

        if (ph->getCurrentPosition(mPlayHeadInfo)) {
            BPM = mPlayHeadInfo.bpm;
            currentPlayHeadPosition.store(mPlayHeadInfo.ppqPosition);
        }
    }
}

double TVRATremoloAudioProcessor::GetPPQPerSample() const {

    auto samplesPerBeat = samplesInMinutes / mPlayHeadInfo.bpm;
    return 1.0 / samplesPerBeat; // calculate musical timing in 1 sample

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
    std::unique_ptr<XmlElement> xml(new XmlElement("Tremolo"));
    xml->setAttribute("DryWet", *mDryWetParameter);
    xml->setAttribute("Depth", *mDepthParameter);
    xml->setAttribute("Speed", *mSpeedParameter);
    copyXmlToBinary(*xml, destData);
}

void TVRATremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);

    if (xml.get() != nullptr && xml->hasTagName("Tremolo")) {
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mDepthParameter = xml->getDoubleAttribute("Depth");
        *mSpeedParameter = xml->getDoubleAttribute("Speed");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TVRATremoloAudioProcessor();
}

bool TVRATremoloAudioProcessor::getSync() {
    return mSyncToggle;
}

void TVRATremoloAudioProcessor::setSync(bool isSynced) {
    mSyncToggle = isSynced;
}

void TVRATremoloAudioProcessor::setSyncAmount()
{

    if (BPM != 0 && mSyncToggle) {
        float BPS = BPM / 60;
        switch ((int)*mSpeedParameter)
        {
        case 0: // 16/1
            syncSpeed = BPS / 64;
            break;
        case 1: // 8/1
            syncSpeed = BPS / 32;
            break;
        case 2: // 4/1
            syncSpeed = BPS / 16;
            break; 
        case 3: // 2/1
            syncSpeed = BPS / 8;
            break; 
        case 4: // 1/1
            syncSpeed = BPS / 4;
            break; 
        case 5: // 1/2
            syncSpeed = BPS / 2;
            break; 
        case 6: // 1/4
            syncSpeed = BPS;
            break;
        case 7: // 1/8
            syncSpeed = BPS / 0.5f;
            break;
        case 8: // 1/16
            syncSpeed = BPS / 0.25f;
            break;
        case 9: // 1/32
            syncSpeed = BPS / 0.125f;
            break;
        case 10: // 1/64
            syncSpeed = BPS / 0.0625f;
            break;
        default:
            jassertfalse;
            break;
        }
    }
}

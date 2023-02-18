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

    addParameter(mSpeedParameter = new AudioParameterFloat("Speed", "Speed", 0.01f, 20.f, 10.f));
    addParameter(mDryWetParameter = new AudioParameterFloat("DryWet", "DryWet", 0.0f, 1.0f, 0.5f));
    addParameter(mDepthParameter = new AudioParameterFloat("Depth", "Depth", 0.0f, 1.0f, 0.5f));
    addParameter(mShapeParameter = new AudioParameterInt("Shape", "Shape", 0, 3, 0));

    addParameter(mSpeedLFOParameter = new AudioParameterFloat("SpeedLFO", "SpeedLFO", 0.01f, 0.5f, 0.5f));
    addParameter(mDryWetLFOParameter = new AudioParameterFloat("DryWetLFO", "DryWetLFO", 1.f, 20.f, 1.f));
    addParameter(mDepthLFOParameter = new AudioParameterFloat("DepthLFO", "DepthLFO", 1.f, 20.f, 1.f));

    addParameter(mSpeedLFODepthParameter = new AudioParameterFloat("SpeedLFODepth", "SpeedLFODepth", 0.f, 1.f, 0.f));
    
    mPeriod = 0.0;
    mTime = 0.0;
    mSmoothSpeedParam = mSpeedParameter->get();
    mSmoothLFO = 0.0;
    mBPM = 0.0f;
    mSyncSpeed = 1.0f;

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
    mPeriod = 0.0;
    mSmoothLFO = 0.0;

    mSamplesInMinutes = sampleRate * 60;
    mPpqPositions.resize(samplesPerBlock); //block of 480
    
    if (mLfoPositions == nullptr) {
        mLfoPositions = new float[samplesPerBlock];
        zeromem(mLfoPositions, samplesPerBlock);
    }

    if (mSyncToggle)
        setSyncAmount();
 
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


    auto ppqPerSample = GetPPQPerSample();
    double offset = 0.0;


    for (size_t i = 0; i < buffer.getNumSamples(); i++)
    {
        float BPS = mBPM != 0 && mSyncToggle ? mBPM / 60.0 : 1.f;

        mSpeedLFOPeriod += juce::MathConstants<float>::twoPi * *mSpeedLFOParameter / getSampleRate();
        float mSpeedLFO = sin(mSpeedLFOPeriod);
        float mSpeedLFOMapped = 1 - *mSpeedLFODepthParameter + *mSpeedLFODepthParameter * jmap(mSpeedLFO, -1.f, 1.f, 0.1f, 1.f);

        float speed = mSyncToggle ? BPS / mSyncSpeed : *mSpeedParameter * mSpeedLFOMapped;
        mSmoothSpeedParam = mSmoothSpeedParam + 0.001 * (speed - mSmoothSpeedParam);

        mPeriod += juce::MathConstants<float>::twoPi * mSmoothSpeedParam / getSampleRate();

        float sine = sin(mPeriod + mPhaseOffset); 


        mTime += mSmoothSpeedParam / getSampleRate();

        float lfo;
        switch (*mShapeParameter) {
        case 0:
            lfo = sine;
            break;
        case 1:
            lfo = sine > 0 ? 1.f: -1.f;
            break;
        case 2:
            lfo = sine > 0.5f ? 1.f : -1.f;
            break;
        case 3:
            lfo = 4.f * abs(mTime - floor(mTime + 1.f / 2.f)) -1.f;
            break;
        default:
            jassertfalse;
            break;
        }

        mSmoothLFO = mSmoothLFO + 0.01f * (lfo - mSmoothLFO);
        float lfoMapped = jmap(mSmoothLFO, -1.f, 1.f, 0.99f - *mDepthParameter, 0.99f);

        updateCurrentTimeInfoFromHost();

        offset += ppqPerSample;
        mPpqPositions[(int)i] = mPlayHeadInfo.ppqPosition + offset;

        auto relativePosition = fmod(mPpqPositions[(int)i], 1.0); 

        if (relativePosition <= ppqPerSample && mSyncToggle && mPlayHeadInfo.isPlaying)
            mPhaseOffset = -mPeriod; 

        float leftOut = buffer.getSample(0, (int)i) * (1 - *mDryWetParameter) + (buffer.getSample(0, (int)i) * lfoMapped) * *mDryWetParameter;
        float rightOut = buffer.getSample(1, (int)i) * (1 - *mDryWetParameter) + (buffer.getSample(1, (int)i) * lfoMapped) * *mDryWetParameter;

        buffer.setSample(0, (int)i, leftOut);
        buffer.setSample(1, (int)i, rightOut);
    }
}

void TVRATremoloAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (AudioPlayHead* ph = getPlayHead()) {

        if (ph->getCurrentPosition(mPlayHeadInfo)) {
            mBPM = mPlayHeadInfo.bpm;
            CurrentPlayHeadPosition.store(mPlayHeadInfo.ppqPosition);
        }
    }
}

double TVRATremoloAudioProcessor::GetPPQPerSample() const {

    auto samplesPerBeat = mSamplesInMinutes / mPlayHeadInfo.bpm;
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
    xml->setAttribute("SyncToggle", mSyncToggle);
    xml->setAttribute("Shape", *mShapeParameter);
    copyXmlToBinary(*xml, destData);
}

void TVRATremoloAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xml = getXmlFromBinary(data, sizeInBytes);

    if (xml.get() != nullptr && xml->hasTagName("Tremolo")) {
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mDepthParameter = xml->getDoubleAttribute("Depth");
        *mSpeedParameter = xml->getDoubleAttribute("Speed");
        mSyncToggle = xml->getBoolAttribute("SyncToggle");
        *mShapeParameter = xml->getIntAttribute("Shape");
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
    if (mBPM != 0 && mSyncToggle) {
        switch ((int)round(*mSpeedParameter))
        {
        case 0: 
            mSyncSpeed = 1;
            break;
        case 1: 
            mSyncSpeed = 0.5f;
            break; 
        case 2:
            mSyncSpeed = 0.5f / 1.5f;
            break; 
        case 3: 
            mSyncSpeed = 0.25f;
            break; 
        case 4: 
            mSyncSpeed = 0.25f / 1.5f;
            break; 
        case 5: 
            mSyncSpeed = 0.125f;
            break;
        case 6: 
            mSyncSpeed = 0.125f / 1.5f;
            break;
        case 7: 
            mSyncSpeed = 0.0625f;
            break;
        case 8: 
            mSyncSpeed = 0.0625f / 1.5f; 
            break;
        case 9: 
            mSyncSpeed = 0.03125f;
            break;
        default:
            mSyncSpeed = 1;
            break;
        }
    }
}

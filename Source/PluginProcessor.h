/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TVRATremoloAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    TVRATremoloAudioProcessor();
    ~TVRATremoloAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    void updateCurrentTimeInfoFromHost();
    double GetPPQPerSample() const;

    AudioParameterFloat* mSpeedParameter;
    AudioParameterFloat* mDryWetParameter;
    AudioParameterFloat* mDepthParameter;

    AudioParameterInt* mShapeParameter;
    AudioParameterInt* mSyncParameter;

    float BPM;
    float syncSpeed;
    bool mSyncToggle;
    
    
    double period;
    double time;
    double smoothSpeedParam;
    float smoothLFO;
    double debugNum{ 0.0 };

    // Tempo Sync
    double samplesInMinutes{0};
    std::vector<double> mPpqPositions;
    juce::AudioPlayHead::CurrentPositionInfo mPlayHeadInfo;

    float* mLfoPositions{ nullptr };
    double phaseOffset{ 0.0 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TVRATremoloAudioProcessor)

public:
    bool getSync();
    void setSync(bool isSynced);
    void setSyncAmount();

    std::atomic<double> currentPlayHeadPosition{ 0 };
    std::atomic<double> quarterNotePosition{ 0 };

};

#include "JuceHeader.h"
#include "SOUL/include/soul/patch/helper_classes/soul_patch_AudioProcessor.h"
#include "SOUL/include/soul/patch/helper_classes/soul_patch_LoaderPlugin.h"

//==============================================================================
struct Example  : public juce::JUCEApplication,
                  private juce::Timer
{
    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise (const juce::String& commandLineParameters) override {

        deviceManager.initialiseWithDefaultDevices (2,2);
        deviceManager.addAudioCallback (&player);
        deviceManager.addMidiInputDeviceCallback ({}, &player);

        auto patch = library.createPatchInstance(juce::File::getCurrentWorkingDirectory()
                                                 .getChildFile("SineOsc.soulpatch")
                                                 .getFullPathName()
                                                 .toStdString());

        // Init SOULPatchAudioProcessor
        processor = std::make_unique<soul::patch::SOULPatchAudioProcessor> (patch);
        processor->askHostToReinitialise = [&]
        {
            player.setProcessor(nullptr);
            processor->reinitialise();
            player.setProcessor(processor.get());
        };

        processor->askHostToReinitialise();

        startTimer(5000);
    }

    void shutdown() override { 
        player.setProcessor(nullptr);
        deviceManager.closeAudioDevice();
    }

    void timerCallback() override
    {
        processor->sendInputEvent("note", 60);
        // debugging the line above shows processor->inputEventDetails.size()==0
    }

    void systemRequestedQuit() override                         { quit(); }
    void anotherInstanceStarted (const juce::String&) override  {}


    soul::patch::PatchLibraryDLL library;
    std::unique_ptr<soul::patch::SOULPatchAudioProcessor> processor;

    juce::AudioProcessorPlayer player;
    juce::AudioDeviceManager deviceManager;
};

//==============================================================================
START_JUCE_APPLICATION (Example)

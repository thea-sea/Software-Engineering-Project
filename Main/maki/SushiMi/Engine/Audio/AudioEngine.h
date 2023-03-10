/*!
@file    AudioEngine.hpp
@author	 Aurelia Chong

         Header file for AudioEngine.cpp
*//*__________________________________________________________________________*/
#pragma once
/*                                                                   includes
----------------------------------------------------------------------------- */
#include <iostream>
#include <chrono>
#include <thread>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>

namespace Core
{
    /*                                                                   instances
    ----------------------------------------------------------------------------- */
#define AudioManager _audioManager::Instance()

    /*                                                                   Class
    ----------------------------------------------------------------------------- */
    class _audioManager
    {

        /*                                                              Private Class
       ----------------------------------------------------------------------------- */
    private:
        _audioManager(void);
        ~_audioManager(void);
        FMOD::System* fmodSystem = NULL;
        FMOD::Channel* channel = nullptr;
        FMOD::Channel* musicChannel = nullptr;
        FMOD::Channel* voiceChannel = nullptr;
        FMOD::ChannelGroup* channelGroup = nullptr;
        std::map<std::string, FMOD::Sound*> soundDatabase;
        std::map<std::string, FMOD::Sound*> musicDatabase;
        std::map<std::string, FMOD::Sound*> voiceDatabase;
        bool isActive = true;

        /*                                                             Public Class
        ----------------------------------------------------------------------------- */
    public:
        static _audioManager& Instance(void);
        FMOD::Channel* GetMusicChannel(void);
        void CleanPlaying(void);
        void PlaySFX(std::string audiClip);
        void PlayMusic(std::string musicTrack);
        void PlayVoice(std::string voiceClip);
        void StopSFX(void);
        void StopMusic(void);
        void StopVoice();
        void SetAudioVolume(float volume);
        void SetMusicVolume(float volume);
        void SetVoiceVolume(float volume);
        void Update(void);
        void LoadSFX(std::string name);
        void LoadMusic(std::string name);
        void LoadVoice(std::string name);
        void UnLoadSFX(std::string name);
        void UnloadMusic(std::string name);
        void UnloadVoice(std::string name);
        void Free(void);
        FMOD::Sound* GetSound(std::string name);
        FMOD::Sound* GetMusic(std::string name);
    };

    /*                                                             Call back function
     ----------------------------------------------------------------------------- */
    FMOD_RESULT F_CALLBACK channelGroupCallback(FMOD_CHANNELCONTROL* channelControl,
        FMOD_CHANNELCONTROL_TYPE controlType,
        FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
        void* commandData1,
        void* commandData2);
}
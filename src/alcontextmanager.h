// Copyright (C) 2008,2009,2010 by Philipp Muenzel. All rights reserved
// Released under the terms of the GNU General Public License version 2 or later
// as published by the Free Software Foundation, Inc.

#ifndef ALCONTEXTMANAGER_H
#define ALCONTEXTMANAGER_H

#include <map>
#include <string>

#include <boost/noncopyable.hpp>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "alsoundbuffer.h"

namespace XSPL{
    namespace Sound {

        /**
          * Encapsulates all openAL and alut related stuff for looking up devices, handling different
          * contexts etc. Stores SoundBuffer istances to keep track of the currently loaded sounds
          * and provides convenient functions for standard operations like playing
          * @version 0.4
          * @author (c) 2009, 2010 by Philipp Muenzel, Technische Universitaet Darmstadt, Department of Mathematics
          * @file alcontextmanager.h
          */
        class ALContextManager : boost::noncopyable
        {
        public:
            class SoundNotFoundError : public std::runtime_error
            {
            public:
                SoundNotFoundError(const std::string& msg):
                        std::runtime_error(msg)
                {}
            };

            class SoundLoadError : public std::runtime_error
            {
            public:
                SoundLoadError(const std::string& msg):
                        std::runtime_error(msg)
                {}
            };

            class SoundPlayError : public std::runtime_error
            {
            public:
                SoundPlayError(const std::string& msg):
                        std::runtime_error(msg)
                {}
            };

            /**
              * initializes openAL and alut and creates context
              */
            ALContextManager() throw();

            /**
              * deletes context and cleans up loaded sounds
              */
            ~ALContextManager() throw();

            /**
              * tries to load a sound by a file (format depends on what alut distro supports)
              * and stores it locally in a map, providing access by an integer key
              * @param filename the sound to load as a path relative to X-Planes main directory
              * @return unique integer id for adressing the sound buffer
              * @exception throws a SoundLoadError if file cannot be found or has unacceptable format
              */
            int addSoundByFile(const std::string& filename) throw(SoundLoadError);

            /**
              * removes the sound from the map and deletes its buffer
              * @param id the sound buffers id in the map
              */
            void removeSound(int id) throw(SoundNotFoundError);

            /**
              * starts playback of the sound (playback continues when function returns)
              * @param id the sound buffers id in the map
              */
            bool playSound(int id) throw(SoundNotFoundError, SoundPlayError);

            /**
              * stops playback of the sound
              * @param id the sound buffers id in the map
              */
            void stopSound(int id) throw(SoundNotFoundError);

            /**
              * marks the sound to be played in a loop (when playback starts by play() )
              * @param id the sound buffers id in the map
              */
            void loopSound(int id) throw(SoundNotFoundError);

            /**
              * removes the looping flag (playback does stop when the sound's end is reached)
              * @param id the sound buffers id in the map
            */
            void unLoopSound(int id) throw(SoundNotFoundError);

            /**
              * rewinds the sound to it's starting position
              * @param id the sound buffers id in the map
              */
            void rewindSound(int id) throw(SoundNotFoundError);


        private:
            ALSoundBuffer* findSoundById(int id) throw(SoundNotFoundError);
            void deleteAllSounds() throw();

        private:
            std::map<int, ALSoundBuffer*> m_sounds;
            int m_internal_counter;
            ALCdevice* m_device;
            ALCcontext* m_my_context;
            ALCcontext* m_current_context;
        };

    }
}
#endif // ALCONTEXTMANAGER_H

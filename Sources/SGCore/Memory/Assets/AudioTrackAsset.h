//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIOTRACKASSET_H
#define SUNGEARENGINE_AUDIOTRACKASSET_H

#include "SGCore/Audio/AudioTrack.h"
#include "ByteFileAsset.h"

namespace SGCore
{
    struct AudioTrackAsset : public ByteFileAsset
    {
        AudioTrack& getAudioTrack() noexcept;
    
    protected:
        void doLoad(const std::string& path) override;
        
    private:
        AudioTrack m_audioTrack;
    };
}

#endif //SUNGEARENGINE_AUDIOTRACKASSET_H

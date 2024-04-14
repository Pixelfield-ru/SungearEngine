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
    private:
        AudioTrack m_audioTrack;
    
    public:
        void load(const std::string& path) override;
        
        AudioTrack& getAudioTrack() noexcept;
    };
}

#endif //SUNGEARENGINE_AUDIOTRACKASSET_H

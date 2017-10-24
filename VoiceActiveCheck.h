//
//  VoiceActiveCheck.hpp
//  VADTEST
//
//  Created by zhangyu on 2017/10/11.
//  Copyright © 2017年 Michong. All rights reserved.
//

#ifndef VoiceActiveCheck_hpp
#define VoiceActiveCheck_hpp

#include <stdio.h>
#include <memory>
#include <stdlib.h>

namespace webrtc {
    class VoiceActiveCheck {
    public:
        VoiceActiveCheck(int sample_rate_hz);
        ~VoiceActiveCheck();

        bool isActiveVoice(int16_t *buf);
        
        enum Likelihood {
            kLowestLikelihood,
            kVeryLowLikelihood,
            kLowLikelihood,
            kModerateLikelihood,
            kHighLikelihood
        };
        void set_likelihood(Likelihood likelihood);
        Likelihood likelihood();

        void reset();
    private:
        class Vad;
        int frame_size_ms_  = 10;
        size_t frame_size_samples_ = 0;
        int sample_rate_hz_  = 0;
        Vad *vad_ = nullptr;
        
        Likelihood likelihood_ = kLowestLikelihood;
    };
}

#endif /* VoiceActiveCheck_hpp */

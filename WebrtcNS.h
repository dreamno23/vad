//
//  WebrtcNS.hpp
//  VADTEST
//
//  Created by zhangyu on 2017/10/23.
//  Copyright © 2017年 Michong. All rights reserved.
//

#ifndef WebrtcNS_hpp
#define WebrtcNS_hpp

#include <stdio.h>

namespace webrtc {
    class WebrtcNS {
        public:
        WebrtcNS(int sampleRateHz);
        ~WebrtcNS();
        
        void processAudio(short **input, short **output, int sampleCount);
        
        //return sub_estimate
        int *noise_estimate(int *length, int *estimateValue);
        
        enum NoiseSuppression {
            kLow,
            kModerate,
            kHigh,
            kVeryHigh
        };
        
//        mode          : 0: Mild, 1: Medium , 2: Aggressive
        void setLevel(WebrtcNS::NoiseSuppression level);
        
        void reset();
        
        private:
        
        class Suppressor;
        int sample_rate_hz_ = 0;
        size_t frame_size_samples_ = 0;
        Suppressor *suppressors_ = nullptr;
        NoiseSuppression level_ = kModerate;
    };
}


#endif /* WebrtcNS_hpp */

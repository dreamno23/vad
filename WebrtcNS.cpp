//
//  WebrtcNS.cpp
//  VADTEST
//
//  Created by zhangyu on 2017/10/23.
//  Copyright © 2017年 Michong. All rights reserved.
//

#include "WebrtcNS.h"
#include "noise_suppression_x.h"

namespace webrtc {
    class WebrtcNS::Suppressor {
        public:
        explicit Suppressor(int sample_rate_hz) {
            state_ = WebRtcNsx_Create();
            WebRtcNsx_Init(state_, sample_rate_hz);
        }
        ~Suppressor() {
            if (state_ != nullptr) {
                WebRtcNsx_Free(state_);
                state_ = nullptr;
            }
        }
        void reset(int sample_rate_hz) {
            if (state_)
                WebRtcNsx_Free(state_);
            state_ = WebRtcNsx_Create();
            WebRtcNsx_Init(state_, sample_rate_hz);
        }
        NsxHandle* state() { return state_; }
        private:
        NsxHandle* state_ = nullptr;
    };
    
    WebrtcNS::WebrtcNS(int sampleRateHz) {
        sample_rate_hz_ = sampleRateHz;
        suppressors_ = new WebrtcNS::Suppressor(sample_rate_hz_);
        frame_size_samples_ =
        static_cast<size_t>(10 * sample_rate_hz_) / 1000;
        reset();
    }
    WebrtcNS::~WebrtcNS() {
        delete suppressors_;
        suppressors_ = nullptr;
    }
    
    void WebrtcNS::processAudio(short **input, short **output, int sampleCount) {
//        input and output signals should always be 10ms (80 or 160 samples)
        if (sampleCount >= frame_size_samples_) {
            WebRtcNsx_Process(suppressors_->state(), input, frame_size_samples_,output);
        }
    }
    
    int *WebrtcNS::noise_estimate(int *length, int *estimateValue)
    {
        const uint32_t *r_value = WebRtcNsx_noise_estimate(suppressors_->state(), estimateValue);
        int tlength = (int)WebRtcNsx_num_freq();
        *length = tlength;
        return (int *)r_value;
    }
    
    //        mode          : 0: Mild, 1: Medium , 2: Aggressive
    void WebrtcNS::setLevel(WebrtcNS::NoiseSuppression level) {
        if (level_ == level) {
            return;
        }
        int policy = 1;
        switch (level) {
            case NoiseSuppression::kLow:
            policy = 0;
            break;
            case NoiseSuppression::kModerate:
            policy = 1;
            break;
            case NoiseSuppression::kHigh:
            policy = 2;
            break;
            case NoiseSuppression::kVeryHigh:
            policy = 3;
            break;
            default:
            break;
        }
        level_ = level;
        WebRtcNsx_set_policy(suppressors_->state(), level_);
    }
    
    void WebrtcNS::reset() {
        suppressors_->reset(sample_rate_hz_);
        NoiseSuppression hood = level_;
        level_ = (NoiseSuppression)1001;
        setLevel(hood);
    }
}

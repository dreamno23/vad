//
//  VoiceActiveCheck.cpp
//  VADTEST
//
//  Created by zhangyu on 2017/10/11.
//  Copyright © 2017年 Michong. All rights reserved.
//

#include "VoiceActiveCheck.h"
#include "webrtc_vad.h"

namespace webrtc {
    class VoiceActiveCheck::Vad {
    public:
        Vad() {
            reset();
        }
        ~Vad() {
            if (state_)
                WebRtcVad_Free(state_);
        }
        void reset() {
            if (state_)
                WebRtcVad_Free(state_);
            state_ = WebRtcVad_Create();
            WebRtcVad_Init(state_);
        }
        VadInst* state() { return state_; }
    private:
        VadInst* state_ = nullptr;
    };
    
    VoiceActiveCheck::VoiceActiveCheck(int sample_rate_hz) {
        sample_rate_hz_ = sample_rate_hz;
        vad_ = new VoiceActiveCheck::Vad();
        frame_size_samples_ =
        static_cast<size_t>(frame_size_ms_ * sample_rate_hz_) / 1000;
        //preset & reset
        reset();
    }
    
    VoiceActiveCheck::~VoiceActiveCheck() {
        if (vad_) {
            delete vad_;
            vad_ = nullptr;
        }
    }

    bool VoiceActiveCheck:: isActiveVoice(int16_t *buf) {
        int vad_ret = WebRtcVad_Process(vad_->state(), sample_rate_hz_,
                                        buf,
                                        frame_size_samples_);
        return vad_ret != 0;
    }
    
    void VoiceActiveCheck::set_likelihood(VoiceActiveCheck::Likelihood likelihood) {
        if (likelihood_ == likelihood) {
            return;
        }
        likelihood_ = likelihood;
        
        int mode = 2;
        switch (likelihood) {
            case VoiceActiveCheck::kLowestLikelihood:
                mode = 4;
                break;
            case VoiceActiveCheck::kVeryLowLikelihood:
                mode = 3;
                break;
            case VoiceActiveCheck::kLowLikelihood:
                mode = 2;
                break;
            case VoiceActiveCheck::kModerateLikelihood:
                mode = 1;
                break;
            case VoiceActiveCheck::kHighLikelihood:
                mode = 0;
                break;
            default:
                break;
        }
        WebRtcVad_set_mode(vad_->state(), mode);
    }

    void VoiceActiveCheck::reset() {
        vad_->reset();
        Likelihood hood = likelihood_;
        likelihood_ = (Likelihood)1001;
        set_likelihood(hood);
    }
}


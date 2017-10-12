

v0.0.1
用于发声检测，新增参数Likelihood：kLowestLikelihood

use:
m_ivad = new webrtc::VoiceActiveCheck(48000);
int valid = m_ivad->isActiveVoice(m_ibuffer);

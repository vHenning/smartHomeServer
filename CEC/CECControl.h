#ifndef CEC_CONTROL_H
#define CEC_CONTROL_H

#include <libcec/cec.h>

class CECControl
{
public:
    static CECControl* getInstance();

private:
    CECControl();
    CEC::ICECAdapter* adapter;

    static void onKeyPress(void*, const CEC::cec_keypress* msg);

    static void commandReceived(void*, const CEC::cec_command* cmd);

};

#endif

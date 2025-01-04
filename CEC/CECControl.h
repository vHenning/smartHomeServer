#ifndef CEC_CONTROL_H
#define CEC_CONTROL_H

#include <libcec/cec.h>
#include <vector>
#include <functional>

class CECControl
{
public:
    static CECControl* getInstance();

    std::vector<std::function<void (bool)> > onOffListeners;

    void setCommand();

private:
    CECControl();
    CEC::ICECAdapter* adapter;

    static void onKeyPress(void*, const CEC::cec_keypress* msg);

    static void commandReceived(void*, const CEC::cec_command* cmd);

    // Last command that contains info about TV input source
    CEC::cec_command* lastCommand;
};

#endif

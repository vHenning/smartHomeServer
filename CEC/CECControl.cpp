#include "CECControl.h"

#include <libcec/cec.h>
#include <iostream>
using std::cout;
using std::endl;
#include <libcec/cecloader.h>
#include <unistd.h>
#include <boost/bind/bind.hpp>

CECControl* CECControl::getInstance()
{
    static CECControl instance;
    return &instance;
}

void CECControl::commandReceived(void*, const CEC::cec_command* cmd)
{
    fprintf(stderr, "Command received. Opcode: %s Size %d\n", CECControl::getInstance()->adapter->ToString(cmd->opcode), cmd->Size());

    for (size_t i = 0; i < cmd->Size(); ++i)
    {
        fprintf(stderr, "%02X", cmd->parameters[i]);
    }
}

void CECControl::onKeyPress(void*, const CEC::cec_keypress* msg)
{
    fprintf(stderr, "Key %d\n", static_cast<int>(msg->keycode));
}

CECControl::CECControl()
{
    CEC::ICECCallbacks* callbacks = new CEC::ICECCallbacks();
    callbacks->Clear();
    callbacks->keyPress = &CECControl::onKeyPress;
    callbacks->commandReceived = &CECControl::commandReceived;

    CEC::libcec_configuration config;
    config.Clear();

    config.clientVersion = CEC::LIBCEC_VERSION_CURRENT;
    config.bActivateSource = false;
    config.callbacks = callbacks;

    fprintf(stderr, "Calling initialise\n");
    adapter = LibCecInitialise(&config);
    if (adapter == 0)
    {
        fprintf(stderr, "adapter is null");
        return;
    }
    CEC::cec_adapter_descriptor* descriptor = 0;
    int8_t found = adapter->DetectAdapters(descriptor, 1);
    if (found > 0)
    {
        fprintf(stderr, "Found device\n");
        if (!adapter->Open(descriptor->strComName))
        {
            fprintf(stderr, "Could not open device.\n");
        }
    }

    fprintf(stderr, "ID: %d\n", adapter->GetAdapterProductId());
}

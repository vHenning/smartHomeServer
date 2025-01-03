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

void CECControl::setCommand()
{
    adapter->PowerOnDevices(CEC::cec_logical_address::CECDEVICE_AUDIOSYSTEM);
    CEC::cec_command cmd;
    cmd.initiator = CEC::cec_logical_address::CECDEVICE_FREEUSE; // We just say that thats us
    cmd.destination = CEC::cec_logical_address::CECDEVICE_AUDIOSYSTEM; // This is hopefully our audio system
    cmd.opcode = CEC::cec_opcode::CEC_OPCODE_REQUEST_ACTIVE_SOURCE; // This should be the right opcode (or just ACTIVE_SOURCE without REQUEST?)
    cmd.parameters.PushBack(0x01); // This is hopefully Digital in
    adapter->Transmit(cmd);
}

void CECControl::commandReceived(void*, const CEC::cec_command* cmd)
{
    fprintf(stderr, "Command received. Opcode: %s Size %d\n", CECControl::getInstance()->adapter->ToString(cmd->opcode), cmd->Size());

    if (cmd->initiator == CEC::cec_logical_address::CECDEVICE_TV)
    {
        if (cmd->opcode == CEC::cec_opcode::CEC_OPCODE_REPORT_POWER_STATUS)
        {
            if (cmd->Size() > 0)
            {
                if (cmd->parameters[0] == CEC::CEC_POWER_STATUS_ON || cmd->parameters[0] == CEC::CEC_POWER_STATUS_STANDBY)
                {
                    bool on = cmd->parameters[0] == CEC::CEC_POWER_STATUS_ON;

                    CECControl* instance = CECControl::getInstance();
                    if (on)
                    {
                        printf("Command received, on\n");
                        // instance->setCommand();
                    }
                }
            }
        }

    }

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
    CEC::cec_adapter_descriptor* descriptor = new CEC::cec_adapter_descriptor();
    int8_t found = adapter->DetectAdapters(descriptor, 1);
    if (found > 0)
    {
        adapter->Close();
        fprintf(stderr, "Found device %s\n", descriptor->strComName);
        if (!adapter->Open(descriptor->strComName))
        {
            fprintf(stderr, "Could not open device.\n");
            if (!adapter->Open(descriptor->strComPath))
            {
                fprintf(stderr, "And also not path\n");
            }
        }
    }

    fprintf(stderr, "ID: %d\n", adapter->GetAdapterProductId());
}

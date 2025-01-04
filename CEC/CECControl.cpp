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

void CECControl::commandReceived(void* params, const CEC::cec_command* cmd)
{
    // We Listen for the active TV source. We receive opcodes
    // 0x80 Routing Change
    // 0x82 Active Source:      This is sent when we switch to the internal TV receiver (i.e. regular )
    // 0x86 Set Stream Path:    This is sent when we switch to an input source (e.g. HDMI1)
    char dataAsString[CEC_MAX_DATA_PACKET_SIZE * 2];
    memset(dataAsString, 0, CEC_MAX_DATA_PACKET_SIZE * 2);
    for (int i = 0; i < cmd->parameters.size; ++i)
    {
        sprintf(&dataAsString[i*2], "%X", cmd->parameters[i]);
    }
    printf("Command received from %d opcode 0x%X payload %s\n", cmd->initiator, cmd->opcode, dataAsString);

    // If this is a source command, copy it to instance to send it again from time to time
    if (cmd->initiator == CEC::cec_logical_address::CECDEVICE_TV && (cmd->opcode == CEC::cec_opcode::CEC_OPCODE_ACTIVE_SOURCE || cmd->opcode == CEC::cec_opcode::CEC_OPCODE_SET_STREAM_PATH))
    {
        printf("Saving new input command\n");
        CECControl* instance = static_cast<CECControl*>(params);
        if (!instance->lastCommand)
        {
            instance->lastCommand = new CEC::cec_command;
        }
        memcpy(instance->lastCommand, cmd, sizeof(CEC::cec_command));
    }

    // If the TV is turned off (=standby), delete the last command so we dont keep sending it while its turned off
    if (cmd->initiator == CEC::cec_logical_address::CECDEVICE_TV && cmd->opcode == CEC::cec_opcode::CEC_OPCODE_STANDBY)
    {
        printf("TV turned off. Deleting input command\n");
        CECControl* instance = static_cast<CECControl*>(params);
        if (instance->lastCommand)
        {
            delete instance->lastCommand;
            instance->lastCommand = 0;
        }
    }
}

void CECControl::onKeyPress(void*, const CEC::cec_keypress* msg)
{
    printf("Key received\n");
    return;
}

CECControl::CECControl()
    : running(true)
{
    CEC::ICECCallbacks* callbacks = new CEC::ICECCallbacks();
    callbacks->Clear();
    callbacks->keyPress = CECControl::onKeyPress;
    callbacks->commandReceived = CECControl::commandReceived;

    CEC::libcec_configuration config;
    config.Clear();

    lastCommand = NULL;

    memset(&config, 0, sizeof(config));
    config.clientVersion = CEC::LIBCEC_VERSION_CURRENT;
    snprintf(config.strDeviceName, sizeof(config.strDeviceName), "CECScanner");
    config.callbacks = callbacks;
    config.callbackParam = this;

    fprintf(stderr, "Calling initialise\n");
    adapter = LibCecInitialise(&config);
    if (adapter == 0)
    {
        fprintf(stderr, "adapter is null");
        return;
    }
    CEC::cec_adapter_descriptor descriptors[10];
    int8_t found = adapter->DetectAdapters(descriptors, 10);
    for (int i = 0; i < found; ++i)
    {
        printf("Found device %s\n", descriptors[i].strComName);
    }
    if (found > 0)
    {
        printf("Connecting name %s path %s\n", descriptors[0].strComName, descriptors[0].strComPath);
        bool result = adapter->Open(descriptors[0].strComName, 10000);
        if (!result)
        {
            printf("Failed opening device\n");
        }
        else
        {
            printf("Successfully connected.\n");
        }
    }
    else
    {
        printf("No devices found\n");
    }

    fprintf(stderr, "ID: %d\n", adapter->GetAdapterProductId());

    // Start new thread that repeats TV input source occasionally
    runner = new std::thread(&CECControl::runThread, this);
}

void CECControl::runThread()
{
    while (running)
    {
        usleep(5000000);

        if (lastCommand)
        {
            printf("Sending command\n");
            CEC::cec_command command = *lastCommand;
            command.initiator = CEC::cec_logical_address::CECDEVICE_FREEUSE;
            adapter->Transmit(command);
        }
    }
}

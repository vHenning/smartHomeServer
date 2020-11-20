#include "OnkyoManager.h"

OnkyoManager::OnkyoManager()
{
    device = OnkyoServer::getInstance();
    pcPreviousPower = device->getPower();
    pcPreviousSource = device->getSource();
    pcPreviousVolume = device->getVolume();
}

void OnkyoManager::PCListener(bool isOn)
{
    if (isOn)
    {
        pcPreviousPower = device->getPower();
        pcPreviousSource = device->getSource();
        pcPreviousVolume = device->getVolume();

        device->setPower(OnkyoServer::eOn);
        OnkyoServer::Source source = device->setSource(OnkyoServer::ePC);
        if (source == OnkyoServer::ePC)
        {
            device->setVolume(50);
        }
    }
    else
    {
        device->setVolume(pcPreviousVolume);
        device->setSource(pcPreviousSource);
        device->setPower(pcPreviousPower);
    }
}

#ifndef IRREMOTE_CONTROLLER_H
#define IRREMOTE_CONTROLLER_H

#include <Arduino.h>
#include <IRremote.h>
#include "ChannelManager.h"

class IrRemoteController
{
public:
    IrRemoteController(int recvPin, ChannelManager &mgr);

    void begin();  // 初期化
    void update(); // ループで毎回呼ぶ

private:
    IRrecv irrecv;
    // decode_results results;
    ChannelManager &channelManager;

    void handleButton(unsigned long value);
};

#endif

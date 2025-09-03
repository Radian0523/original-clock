#include "IrRemoteController.h"

IrRemoteController::IrRemoteController(int recvPin, ChannelManager &mgr)
    : irrecv(recvPin), channelManager(mgr) {}

void IrRemoteController::begin()
{
    irrecv.enableIRIn();
}

void IrRemoteController::update()
{
    if (irrecv.decode())
    {
        handleButton(irrecv.decodedIRData.decodedRawData);
        irrecv.resume(); // 次の信号を受ける準備
    }
}

// ボタン値に応じてチャンネル切り替え
void IrRemoteController::handleButton(unsigned long value)
{
    int channel = -1;

    // 例: 0xFFA25D = ボタン1, 0xFF629D = ボタン2 ...
    switch (value)
    {
    case 0xFFA25D:
        channel = 0;
        break; // 数字ボタン1
    case 0xFF629D:
        channel = 1;
        break; // 数字ボタン2
    case 0xFFE21D:
        channel = 2;
        break; // 数字ボタン3
    // 必要に応じて他のボタンも追加
    default:
        break;
    }

    if (channel >= 0)
    {
        channelManager.setChannel(channel);
    }
}

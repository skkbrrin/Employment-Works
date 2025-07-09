#pragma once
#ifndef IOBSERVER_DEFINED
#define IOBSERVER_DEFINED
#include "Message.h"

// IObserverインターフェースを定義する
class IObserver
{
public:
	// メッセージを受け取る
	virtual void OnMessegeAccepted(Message::MessageID messageID) = 0;
};

#endif	// IOBSERVER_DEFINED

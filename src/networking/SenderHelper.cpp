#include "stdafx.h"
#include "networking/SenderHelper.h"

SenderHelper::SenderHelper(const utils::milisecs i_timeoutMs)
	: m_timeoutMs(i_timeoutMs), m_senderThread(utils::thread_config{ "sender thread" })
{
}
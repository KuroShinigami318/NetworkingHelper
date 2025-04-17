#pragma once

#include "networking/RawTransferData.h"
#include "networking/ReceiveRawTransferDataError.h"

using ReceiveResult = Result<RawTransferData, ReceiveRawTransferDataError>;

class ReceiverHelper
{
public:
	ReceiveResult ReceiveRawTransferData(class ISocket& i_socket, const std::vector<char>& i_bytes);

private:
	std::unordered_map<ISocket*, RawTransferData> m_pendingData;;
};
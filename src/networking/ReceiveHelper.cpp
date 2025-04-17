#include "stdafx.h"
#include "networking/ReceiverHelper.h"
#include "ISocket.h"

ReceiveResult ReceiverHelper::ReceiveRawTransferData(ISocket& i_socket, const std::vector<char>& i_bytes)
{
	try
	{
		auto pendingDataIt = m_pendingData.find(&i_socket);
		const bool hasPending = pendingDataIt != m_pendingData.end();
		RawTransferData rawData;
		if (hasPending)
		{
			pendingDataIt->second.msg.insert(pendingDataIt->second.msg.end(), i_bytes.begin(), i_bytes.end());
			if (pendingDataIt->second.msg.size() < pendingDataIt->second.totalBytes)
			{
				return make_error<ReceiveRawTransferDataError>(ReceiveTransferErrorCode::PendingData);
			}
			rawData = pendingDataIt->second;
			m_pendingData.erase(pendingDataIt);
		}
		else
		{
			nlohmann::json rawJson = DESERIALIZE_FUNC(i_bytes);
			rawData = rawJson.get<RawTransferData>();
			if (rawData.totalBytes > rawData.msg.size())
			{
				m_pendingData.emplace(&i_socket, rawData);
				return make_error<ReceiveRawTransferDataError>(ReceiveTransferErrorCode::PendingData);
			}
		}
		return rawData;
	}
	catch (const nlohmann::json::exception& e)
	{
		return make_inner_error<ReceiveRawTransferDataError>(ReceiveTransferErrorCode::SerializationFailed, SerializationErrorCode::DeserializeFailed, "Deserialize failed! {} - raw message: {}", e.what(), i_bytes.data());
	}
}
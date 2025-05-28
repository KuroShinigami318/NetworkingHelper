#pragma once

#include "ISocket.h"
#include "networking/RawTransferData.h"
#include "networking/ReceiveRawTransferDataError.h"

template <typename CustomTransferData>
using ReceiveResult = utils::Result<RawTransferData<CustomTransferData>, ReceiveRawTransferDataError>;

template <typename CustomTransferData>
class ReceiverHelper
{
public:
	ReceiveResult<CustomTransferData> ReceiveRawTransferData(class ISocket& i_socket, const std::vector<char>& i_bytes)
	{
		try
		{
			auto pendingDataIt = m_pendingData.find(&i_socket);
			const bool hasPending = pendingDataIt != m_pendingData.end();
			RawTransferData<CustomTransferData> rawData;
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
				rawData = rawJson.get<RawTransferData<CustomTransferData>>();
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

private:
	std::unordered_map<ISocket*, RawTransferData<CustomTransferData>> m_pendingData;;
};
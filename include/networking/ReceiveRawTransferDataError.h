#pragma once

#include "SerializationError.h"
#include "SocketError.h"

DeclareScopedEnumWithOperatorDefined(ReceiveTransferErrorCode, DUMMY_NAMESPACE, uint8_t
	, PendingData
	, ReceiveFailed
	, SerializationFailed);
using ReceiveRawTransferDataError = utils::Error<ReceiveTransferErrorCode, SerializationError, SocketError>;
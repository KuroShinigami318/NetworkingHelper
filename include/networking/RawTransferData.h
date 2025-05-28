#pragma once
#include "nlohmann/json.hpp"
#include <vector>

struct RawTransferData
{
public:
	size_t totalBytes = 0;
	std::vector<char> msg;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RawTransferData, totalBytes, msg);

#define SERIALIZE_FUNC nlohmann::json::to_ubjson
#define DESERIALIZE_FUNC nlohmann::json::from_ubjson
#pragma once
#include "nlohmann/json.hpp"
#include <vector>

struct NoCustomTransferData {};

template <typename CustomTransferData = NoCustomTransferData>
struct RawTransferData : public CustomTransferData
{
public:
	size_t totalBytes = 0;
	std::vector<char> msg;
};

template <typename CustomTransferData>
void to_json(nlohmann::json& j, const RawTransferData<CustomTransferData>& data)
{
	j["totalBytes"] = data.totalBytes;
	j["msg"] = data.msg;
	if constexpr (!std::is_same_v<CustomTransferData, NoCustomTransferData>)
	{
		j["customData"] = static_cast<const CustomTransferData&>(data);
	}
}

template <typename CustomTransferData>
void from_json(const nlohmann::json& j, RawTransferData<CustomTransferData>& data)
{
	j.at("totalBytes").get_to(data.totalBytes);
	j.at("msg").get_to(data.msg);
	if constexpr (!std::is_same_v<CustomTransferData, NoCustomTransferData>)
	{
		j.at("customData").get_to(static_cast<CustomTransferData&>(data));
	}
}

#define SERIALIZE_FUNC nlohmann::json::to_ubjson
#define DESERIALIZE_FUNC nlohmann::json::from_ubjson
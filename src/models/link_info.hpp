#pragma once

#include <chrono>
#include <string>
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"

namespace short_link::models {

struct LinkInfo {
    std::string code;
    std::string short_url;
    std::string original_url;
    std::chrono::time_point<std::chrono::system_clock> created_at;
    std::chrono::time_point<std::chrono::system_clock> expires_at;
    int clicks;

    userver::formats::json::Value ToJSON() const {
        userver::formats::json::ValueBuilder json;
        json["code"] = code;
        json["short_url"] = short_url;
        json["original_url"] = original_url;
        json["created_at"] = created_at;
        json["expires_at"] = expires_at;
        auto res = json.ExtractValue();
        return res;
    }
};

}  // namespace short_link::models

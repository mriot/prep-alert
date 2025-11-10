#pragma once

#include <Common/Types.h>
#include <nlohmann/json.hpp>

using nlohmann::json;

// compact mode
void to_json(json &j, const bool &b);

void from_json(const json &j, bool &b);

// horizontal
void to_json(json &j, const bool &b);

void from_json(const json &j, bool &b);

// shown buff types
void to_json(json &j, const ShownBuffTypes &t);

void from_json(const json &j, ShownBuffTypes &t);

// overlay position
void to_json(json &j, const Position &p);

void from_json(const json &j, Position &p);

// settings
void to_json(json &j, const Settings &s);

void from_json(const json &j, Settings &s);

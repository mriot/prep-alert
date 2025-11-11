#include "SettingsSerialization.h"

using nlohmann::json;

// shown buff types
void to_json(json &j, const ShownBuffTypes &t)
{
    j = {{"food", t.food}, {"utility", t.utility}, {"sigil", t.sigil}};
}

void from_json(const json &j, ShownBuffTypes &t)
{
    t.food    = j.value("food", true);
    t.utility = j.value("utility", true);
    t.sigil   = j.value("sigil", true);
}

// overlay position
void to_json(json &j, const Position &p)
{
    j = {{"x", p.x}, {"y", p.y}};
}

void from_json(const json &j, Position &p)
{
    p.x = j.value("x", 0.0f);
    p.y = j.value("y", 0.0f);
}

// settings
void to_json(json &j, const Settings &s)
{
    j = {
        {"position", s.position},
        {"compact", s.compact},
        {"horizontal", s.horizontal},
        {"flash_duration", s.flashDuration},
        {"default_buff_timeout", s.defaultBuffTimeout},
        {"image_size", s.imageSize},
        {"shown_buffs", s.shownBuffTypes}
    };
}

void from_json(const json &j, Settings &s)
{
    j.at("position").get_to(s.position);
    j.at("compact").get_to(s.compact);
    j.at("horizontal").get_to(s.horizontal);
    j.at("flash_duration").get_to(s.flashDuration);
    j.at("default_buff_timeout").get_to(s.defaultBuffTimeout);
    j.at("image_size").get_to(s.imageSize);
    j.at("shown_buffs").get_to(s.shownBuffTypes);
}

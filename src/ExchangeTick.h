#pragma once

#include <chrono>
#include <string>

class ExchangeTick {
    public:
        ExchangeTick()
        {}
        ExchangeTick(std::string tick_json) : _tick_json{std::move(tick_json)}
        {}
        const std::string& get_tick_json() const { return _tick_json; } 
    private:
        std::string _tick_json;
        std::chrono::system_clock::time_point _timestamp;

};

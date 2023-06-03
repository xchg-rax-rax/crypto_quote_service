#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "ExchangeTick.h"

// We will start out with a single threaded implementation but 
// should bear in mind that a multi threaded once will follow

namespace sm {
    constexpr uint32_t BUFFER_SIZE = 4;
    class StorageManager {
        public:
            StorageManager() :
                _buffer{std::make_unique<std::array<ExchangeTick, BUFFER_SIZE>>()}
            {}
            void add_tick(ExchangeTick tick);
        private:
            uint64_t _next_buffer_index = 0;
            std::unique_ptr<std::array<ExchangeTick, BUFFER_SIZE>> _buffer;

            inline bool is_buffer_full() {
                return BUFFER_SIZE <= _next_buffer_index;
            }
            void write_buffer();
    };
}

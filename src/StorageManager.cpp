#include "StorageManager.h"
#include "ExchangeTick.h"

#include <cstdint>
#include <memory>
#include <thread>
#include <iostream>

namespace sm {
    void StorageManager::add_tick(ExchangeTick tick) {
        if (is_buffer_full()) {
            write_buffer();
        }
        (*_buffer)[_next_buffer_index] = tick;
        _next_buffer_index++;
        std::cout << _next_buffer_index << std::endl;
    }

    void StorageManager::write_buffer() {
        auto write_thread =  std::thread{[buffer = std::move(_buffer)]() {
            // for now we just print but logic to compress and write to disk will go here
            for (auto& tick : *buffer) {
                std::cout << "Number 9" << "\n";
                std::cout << tick.get_tick_json() << std::endl;
            }
        }};
        write_thread.detach();
        _buffer = std::make_unique<std::array<ExchangeTick, BUFFER_SIZE>>();
    }
}

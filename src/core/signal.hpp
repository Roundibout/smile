#pragma once

#include <functional>
#include <vector>
#include <algorithm>

template <typename... Args>
class Signal {
public:
    using Slot = std::function<void(Args...)>;

    class Connection {
        friend class Signal;
        Signal* signal = nullptr;
        size_t id = 0;

        Connection(Signal* signal, size_t id) : signal(signal), id(id) {}
    public:
        Connection() = default;
        void disconnect() {
            if (signal) {
                signal->disconnect(id);
                signal = nullptr;
            }
        }
        bool connected() const {return signal != nullptr;}
    };
private:
    struct SlotData {
        size_t id;
        Slot slot;
    };
    std::vector<SlotData> slots;
    size_t nextId = 1;

    void disconnect(size_t id) {
        slots.erase(std::remove_if(slots.begin(), slots.end(),
            [id](const SlotData& s) {return s.id == id;}),
            slots.end()
        );
    }
public:
    Connection connect(Slot&& slot) {
        slots.push_back({nextId, std::move(slot)});
        return Connection(this, nextId++);
    }

    void emit(Args... args) {
        for (auto& slot : slots) slot.slot(args...);
    }
};

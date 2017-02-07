#include "State.h"

State::State(int id) {
    this->id = id;
}

bool State::equals(State const& other) const {
    return id == other.id;
}

std::size_t State::hash() const {
    return std::hash<int>{}(id);
}

std::string State::to_string() const {
    return std::to_string(id);
}


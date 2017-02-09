#include "State.h"

/*** SimpleState ***/

SimpleState::SimpleState(int id) : id(id) {}

bool SimpleState::equals(State const& other) const {
    if (SimpleState const *obj = dynamic_cast<SimpleState const *> (&other)) {
        return id == obj->id;
    }
    return false;
}
    
std::size_t SimpleState::hash() const {
    return std::hash<int>{}(id);
}   
        
std::string SimpleState::to_string() const {
    return std::to_string(id);
}       

State * SimpleState::clone() const {
    return new SimpleState(id);
}

/*** PairState ***/

PairState::PairState(State *p, State *q) {
    this->p = p->clone();
    this->q = q->clone();
}

PairState::~PairState() {
    delete p;
    delete q;
}

bool PairState::equals(State const& other) const {
    if (PairState const *obj = dynamic_cast<PairState const *> (&other)) {
        return p->equals(*(obj->p)) && q->equals(*(obj->q));
    }
    return false;
}

std::size_t PairState::hash() const {
    std::size_t res = 17;
    res = res * 31 * p->hash();
    res = res * 43 * q->hash();
    return res;
}

std::string PairState::to_string() const {
    return "(" + p->to_string() + "," + q->to_string() + ")";
}

State * PairState::clone() const {
    return new PairState(p, q);
}

/*** GeneratedState  ***/

GeneratedState::GeneratedState(std::string id) : id(id) {}

bool GeneratedState::equals(State const& other) const {
    if (GeneratedState const *obj = dynamic_cast<GeneratedState const *> (&other)) {
        return id == obj->id;
    }
    return false;
}
    
std::size_t GeneratedState::hash() const {
    return std::hash<std::string>{}(id);
}   
        
std::string GeneratedState::to_string() const {
    return id;
}       
        
State * GeneratedState::clone() const {
    return new GeneratedState(id);
}

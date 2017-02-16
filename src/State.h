#ifndef STATE_H
#define STATE_H

#include "Set.h"

/**
 * Represents a state in a PDA.
 */
class State {
    private:
        int id;

    public:
        friend bool operator==(State const& lhs, State const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, State const& self) {
            return strm << self.to_string();
        }

        virtual bool equals(State const& other) const = 0;
        virtual std::size_t hash() const = 0;
        virtual std::string to_string() const = 0;
        virtual State * clone() const = 0;
        virtual ~State() {};
};

/**
 * A State that is labeled with an integer id.
 */
class SimpleState : public State {
    private:
        int id;

    public:
        SimpleState(int id);
        bool equals(State const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        State * clone() const;
};

/**
 * A State that is composed of two other State objects.
 * The order that the two State objects are passed to the constructor matters.
 */
class PairState : public State {
    private:
        State *p, *q;

    public:
        PairState(State *p, State *q);
        bool equals(State const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        State * clone() const;
        ~PairState();
};

/**
 * A State that is labeled with a string id.
 * It is only meant to be generated for temporary use by the cleaning algorithm.
 */
class GeneratedState : public State {
    private:
        std::string id;

    public:
        GeneratedState(std::string id);
        bool equals(State const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        State * clone() const;
};

/**
 * This singleton object is responsible for generating unique GeneratedStates
 * to be used by the cleaning algorithm.
 */
class StateGenerator {
    private:
        StateGenerator() {}

    public:
        static StateGenerator& get() {
            static StateGenerator instance;
            return instance;
        }

        static GeneratedState * get_state() {
            static int id = -4;
            id--;
            return new GeneratedState(std::to_string(id));
        }
};

/**
 * The hasher required for the Set.
 */
struct StateHash {
    inline std::size_t operator()(State* const& s) const {
        return s->hash();
    }
};

/**
 * The equality checker required for the Set.
 */
struct StateEq {
    inline bool operator()(State const *lhs, State const *rhs) const {
        return lhs->equals(*rhs);
    }
};

typedef Set<State, StateHash, StateEq> StateSet;

#endif /* STATE_H */

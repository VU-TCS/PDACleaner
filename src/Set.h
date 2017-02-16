#ifndef SET_H
#define SET_H

#include <unordered_set>
#include <sstream>

/**
 * A collection of elements of type T.
 * The collection will not contain duplicate elements, as determined by
 * the hasher H and the equality checker E.
 *
 * Type T has to have a clone() method that returns a pointer to a deep copy
 * of the object.
 *
 * H is a unary function object type that takes an object of type T as an argument
 * and returns a unique value of type size_t based on that object.
 *
 * E is a binary predicate that takes two objects of type T and returns a bool.
 *
 * See State.h for an example usage.
 */
template<class T, class H, class E>
class Set {
    private:
        std::unordered_set<T *, H, E> set;

    public:
        friend bool operator==(Set const& lhs, Set const& rhs) {
            return lhs.equals(rhs);
        }

        /**
         * Adds a copy of the specified element to this set if it was not already present.
         * If this set already contains the element, the call leaves the set unchanged.
         * 
         * @param e The element to be added to this set.
         * @return The element that was just added to the set.
         */
        T * add(T *e) {
            auto it = set.find(e);
            if (it != set.end()) {
                return *it;
            }

            T *n = e->clone();
            set.insert(n);
            return n;
        }

        /**
         * Adds all elements of the other set to this set.
         *
         * @param other A pointer to the set whose elements are to be added.
         */
        void add_all(Set<T, H, E> *other) {
            for (auto it = other->begin(); it != other->end(); it++) {
                add(*it);
            }
        }

        /**
         * Removes the specified element from this set if it is present.
         * Returns the next valid iterator to be used in for-loops.
         *
         * @param e The element to be removed from this set.
         * @return the next valid iterator.
         */
        typename std::unordered_set<T *, H, E>::const_iterator remove(T *e) {
            auto it = set.find(e);
            if (it != set.end()) {
                T *d = *it;
                it = set.erase(it);
                delete d;
            }

            return it;
        }

        /**
         * Removes all elements in this set that are also contained in the specified set.
         *
         * @param other A pointer to the set whose elements are to be removed from this set.
         */
        void remove_all(Set<T, H, E> *other) {
            for (auto it = other->begin(); it != other->end(); it++) {
                remove(*it);
            }
        }

        /**
         * Retain only the elements in this set that are also contained in the specified set.
         *
         * @param other A pointer to the set whose elements are to be retained in this set.
         */
        void retain_all(Set<T, H, E> *other) {
            for (auto it = set.begin(); it != set.end();) {
                if (!other->contains(*it)) {
                    T *d = *it;
                    it = set.erase(it);
                    delete d;
                } else {
                    it++;
                }
            }
        }

        /**
         * Returns true if this set contains the specified element.
         *
         * @param e The element whose presence in this set is to be tested.
         * @return true if this set contains the specified element.
         */
        bool contains(T *e) const {
            return !(set.find(e) == set.end());
        }

        /**
         * Returns the number of elements contained in this set.
         *
         * @return the number of elements in this set.
         */
        std::size_t size() const {
            return set.size();
        }

        /**
         * Returns true if this set contains no elements.
         *
         * @return true if this set contains no elements.
         */
        bool is_empty() {
            return set.empty();
        }

        /**
         * Empties the set.
         */
        void clear() {
            for (auto it = set.begin(); it != set.end(); it++) {
                delete *it;
            }

            set.clear();
        }

        /**
         * Returns an iterator pointing to the first element in this set.
         * There are no guarantees as to which element is considered the first one.
         *
         * @return An iterator pointing to the first element in this set.
         */
         typename std::unordered_set<T *, H, E>::const_iterator begin() const {
             return set.begin();
         }

        /**
         * Returns an iterator pointing to the past-the-end element of this set.
         * The value returned shall not be dereferenced.
         *
         * @return An iterator pointing to the past-the-end element of this set.
         */
        typename std::unordered_set<T *, H, E>::const_iterator end() const {
            return set.end();
        }

        /**
         * Returns true if this set contains exactly the same elements
         * as the specified set.
         *
         * @param other The set against which equality should tested.
         * @return true if this set contains the same elements as the specified set.
         */
        bool equals(Set const& other) const {
            if (size() != other.size())
                return false;

            for (auto it = set.begin(); it != set.end(); it++) {
                if (!other.contains(*it))
                    return false;
            }

            return true;
        }

        /**
         * Returns a string representation of this set formatted using the pre and
         * post strings as element separators.
         *
         * @param pre The string to be prepended to every element
         * @param post The string to be appended to every element
         * @return A string representation of this set.
         */
        std::string to_string(std::string pre, std::string post) const {
            std::stringstream ss;

            ss << "{" << post;
            for (auto it = set.begin(); it != set.end(); it++) {
                ss << pre << **it << post;
            }
            ss << "}";

            return ss.str();
        }

        /**
         * Returns a pointer to a deep copy of the set.
         * All elements in the new set are copies of the elements in the set.
         *
         * @return A pointer to a deep copy of the set.
         */
        Set<T, H, E> * clone() const {
            Set<T, H, E> *clone_set = new Set<T, H, E>;

            for (auto it = set.begin(); it != set.end(); it++) {
                clone_set->add(*it);
            }

            return clone_set;
        }

        /**
         * Returns a pointer to a new set that contains all elements of the current
         * set that are not contained in other.
         *
         * @param other The set whose elements are not to be copied into the new set.
         * @return A pointer to a set representing the difference of this set minus other.
         */
        Set<T, H, E> * difference(Set<T, H, E> *other) const {
            Set<T, H, E> *diff = new Set<T, H, E>;

            for (auto it = set.begin(); it != set.end(); it++) {
                if (!other->contains(*it))
                    diff->add(*it);
            }

            return diff;
        }

        ~Set() {
            for (auto it = set.begin(); it != set.end(); it++) {
                delete *it;
            }
        }
};

#endif /* SET_H */

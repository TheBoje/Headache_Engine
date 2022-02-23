#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <stdexcept>
#include <memory>

namespace ift3100 {

    /**
     * A simple tree representing hierarchy.
     * @param T
     */
    template <class T>
    class Hierarchy {
        std::shared_ptr<T> _ref;
        std::vector<Hierarchy<T> *> _children;

    public:
        Hierarchy() : _ref(nullptr) {}
        Hierarchy(std::shared_ptr<T> ref) : _ref(ref) {}

        Hierarchy(const Hierarchy<T> &cpy) : _ref(cpy._ref)) {
            int children_size = cpy._children.size();
            _children.reserve(children_size);

            for (Hierarchy<T> *child : cpy._children) {
                _children.push_back(new Hierarchy<T>(*child));
            }
        }

        ~Hierarchy() {
            for(Hierarchy<T> *child : _children) {
                delete child;
            }
            // delete _ref; // Hierarchy does not have property of pointers.
        }

        /**
         * Will add a child to the current node
         * @param child
         */
        void addChild(std::shared_ptr<T> child) {
            _children.push_back(child);
        }

        Hierarchy<T> &operator=(const Hierarchy<T> &other) {
            if(other != *this) {
                for(Hierarchy<T> *child : _children) {
                    delete child;
                }

                int children_size = other._children.size();
                _children.reserve(children_size);

                for (Hierarchy<T> *child : other._children) {
                    _children.push_back(new Hierarchy<T>(*child));
                }
            }
        }

        /**
         *
         * @param index
         * @return the index-th child of the node
         * @throw out_of_range
         */
        Hierarchy<T> &operator[](std::size_t index) {
            try {
                return _children.at(index);
            } catch (std::out_of_range & e) {
                throw e;
            }
        }

        /**
         * Will apply "func" method to the node value and all its children (grand-children etc...)
         * Usefull for parent transformation.
         * @param func
         */
        void map(void (*func)(std::shared_ptr<T>)) {
            func(_ref);
            for(Hierarchy<T> child : _children) {
                child->map(func);
            }
        }

        void setRef(std::shared_ptr<T> ref) {
            _ref = ref;
        }

        std::shared_ptr<T> getRef() {
            return _ref;
        }
    };
}

#endif
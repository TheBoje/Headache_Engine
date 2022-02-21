#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <vector>
#include <stdexcept>

namespace ift3100 {

    template <class T>
    class Hierarchy {
        T *_ref;
        std::vector<Hierarchy<T> *> _children;

    public:
        Hierarchy() : _ref(nullptr) {}
        Hierarchy(T *ref) : _ref(ref) {}

        Hierarchy(const Hierarchy<T> &cpy) : _ref(new T(cpy._ref)) {
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
            delete _ref;
        }

        void addChild(T *child) {
            _children.push_back(child);
        }

        Hierarchy<T> &operator=(const Hierarchy<T> &other) {
            if(other != *this) {
                for(Hierarchy<T> *child : _children) {
                    delete child;
                }
                delete _ref;

                int children_size = other._children.size();
                _children.reserve(children_size);

                for (Hierarchy<T> *child : other._children) {
                    _children.push_back(new Hierarchy<T>(*child));
                }
            }
        }


        Hierarchy<T> &operator[](std::size_t index) {
            try {
                return _children.at(index);
            } catch (std::out_of_range & e) {
                throw e;
            }
        }

        void setRef(T *ref) {
            delete _ref;
            _ref = ref;
        }

        T *getRef() {
            return _ref;
        }
    };
}

#endif
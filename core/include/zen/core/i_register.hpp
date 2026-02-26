#pragma once

#include <algorithm>

namespace zen {

namespace core {

template<typename T>
class IRegister {
public:
    // register a unique instance of T, return false if already registered or NULL
    bool register(T* _instance) {
        // cannot register null
        if (_instance == nullptr) {
            return false; 
        }

        // cannot register duplicate        
        for (size_t i = 0; i < m_count; ++i) {
            if (m_instances[i] == _instance) {
                return false; 
            }
        }

        // resize array if needed
        if (m_count >= m_capacity) {
            // resize array
            size_t newCapacity = m_capacity * 2;
            T** newInstances = new T*[newCapacity];
            for (size_t i = 0; i < m_count; ++i) {
                newInstances[i] = m_instances[i];
            }
            delete[] m_instances;
            m_instances = newInstances;
            m_capacity = newCapacity;
        }

        // register unique instance
        m_instances[m_count++] = _instance;
        return true;
    }

    void unregister(T* _instance) {
        // cannot unregister null
        if (_instance == nullptr) {
            return;
        }

        // find instance if exists and remove
        for (size_t i = 0; i < m_count; ++i) {
            if (m_instances[i] == _instance) {
                // shift remaining instances down
                for (size_t j = i; j < m_count - 1; ++j) {
                    m_instances[j] = m_instances[j + 1];
                }
                --m_count;

                // shrink array if too empty
                if (m_count > 0 && m_count <= m_capacity / 4 && m_capacity > m_kInitCapacity) {
                    size_t newCapacity = std::max(m_capacity / 2, m_kInitCapacity);
                    T** newInstances = new T*[newCapacity];
                    for (size_t k = 0; k < m_count; ++k) {
                        newInstances[k] = m_instances[k];
                    }
                    delete[] m_instances;
                    m_instances = newInstances;
                    m_capacity = newCapacity;
                }

                return;
            }
        }
    }

    size_t count() const {
        return m_count;
    }

protected:
    // private constructor for singleton
    virtual IRegister()
        : m_count(0),
        m_capacity(m_kInitCapacity),
        m_instances(new T*[m_kInitCapacity]) {}
    virtual ~IRegister() {
        delete[] m_instances;
    }

private:
    // constants
    const size_t m_kInitCapacity = 8;

    // dynamically allocated list
    T** m_instances;
    size_t m_count;
    size_t m_capacity;

};

} // namespace core

} // namespace zen
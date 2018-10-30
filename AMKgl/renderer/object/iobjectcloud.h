#ifndef IOBJECTCLOUD_H
#define IOBJECTCLOUD_H

#include <QtGlobal>
#include <vector>
#include <functional>

/**
 * @brief Class that can modify multiple element instead of one. Useful when an object is defined by multiple colors/infos
 */
template<typename T>
class ElementModifier
{
public:
    ElementModifier(T* first, const quint8& size) : firstT(first), m_size(size) {}
    virtual ~ElementModifier() {}

    ElementModifier& operator=(const T& newValue) {
        for(int i=0; i<m_size; ++i) {
            firstT[i] = newValue;
        }

        return *this;
    }

    ElementModifier& operator|=(const T& newValue) {
        for(int i=0; i<m_size; ++i) {
            firstT[i] |= newValue;
        }

        return *this;
    }

    ElementModifier& operator&=(const T& newValue) {
        for(int i=0; i<m_size; ++i) {
            firstT[i] &= newValue;
        }

        return *this;
    }

    ElementModifier& operator^=(const T& newValue) {
        for(int i=0; i<m_size; ++i) {
            firstT[i] ^= newValue;
        }

        return *this;
    }

    T& operator [](const size_t& index) {
        return firstT[index];
    }

    const T& operator [](const size_t& index) const {
        return firstT[index];
    }

    T& operator ()(const size_t& index) {
        return firstT[index];
    }

    const T& operator ()(const size_t& index) const {
        return firstT[index];
    }

    bool operator|(const int& valueToTest) const {
        return firstT[0] | valueToTest;
    }

    bool operator&(const int& valueToTest) const {
        return firstT[0] & valueToTest;
    }

    operator T() const { return (*firstT); }

protected:
    T*              firstT;
    const quint8&   m_size;
};

/**
 * @brief Represent a cloud that can access element like colors, infos, etc... of objects
 */
template<typename valueT>
class AbstractObjectCloud
{
public:
    typedef valueT value_type;
    typedef ElementModifier<valueT> return_type;

    virtual ~AbstractObjectCloud() {}

    virtual size_t size() const = 0;

    virtual ElementModifier<valueT> operator[](const size_t& objectIndex) const = 0;
};

/**
 * @brief Concrete class to access element like colors, infos, etc... of objects
 */
template<typename arrayT>
class ObjectCloud : public AbstractObjectCloud< typename arrayT::value_type >
{
public:
    typedef typename arrayT::value_type value_type;

    ObjectCloud(arrayT &array, const quint8& nElementByObject) : m_array(array), m_nElementByObject(nElementByObject) {}

    virtual ElementModifier<typename arrayT::value_type> operator[](const size_t& objectIndex) const {
        return ElementModifier<typename arrayT::value_type>(&m_array[ objectIndex*m_nElementByObject ], m_nElementByObject);
    }

    inline quint8 getNumberOfElementPerObject() const { return m_nElementByObject; }

    size_t size() const { return m_array.size(); }

protected:
    quint8                                          m_nElementByObject;
    arrayT&                                         m_array;
};

/**
 * @brief Represent a cloud that can access element like colors, infos, etc... for object
 */
template<typename arrayT, typename IndexCollection>
class ObjectCloudIndexed : public ObjectCloud<arrayT>
{
public:
    ObjectCloudIndexed(arrayT &array,
                       const int& nElementByObject,
                       IndexCollection& indexes) : ObjectCloud<arrayT>(array, nElementByObject), m_indexes(indexes) {}

    virtual ElementModifier<typename arrayT::value_type> operator[](const size_t& objectIndex) const {
        return ElementModifier<typename arrayT::value_type>(&this->m_array[ m_indexes[objectIndex*this->m_nElementByObject] ], this->m_nElementByObject);
    }

    size_t size() const { return m_indexes.size(); }

private:
    IndexCollection&  m_indexes;
};

/**
 * @brief Concrete class to access element like colors, infos, etc... of objects
 */
template<typename valueT>
class ObjectPointerCloud : public AbstractObjectCloud< valueT >
{
public:
    typedef valueT value_type;

    ObjectPointerCloud(valueT* array, const size_t& size, const quint8& nElementByObject) : m_array(array), m_size(size), m_nElementByObject(nElementByObject) {}

    virtual ElementModifier<valueT> operator[](const size_t& objectIndex) const {
        return ElementModifier<valueT>(&m_array[ objectIndex*m_nElementByObject ], m_nElementByObject);
    }

    inline quint8 getNumberOfElementPerObject() const { return m_nElementByObject; }

    size_t size() const { return m_size; }

protected:
    quint8  m_nElementByObject;
    valueT* m_array;
    size_t  m_size;
};

/**
 * @brief Same as class ObjectPointerCloud but use a collection of index
 */
template<typename valueT, typename IndexCollection>
class ObjectPointerCloudIndexed : public ObjectPointerCloud<valueT>
{
public:
    ObjectPointerCloudIndexed(valueT* array,
                              const int& nElementByObject,
                              IndexCollection& indexes) : ObjectPointerCloud<valueT>(array, indexes.size(), nElementByObject), m_indexes(indexes) {}

    virtual ElementModifier<valueT> operator[](const size_t& objectIndex) const {
        return ElementModifier<valueT>(&this->m_array[ m_indexes[objectIndex*this->m_nElementByObject] ], this->m_nElementByObject);
    }

private:
    IndexCollection&  m_indexes;
};

/**
 * @brief Concrete class to access element like colors, infos, etc... of objects with use of a function that get the first value in the array
 */
template<typename valueT>
class ObjectFuncPointerCloud : public AbstractObjectCloud< valueT >
{
public:
    typedef valueT value_type;
    typedef std::function<valueT* (void* context)> funcGetFirstValue;

    ObjectFuncPointerCloud(funcGetFirstValue func,
                           void* context,
                           const size_t& size,
                           const quint8& nElementByObject) : m_func(func), m_context(context), m_size(size), m_nElementByObject(nElementByObject) {}

    virtual ElementModifier<valueT> operator[](const size_t& objectIndex) const {
        return ElementModifier<valueT>(&m_func(m_context)[ objectIndex*m_nElementByObject ], m_nElementByObject);
    }

    inline quint8 getNumberOfElementPerObject() const { return m_nElementByObject; }

    size_t size() const { return m_size; }

protected:
    quint8              m_nElementByObject;
    funcGetFirstValue   m_func;
    void*               m_context;
    size_t              m_size;
};

/**
 * @brief Same as class ObjectFuncPointerCloud but use a collection of index
 */
template<typename valueT, typename IndexCollection>
class ObjectFuncPointerCloudIndexed : public ObjectFuncPointerCloud<valueT>
{
public:
    ObjectFuncPointerCloudIndexed(typename ObjectFuncPointerCloud<valueT>::funcGetFirstValue func,
                                  void* context,
                                  const int& nElementByObject,
                                  IndexCollection& indexes) : ObjectFuncPointerCloud<valueT>(func, context, indexes.size(), nElementByObject), m_indexes(indexes) {}

    virtual ElementModifier<valueT> operator[](const size_t& objectIndex) const {
        return ElementModifier<valueT>(&this->m_func(this->m_context)[ m_indexes[objectIndex*this->m_nElementByObject] ], this->m_nElementByObject);
    }

private:
    IndexCollection&  m_indexes;
};

#endif // IOBJECTCLOUD_H

#ifndef SFINAE_H
#define SFINAE_H

/**
 * @brief SFINAE tool to know if the model was an input result model that produce copies
 */
template <typename Type>
class IsAResultModelCopy
{
   class yes { char m;};
   class no { yes m[2];};
   struct BaseMixin
   {
     int nOutResultModelCopiesCreated() const {}
   };
   struct Base : public Type, public BaseMixin {};
   template <typename T, T t>  class Helper{};
   template <typename U>
   static no deduce(U*, Helper<int (BaseMixin::*)() const, &U::nOutResultModelCopiesCreated>* = 0);
   static yes deduce(...);
public:
   static const bool Is = sizeof(yes) == sizeof(deduce((Base*)(0)));
};

/**
 * @brief SFINAE tool to know if the model is an output model
 */
template <typename Type>
class IsAnOutputModel
{
   class yes { char m;};
   class no { yes m[2];};
   struct BaseMixin
   {
     int uniqueIndex() const {}
   };
   struct Base : public Type, public BaseMixin {};
   template <typename T, T t>  class Helper{};
   template <typename U>
   static no deduce(U*, Helper<int (BaseMixin::*)() const, &U::uniqueIndex>* = 0);
   static yes deduce(...);
public:
   static const bool Is = sizeof(yes) == sizeof(deduce((Base*)(0)));
};

/**
 * @brief SFINAE tool to know if the model is an input model. If the method "uniqueIndex() const" is present
 *        we deduce that it is an output model.
 */
template <typename Type>
class IsAnInputModel
{
   class yes { char m;};
   class no { yes m[2];};
   struct BaseMixin
   {
     int uniqueIndex() const {}
   };
   struct Base : public Type, public BaseMixin {};
   template <typename T, T t>  class Helper{};
   template <typename U>
   static no deduce(U*, Helper<int (BaseMixin::*)() const, &U::uniqueIndex>* = 0);
   static yes deduce(...);
public:
   static const bool Is = sizeof(no) == sizeof(deduce((Base*)(0)));
};

/**
 * @brief SFINAE tool to know if the handle is an abstract input handle. If the enum "MinValue" is present
 *        we deduce that it is an concrete input handle (not abstract).
 */
// You need void_t to avoid a warning about the lhs of the comma operator
// having no effect. C++ 17 has std::void_t
template<class...> using void_t = void;

template<class T, class = void>
struct IsAnInputAbstractHandle
{
    static constexpr bool value = false;
};

template<class T>
struct IsAnInputAbstractHandle<T, void_t<decltype(T::MinValue)>>
{
    static constexpr bool value = true;
};

#endif // SFINAE_H

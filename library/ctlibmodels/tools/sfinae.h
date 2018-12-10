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
 * @brief SFINAE tool to know if the model is an input model
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

#endif // SFINAE_H

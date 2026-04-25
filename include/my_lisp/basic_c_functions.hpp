#pragma once

#include <my_lisp/sexpression.hpp>
#include <vector>
#include <span>

class Environment;

using SExpressionVector = std::vector<SExpression>;
using SExpressionSpan = std::span<SExpression>;

namespace basic_c_functions
{

using PredicateFunction = bool (*)(const SExpression &);
using EqualityFunction  = bool (*)(const SExpression &, const SExpression &);
using CreationFunction = SExpression (*)();
using UnaryTransformationFunction = SExpression (*)(const SExpression &);
using UnaryTransformationInEnvironmentFunction = SExpression (*)(Environment &, const SExpression &);
using BinaryTransformationFunction = SExpression (*)(const SExpression &, const SExpression &);

inline bool null(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::Nil;
}

inline bool symbolp(const SExpression &expression)
{
    return expression.type() == Variant::Type::Symbol;
}

inline bool consp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::ConsCell;
}

inline bool atom(const SExpression &expression)
{
    return !consp(expression);
}

inline bool listp(const SExpression &parameter)
{
    return consp(parameter) || null(parameter);
}

inline bool endp(const SExpression &parameter)
{
    // ASSERT: either null(parameter) || cons(parameter)
    return null(parameter);
}

inline bool numberp(const SExpression &parameter)
{
    return (parameter.type() == Variant::Type::Number) || (parameter.type() == Variant::Type::FixedNumber);
}

inline bool integerp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::FixedNumber;
}

inline bool rationalp(const SExpression &)
{
    return false; // TODO: Implement me
}

inline bool floatp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::Number;
}

inline bool realp(const SExpression &)
{
    return false; // TODO: Implement me
}

inline bool complexp(const SExpression &)
{
    return false; // TODO: Implement me
}

inline bool characterp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::Char;
}

inline bool stringp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::String;
}

inline bool functionp(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::Function;
}

inline bool packagep(const SExpression &parameter)
{
    return parameter.type() == Variant::Type::Package;
}

inline SExpression cons(const SExpression &car, const SExpression &cdr)
{
    return SExpression::make_cons(car, cdr);
}

inline SExpression nil()
{
    return SExpression::make_nil();
}

inline SExpression t()
{
    return SExpression::make_true();
}

inline SExpression car(const SExpression &parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return (*parameter.asConsCellPtr())->car;
    return nil();
}

inline SExpression cdr(const SExpression &parameter)
{
    if ( parameter.type() == Variant::Type::ConsCell )
        return (*parameter.asConsCellPtr())->cdr;
    return SExpression::make_nil();
}

inline SExpression caar(const SExpression &parameter)
{
    return car( car( parameter ) );
}

inline SExpression cadr(const SExpression &parameter)
{
    return cdr( car( parameter ) );
}

inline SExpression cdar(const SExpression &parameter)
{
    return car( cdr( parameter ) );
}

inline SExpression cddr(const SExpression &parameter)
{
    return cdr( cdr( parameter ) );
}

inline SExpression first(const SExpression &parameter)
{
    return car(parameter);
}

inline SExpression rest(const SExpression &parameter)
{
    return cdr(parameter);
}

inline SExpression second(const SExpression &parameter)
{
    return first( rest(parameter) );
}

inline SExpression third(const SExpression &parameter)
{
    return first( rest( rest(parameter) ) );
}

inline SExpression fourth(const SExpression &parameter)
{
    return first( rest( rest( rest(parameter) ) ) );
}

inline SExpression fifth(const SExpression &parameter)
{
    return first( rest( rest( rest( rest(parameter) ) ) ) );
}

inline SExpression sixth(const SExpression &parameter)
{
    return first( rest( rest( rest( rest( rest(parameter) ) ) ) ) );
}

inline SExpression seventh(const SExpression &parameter)
{
    return first( rest( rest( rest( rest( rest( rest(parameter) ) ) ) ) ) );
}

inline SExpression eighth(const SExpression &parameter)
{
    return first( rest( rest( rest( rest( rest( rest( rest(parameter) ) ) ) ) ) ) );
}

inline SExpression ninth(const SExpression &parameter)
{
    return first( rest( rest( rest( rest( rest( rest( rest( rest(parameter) ) ) ) ) ) ) ) );
}

inline bool eq(const SExpression &left, const SExpression &right)
{
    return left == right;
}

bool eql(const SExpression &left, const SExpression &right);
bool equal(const SExpression &left, const SExpression &right);
bool equalp(const SExpression &left, const SExpression &right);

inline bool logical_not(const SExpression &parameter)
{
    return (parameter.type() == Variant::Type::Nil) ? true : false;
}

SExpression prin1(Environment &current_environment, const SExpression &parameter);
SExpression print(Environment &current_environment, const SExpression &parameter);

SExpression eval(Environment &current_environment, const SExpression &form);
void eval(Environment &current_environment, SExpressionSpan forms);

bool is_special_operator(Environment &current_environment, const SExpression &parameter);
bool is_macro(Environment &, const SExpression &form);

inline bool self_evaluating(const SExpression &parameter)
{
    return !symbolp(parameter) && !listp(parameter);
}

// Returns -1 if it is not a list
// Returns length of list
// If list is circular, returns 0
int list_length(const SExpression &parameter);

std::vector<SExpression> list_to_vector(const SExpression &parameter);

SExpression span_to_list(std::span<SExpression> parameter);

}

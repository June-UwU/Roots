#pragma once
#include "../types.hpp"
#include <variant>
class RootObject;
class BooleanObject;
class FloatObject;
class StringObject;

typedef enum ObjectType
{
    NONE_OBJECT = 0x0,
    BOOLEAN_OBJECT,
    FLOAT_OBJECT,
    STRING_OBJECT,

    OBJECT_TYPE
}ObjectType;

typedef enum RootOperator {
    RT_OP_NEG = 0x0,
    RT_OP_ADD,
    RT_OP_SUB,
    RT_OP_MUL,
    RT_OP_DIV,

    ROOT_OPERATOR
}RootOperator;

RootObject* rootOperation(RootObject* lhs, RootObject* rhs, RootOperator oper);

class RootObject
{
    public:
        ObjectType getType() const;
        std::string toString() const;
    protected:
        ObjectType type = NONE_OBJECT;
};

// Data types
class BooleanObject : public RootObject
{
    public:
        BooleanObject(bool boolValue);
        std::string toString() const;
        bool getValue() const;
        void setValue(bool booleanValue);
    private:
        bool value;
};

class FloatObject : public RootObject
{
    public:
        FloatObject(f64 floatValue);
        std::string toString() const;
        f64 getValue() const;
        void setValue(f64 floatValue);
    private:
        f64 value;
};

class StringObject : public RootObject
{
    public:
        StringObject(std::string stringValue);
        std::string toString() const;
        std::string getValue() const;
        void setValue(std::string stringValue);
    private:
        std::string value;
};

#pragma once
#include "../types.hpp"
#include <variant>

typedef enum ObjectType
{
    NONE_OBJECT = 0x0,
    BOOLEAN_OBJECT,
    FLOAT_OBJECT,
    STRING_OBJECT,

    OBJECT_TYPE
}ObjectType;



class RootObject
{
    public:
        ObjectType getType() const;
        std::string toString() const;
    protected:
        ObjectType type = NONE_OBJECT;
};

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

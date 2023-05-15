#pragma once
#include "../types.hpp"
#include <variant>

typedef enum ObjectType
{
    ROOT_OBJECT = 0x0,
    BOOLEAN_OBJECT,
    FLOAT_OBJECT,
    SIGNED_INTEGER_OBJECT,
    UNSIGNED_INTEGER_OBJECT,
    STRING_OBJECT,

    OBJECT_TYPE
}ObjectType;



class RootObject
{
    public:
        ObjectType getType() const;
        virtual std::string toString() const = 0;
    protected:
        ObjectType type;
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
        FloatObject(f32 floatValue);
        std::string toString() const;
        f32 getValue() const;
        void setValue(f32 floatValue);
    private:
        f32 value;
};

class IntObject : public RootObject
{
    public:
        IntObject(s32 intValue);
        std::string toString() const;
        s32 getValue() const;
        void setValue(s32 intValue);
    private:
        s32 value;
};

class UintObject : public RootObject
{
    public:
        UintObject(u32 uintValue);
        std::string toString() const;
        u32 getValue() const;
        void setValue(u32 uintValue);
    private:
        u32 value;
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


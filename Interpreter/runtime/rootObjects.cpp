#include "rootObjects.hpp"
#include "../logger.hpp"
#include <sstream>

using ostream = std::ostringstream;
constexpr const char* OBJECT_TYPE_STR[OBJECT_TYPE] = 
{
    "NONE_OBJECT","BOOLEAN_OBJECT","FLOAT_OBJECT","STRING_OBJECT"
};


ObjectType RootObject::getType() const
{
    return type;
}

std::string RootObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type];
    return stream.str();    
}

BooleanObject::BooleanObject(bool boolValue)
    :value{boolValue}
{
    type = BOOLEAN_OBJECT;
}

std::string BooleanObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type] << " : " << value;
    return stream.str();
}
    
bool BooleanObject::getValue() const
{
    return value;
}

void BooleanObject::setValue(bool booleanValue)
{
    value = booleanValue;
}

FloatObject::FloatObject(f64 floatValue)
    :value{floatValue}
{
    type = FLOAT_OBJECT;
}

std::string FloatObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type] << " : " << value;
    return stream.str();
}

f64 FloatObject::getValue() const
{
    return value;
}

void FloatObject::setValue(f64 floatValue)
{
    value = floatValue;
}

StringObject::StringObject(std::string stringValue)
    :value{stringValue}
{
    type = STRING_OBJECT;
}

std::string StringObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type] << " : " <<  value;
    return stream.str();
}
 
std::string StringObject::getValue() const
{
    return value;
}
        
void StringObject::setValue(std::string stringValue)
{
    value = stringValue;
}

RootObject* rootOperation(RootObject* lhs, RootObject* rhs, RootOperator oper) 
{
    auto ret = new FloatObject(69);
    switch(oper) 
    {
        case RT_OP_ADD:
            LOG_INFO("ROOT OBJECT ADD");
            break;
        case RT_OP_SUB:
            LOG_INFO("ROOT OBJECT SUB");
            break;
        case RT_OP_MUL:
            LOG_INFO("ROOT OBJECT MUL");
            break;
        case RT_OP_DIV:
            LOG_INFO("ROOT OBJECT DIV");
            break;
        case RT_OP_NEG:
            LOG_INFO("ROOT OBJECT NEG");
            break;
    }
    return ret;
}


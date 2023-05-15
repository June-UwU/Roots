#include "rootObjects.hpp"
#include <sstream>

using ostream = std::ostringstream;
constexpr const char* OBJECT_TYPE_STR[OBJECT_TYPE] = 
{
    "ROOT_OBJECT","BOOLEAN_OBJECT","FLOAT_OBJECT",
    "SIGNED_INTEGER_OBJECT","UNSIGNED_INTEGER_OBJECT","STRING_OBJECT"
};


ObjectType RootObject::getType() const
{
    return type;
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

FloatObject::FloatObject(f32 floatValue)
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

f32 FloatObject::getValue() const
{
    return value;
}

void FloatObject::setValue(f32 floatValue)
{
    value = floatValue;
}

IntObject::IntObject(s32 intValue)
    :value{intValue}
{
    type = SIGNED_INTEGER_OBJECT;
}

std::string IntObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type] << " : " << value;
    return stream.str();
}

s32 IntObject::getValue() const
{
    return value;
}

void IntObject::setValue(s32 intValue)
{
    value = intValue;
}

UintObject::UintObject(u32 uintValue)
    :value{uintValue}
{
    type = UNSIGNED_INTEGER_OBJECT;
}
    
std::string UintObject::toString() const
{
    ostream stream;
    stream << OBJECT_TYPE_STR[type] << " : " << value;
    return stream.str();
}
        
u32 UintObject::getValue() const
{
    return value;
}

void UintObject::setValue(u32 uintValue)
{
    value = uintValue;
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


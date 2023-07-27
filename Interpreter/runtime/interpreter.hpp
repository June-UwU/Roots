#pragma once
#include "../expression.hpp"
#include "rootObjects.hpp"
#include "instructions.hpp"

void interpret();
void interpretMachineCode(ByteCode* code);
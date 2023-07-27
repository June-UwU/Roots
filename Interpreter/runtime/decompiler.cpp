#include "decompiler.hpp"
#include <cstring>

void writeFile(FILE* file, Ins ins, u64& offset);

void decompile(ByteCode* code,OutputOption option)
{
    FILE* fileSink = nullptr;
    u64 offset = 0;
    if(TEXT_FILE == option)
    {
        fileSink = fopen("dump.rts","w");
    }
    else
    {
        fileSink = stdout;
    }
    
    while(*code != INS_END)
    {
        writeFile(fileSink,static_cast<Ins>(*code),offset);
        code++; 
    }

    if(TEXT_FILE == option)
    {
        fclose(fileSink);
    }
}

void writeFile(FILE* file, Ins ins, u64& offset)
{
    switch (ins)
    {
    case INS_END:
    case INS_RET:
        fwrite(INS_STR[ins],strlen(INS_STR[ins]),1,file);
        fwrite("\n",strlen("\n"),1,file);
        offset++;
        break;
    default:
        fwrite(INS_STR[ins],strlen(INS_STR[ins]),1,file);
        fwrite("\n",strlen("\n"),1,file);
        break;
    }
}
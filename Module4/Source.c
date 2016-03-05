#include <stdio.h>
#include <stdarg.h>



char* va(char* format, ...)
{
    va_list va;
    static char b[1024];

    va_start(va, format);

    vsprintf_s(b, 1024, format, va);


    va_end(va);
    return b;
}

int main()
{
    //char* res = va("%d", 10);

    FILE* f;
    fopen_s(&f, "Source.c", "rb");
    
    int		pos;
    int		end;
    
    pos = ftell(f);
    fseek(f, 0, SEEK_END);

    end = ftell(f);
    fseek(f, 15, SEEK_SET);
    
    char FileData[1024];
    fread_s(FileData, 1024, 1, end, f);

    FILE* WriteFile;
    fopen_s(&WriteFile, "Source.out", "wb");

    size_t BytesWritten = fwrite(FileData, 1, end, WriteFile);
    fclose(WriteFile);
    fclose(f);

    return 0;
}

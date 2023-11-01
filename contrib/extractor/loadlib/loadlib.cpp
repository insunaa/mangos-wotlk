#define _CRT_SECURE_NO_DEPRECATE

#include "loadlib.h"
#include "../mpq_libmpq.h"

#include <stdio.h>

class MPQFile;

FileLoader::FileLoader()
{
    data = nullptr;
    data_size = 0;
}

FileLoader::~FileLoader()
{
    free();
}

bool FileLoader::loadFile(char* filename, bool log)
{
    free();
    MPQFile mf(filename);
    if (mf.isEof())
    {
        if (log)
            printf("No such file %s\n", filename);
        return false;
    }

    data_size = mf.getSize();

    data = new uint8 [data_size];
    mf.read(data, data_size);
    mf.close();
    if (prepareLoadedData())
        return true;

    printf("Error loading %s\n", filename);
    mf.close();
    free();
    return false;
}

bool FileLoader::prepareLoadedData()
{
    // Check version
    memcpy(&version, data, sizeof(file_MVER));
    if (version.fcc != fcc_MVER)
    {
        printf("wrong fcc: %#010x\n", version.fcc);
        return false;
    }
    if (version.ver != FILE_FORMAT_VERSION)
    {
        printf("wrong ver: %d\n", version.ver);
        return false;
    }
    return true;
}

void FileLoader::free()
{
    if (data) delete[] data;
    data = nullptr;
    data_size = 0;
}

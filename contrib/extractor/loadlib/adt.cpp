#include "loadlib.h"
#define _CRT_SECURE_NO_DEPRECATE

#include "adt.h"
#include <cstdio>

// Helper
int holetab_h[4] = {0x1111, 0x2222, 0x4444, 0x8888};
int holetab_v[4] = {0x000F, 0x00F0, 0x0F00, 0xF000};

bool isHole(int holes, int i, int j)
{
    int testi = i / 2;
    int testj = j / 4;
    if (testi > 3) testi = 3;
    if (testj > 3) testj = 3;
    return (holes & holetab_h[testi] & holetab_v[testj]) != 0;
}

//
// Adt file loader class
//
ADT_file::ADT_file()
{}

ADT_file::~ADT_file()
{
    free();
}

void ADT_file::free()
{
    FileLoader::free();
}

//
// Adt file check function
//
bool ADT_file::prepareLoadedData()
{
    // Check parent
    if (!FileLoader::prepareLoadedData())
        return false;

    // Check and prepare MHDR
    memcpy(&a_grid, GetData() + 8 + version.size, sizeof(adt_MHDR));
    if (!a_grid.prepareLoadedData(this))
        return false;

    return true;
}

bool adt_MHDR::prepareLoadedData(FileLoader* parent)
{
    if (fcc != fcc_MHDR)
    {
        printf("adt-fcc!=fcc_MHDR");
        return false;
    }

    if (size != sizeof(adt_MHDR) - 8)
    {
        printf("adt-wrong-sizeof-mhdr");
        return false;
    }

    // Check and prepare MCIN
    
    if (offsMCIN && !getMCIN(parent).prepareLoadedData(parent, offsMCIN))
    {
        printf("adt-wrong-getmcin");
        return false;
    }

    // Check and prepare MH2O
    if (offsMH2O && !getMH2O(parent).prepareLoadedData())
    {
        printf("adt-wrong-mh2o");
        return false;
    }

    return true;
}

bool adt_MCIN::prepareLoadedData(FileLoader* parent, uint32 offsMCIN)
{
    if (fcc != fcc_MCIN)
        return false;

    // Check cells data
    for (int i = 0; i < ADT_CELLS_PER_GRID; i++)
        for (int j = 0; j < ADT_CELLS_PER_GRID; j++)
            if (cells[i][j].offsMCNK && !getMCNK(i, j, parent, offsMCIN).prepareLoadedData(parent, cells[i][j].offsMCNK))
            {
                printf("mcin-wrong-mcnk");
                return false;
            }

    return true;
}

bool adt_MH2O::prepareLoadedData()
{
    if (fcc != fcc_MH2O)
    {
        printf("m2ho-wrong-fcc");
        return false;
    }

    // Check liquid data
//    for (int i=0; i<ADT_CELLS_PER_GRID;i++)
//        for (int j=0; j<ADT_CELLS_PER_GRID;j++)

    return true;
}

bool adt_MCNK::prepareLoadedData(FileLoader* parent, uint32 offsMCNK)
{
    if (fcc != fcc_MCNK)
    {
        printf("mcnk-wrong-fcc");
        return false;
    }

    // Check height map
    if (offsMCVT && !getMCVT(parent, offsMCNK).prepareLoadedData())
        return false;
    // Check liquid data
    if (offsMCLQ && !getMCLQ(parent, offsMCNK).prepareLoadedData())
        return false;

    return true;
}

bool adt_MCVT::prepareLoadedData()
{
    if (fcc != fcc_MCVT)
        return false;

    if (size != sizeof(adt_MCVT) - 8)
        return false;

    return true;
}

bool adt_MCLQ::prepareLoadedData()
{
    return fcc == fcc_MCLQ;
}

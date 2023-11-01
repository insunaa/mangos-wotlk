#define _CRT_SECURE_NO_DEPRECATE

#include "wdt.h"
#include <cstdio>

bool wdt_MWMO::prepareLoadedData()
{
    if (fcc != fcc_MWMO)
        printf("wrong wmo-fcc: %#010x\n", fcc);
    return fcc == fcc_MWMO;
}

bool wdt_MPHD::prepareLoadedData()
{
    if (fcc != fcc_MPHD)
        printf("wrong phd-fcc: %#010x\n", fcc);
    return fcc == fcc_MPHD;
}

bool wdt_MAIN::prepareLoadedData()
{
    if (fcc != fcc_MAIN)
        printf("wrong main-fcc: %#010x\n", fcc);
    return fcc == fcc_MAIN;
}

WDT_file::WDT_file()
{
}

WDT_file::~WDT_file()
{
    free();
}

void WDT_file::free()
{
    FileLoader::free();
}

bool WDT_file::prepareLoadedData()
{
    // Check parent
    if (!FileLoader::prepareLoadedData())
        return false;
    uint32 offset = version.size + 8;
    memcpy(&mphd, GetData() + offset, sizeof(wdt_MPHD));
    if (!mphd.prepareLoadedData())
        return false;
    offset += mphd.size + 8;
    memcpy(&main, GetData() + offset, sizeof(wdt_MAIN));
    if (!main.prepareLoadedData())
        return false;
    offset += main.size + 8;
    memcpy(&wmo, GetData() + offset, sizeof(wdt_MWMO));
    if (!wmo.prepareLoadedData())
        return false;
    return true;
}

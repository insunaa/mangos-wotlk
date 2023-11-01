#define _CRT_SECURE_NO_DEPRECATE

#include "wdt.h"

bool wdt_MWMO::prepareLoadedData()
{
    return fcc == fcc_MWMO;
}

bool wdt_MPHD::prepareLoadedData()
{
    return fcc == fcc_MPHD;
}

bool wdt_MAIN::prepareLoadedData()
{
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

    memcpy(&mphd, (uint8*)&version + version.size + 8, sizeof(wdt_MPHD));
    if (!mphd.prepareLoadedData())
        return false;
    memcpy(&main, (uint8*)&mphd + mphd.size + 8, sizeof(wdt_MAIN));
    if (!main.prepareLoadedData())
        return false;
    memcpy(&wmo, (uint8*)&main + main.size + 8, sizeof(wdt_MWMO));
    if (!wmo.prepareLoadedData())
        return false;
    return true;
}

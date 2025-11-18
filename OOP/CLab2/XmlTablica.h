#pragma once

#include <string>
#include <vector>

#include "Tablica.h"

class XmlTablica :
    public Tablica
{
public:
    std::wstring version;
    std::wstring coding;
    std::vector<std::pair<Item, Item>> getDuplicate();
};


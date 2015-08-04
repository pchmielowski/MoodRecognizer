#pragma once
#include "Ubm.h"
#include "Types.h"

class UbmLoader
{
public:
	//virtual void load(FileName ubmFileName) = 0; to bêdzie konstruktor
	virtual Ubm getUbm() = 0;
	virtual ~UbmLoader() {}
};

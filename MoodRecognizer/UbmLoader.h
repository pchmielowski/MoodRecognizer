#pragma once
#include "Ubm.h"
#include "Types.h"

class UbmLoader
{
public:
	//virtual void load(FileName ubmFileName) = 0; to b�dzie konstruktor
	virtual Ubm getUbm() {
		return Ubm();
	}
	virtual ~UbmLoader() {}
};

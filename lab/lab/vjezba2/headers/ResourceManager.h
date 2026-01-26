#pragma once

#include <iostream>
#include <string>

#include "Object.h"

class ResourceManager
{
public:
	static bool getScene(const std::string& name, Object& object);
};
#pragma once

#include <iostream>
#include <string>

#include "Object.h"
#include "Lines.h"

class ResourceManager
{
public:
	static bool getScene(const std::string& name, Object& object);
	static bool getVertices(const std::string& name, Lines& lines);
};
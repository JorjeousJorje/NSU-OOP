#pragma once
#include "command.hpp"

class builder
{
public:
	virtual command_uptr construct() const = 0;
	virtual ~builder() = default;
};
using builder_uptr = std::unique_ptr<builder>;


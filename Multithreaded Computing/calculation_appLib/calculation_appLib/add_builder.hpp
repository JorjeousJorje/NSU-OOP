#pragma once
#include "command_builder.hpp"
#include "add_command.hpp"

class add_builder final : public builder
{
public:
	explicit add_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}
	
	command_uptr construct() const override {
		return std::make_unique<add_command>(lhs_, rhs_);
	}
	

private:
	double lhs_;
	double rhs_;
};

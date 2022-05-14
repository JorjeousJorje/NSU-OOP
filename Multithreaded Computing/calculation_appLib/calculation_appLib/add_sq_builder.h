#pragma once
#include "command_builder.hpp"
#include "add_sq_command.hpp"

class add_sq_builder final : public builder
{
public:
	explicit add_sq_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}

	command_uptr construct() const override {
		return std::make_unique<add_sq_command>(lhs_, rhs_);
	}

private:
	double lhs_;
	double rhs_;
};

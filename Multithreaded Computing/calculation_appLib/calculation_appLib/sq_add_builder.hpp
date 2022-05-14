#pragma once
#include "command_builder.hpp"
#include "sq_add_command.hpp"

class sq_add_builder final : public builder
{
public:
	explicit sq_add_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}

	command_uptr construct() const override {
		return std::make_unique<sq_add_command>(lhs_, rhs_);
	}

private:
	double lhs_;
	double rhs_;
};

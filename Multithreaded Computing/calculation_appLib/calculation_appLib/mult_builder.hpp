#pragma once
#include "command_builder.hpp"
#include "mult_command.hpp"

class mult_builder final : public builder
{
public:
	explicit mult_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}

	command_uptr construct() const override {
		return std::make_unique<mult_command>(lhs_, rhs_);
	}

private:
	double lhs_;
	double rhs_;
};

#pragma once
#include "command_builder.hpp"
#include "div_command.hpp"

class div_builder final : public builder
{
public:
	explicit div_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}

	command_uptr construct() const override {
		return std::make_unique<div_command>(lhs_, rhs_);
	}

private:
	double lhs_;
	double rhs_;
};

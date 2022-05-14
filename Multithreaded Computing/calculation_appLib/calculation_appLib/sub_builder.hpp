#pragma once
#include "command_builder.hpp"
#include "sub_command.hpp"

class sub_builder final : public builder
{
public:
	explicit sub_builder(const double lhs, const double rhs) : lhs_(lhs), rhs_(rhs) {}

	command_uptr construct() const override {
		return std::make_unique<sub_command>(lhs_, rhs_);
	}

private:
	double lhs_;
	double rhs_;
};

#pragma once
#include "command.hpp"

class mult_command final : public command
{
public:
	mult_command() = delete;
	mult_command(const double lhs, const double rhs) : lhs_{ lhs }, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "mult", lhs_ * rhs_ };
	}

private:
	double lhs_;
	double rhs_;
};

#pragma once
#include "command.hpp"

class add_sq_command final : public command
{
public:
	add_sq_command() = delete;
	add_sq_command(const double lhs, const double rhs) : lhs_{ lhs }, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "add_sq", lhs_ * lhs_ + rhs_ * rhs_ };
	}

private:
	double lhs_;
	double rhs_;
};

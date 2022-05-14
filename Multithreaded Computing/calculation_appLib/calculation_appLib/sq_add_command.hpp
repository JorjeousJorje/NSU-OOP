#pragma once
#include "command.hpp"

class sq_add_command final : public command
{
public:
	sq_add_command() = delete;
	sq_add_command(const double lhs, const double rhs) : lhs_{ lhs }, rhs_{ rhs } {}
	
	cmd_result execute() const override {
		return { "sq_add", (lhs_ + rhs_) * (lhs_ + rhs_) };
	}

private:
	double lhs_;
	double rhs_;
};
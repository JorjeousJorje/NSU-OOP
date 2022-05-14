#include "test.hpp"
#include "budget.hpp"
#include "date.hpp"


void start_test(std::istream& budget_, std::istream& expenses, std::ostream& out, std::istream& date_)
{
	const budget bud_get(budget_, expenses);
	bud_get.count_expenses(out, date_);
}

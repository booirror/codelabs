#include <iostream>
#include <queue>
#include <vector>

using my_pair_t = std::pair<size_t, bool>;
using my_container_t = std::vector<my_pair_t>;

void test_priority_queue()
{
	auto my_comp = [](const my_pair_t e1, const my_pair_t& e2)
	{
		return e1.first > e2.first;
	};
	std::priority_queue<my_pair_t, my_container_t, decltype(my_comp)> queue(my_comp);
	queue.push(std::make_pair(5, true));
	queue.push(std::make_pair(3, false));
	queue.push(std::make_pair(7, true));

	std::cout << std::boolalpha;
	while (!queue.empty()) {
		const auto& p = queue.top();
		std::cout << p.first << " " << p.second << std::endl;
		queue.pop();
	}
}
#include <vector>

template <typename T, typename U>
size_t	find_elem( const std::vector<T> tab, const U elem) {
	const size_t len = tab.size();
	for (size_t i = 0; i < len; i++) {
		if (tab[i] == elem)
			return (i + 1);
	}
	return (0);
}

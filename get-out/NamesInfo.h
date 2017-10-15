#ifndef H_NAMES_INFO
#define H_NAMES_INFO

#include <vector>


struct NamesInfo {
public:
	~NamesInfo() {
		items.clear();
		interactables.clear();
	}

	std::vector<std::string> items;
	std::vector<std::string> interactables;
};


#endif // !H_NAMES_INFO

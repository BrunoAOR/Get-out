#ifndef H_ENTITY
#define H_ENTITY

#include <string>


class Entity
{
public:
	Entity();
	~Entity();

	std::string getName() const
	{
		return name;
	}
	
private:
	std::string name;
};


#endif // !H_ENTITY

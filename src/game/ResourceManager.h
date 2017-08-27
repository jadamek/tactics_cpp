#ifndef TACTICS_RESOURCE_MANAGER_H
#define TACTICS_RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//================================================================================
// ** Resource Manager
//================================================================================
// Generalized container for loading resources in order to catalog them into a
// single register
//================================================================================
template <typename Type>
class ResourceManager {
public:
	ResourceManager() {}
	ResourceManager(const ResourceManager&) {}
	virtual ~ResourceManager();

	const Type&	load(const std::string& filepath);
	const Type&	retrieve(const std::string& filepath);

	void		unload(const std::string& filepath);
	void		unload(const Type* resource);

	void		operator=(const ResourceManager&) {}

protected:
	std::map<const std::string, Type*>	files_;
};

// Common Resource Containers
typedef ResourceManager<sf::Texture> TextureManager;

#endif

#include "ResourceManager.inl"

#pragma once


#include <functional>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <string>

#include "engine/resource/IResource.h"

namespace ds
{
/**
 * Creates products of type IResource, the game data resources such as mesh,
 * texture etc. Use dependency injection to
 */
class ResourceFactory
{
public:
    /**
     * Register a resource loader (creator) that will be used to create
     * resources of the given type.
     *
     * @param  creatorFunction
     * std::function<std::unique_ptr<IResource>(std::string), pointer to
     * function to be used to create resource.
     */
    template <typename T>
    void RegisterCreator(
        std::function<std::unique_ptr<IResource>(std::string)> creatorFunction);

    /**
     * Create a resource of the given type.
     *
     * @param   fileIn  std::string, file path to the resource
     * @return          std::unique_ptr<IResource>, pointer to resource created,
     * resource factory maintains no ownership of the pointer and no
     * responsibility for freeing it's associated memory.
     */
    template <typename T>
    std::unique_ptr<T> CreateResource(std::string fileIn) const;

private:
    std::map<std::type_index,
             std::function<std::unique_ptr<IResource>(std::string)>> m_creators;
};

#include "engine/resource/ResourceFactory.hpp"
}

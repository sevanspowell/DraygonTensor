#pragma once

#include <fstream>
#include <functional>
#include <map>
#include <typeindex>
#include <typeinfo>

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
     * std::function<std::unique_ptr<IResource>(std::fstream &), pointer to
     * function to be used to create resource.
     */
    template <typename T>
    void
    RegisterCreator(std::function<std::unique_ptr<IResource>(std::fstream &)>
                        creatorFunction);

    /**
     * Create a resource of the given type.
     *
     * @param   fileIn  std::fstream &, file stream to load resource from.
     * @return          std::unique_ptr<IResource>, pointer to resource created,
     * resource factory maintains no ownership of the pointer and no
     * responsibility for freeing it's associated memory.
     */
    template <typename T>
    std::unique_ptr<IResource> CreateResource(std::fstream &fileIn) const;

private:
    std::map<std::type_index,
             std::function<std::unique_ptr<IResource>(std::fstream &)>>
        m_creators;
};

#include "engine/resource/ResourceFactory.hpp"
}

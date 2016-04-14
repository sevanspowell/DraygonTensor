template <typename T>
void ResourceFactory::RegisterCreator(
    std::function<std::unique_ptr<IResource>(std::string)> creatorFunction)
{
    // Insert creator function at typeid
    m_creators[typeid(T)] = creatorFunction;
}

template <typename T>
std::unique_ptr<IResource>
ResourceFactory::CreateResource(std::string fileIn) const
{
    // Return nullptr if no creator for type exists
    std::unique_ptr<IResource> resourcePtr(nullptr);

    // Find creator for type
    std::map<std::type_index, std::function<std::unique_ptr<IResource>(
                                 std::string)>>::const_iterator it =
        m_creators.find(typeid(T));
    // If creator for type exists
    if (it != m_creators.end())
    {
        // Get appropriate creator function and call it
        resourcePtr = it->second(fileIn);
    }

    return resourcePtr;
}

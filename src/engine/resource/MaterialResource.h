#include <memory>
#include <string>

#include "engine/resource/IResource.h"

namespace ds
{
class MaterialResource : public IResource
{
public:
    /**
     * Create a material resource from file.
     *
     * @param   filePath  std::string, file path to create material resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to material resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string filePath);

    /**
     * Get the file path to the resource.
     *
     * @return  const std::string &, resource file path.
     */
    virtual const std::string &GetResourceFilePath() const;

    /**
     * Set the file path to the resource.
     *
     * @param  filePath  const std::string &, file path of this resource.
     */
    virtual void SetResourceFilePath(const std::string &filePath);

private:
    // This resource's file path
    std::string m_filePath;
};
}

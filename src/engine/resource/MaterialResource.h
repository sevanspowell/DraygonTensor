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

private:
};
}

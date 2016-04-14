#include <memory>

#include "engine/resource/IResource.h"

namespace ds
{
class MaterialResource : public IResource
{
public:
    /**
     * Create a material resource from file.
     *
     * @param   fileIn  std::fstream &, file stream to create material resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to material resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::fstream &fileIn);

private:
};
}

#include <memory>
#include <string>

#include "engine/resource/IResource.h"

namespace ds
{
class ShaderResource : public IResource
{
public:
    /**
     * Create a shader resource from file.
     *
     * @param   filePath  std::string &, file path to create shader resource
     * from.
     * @return          std::unique_ptr<IResource>, pointer to shader resource
     * created.
     */
    static std::unique_ptr<IResource> CreateFromFile(std::string &filePath);

private:
};
}

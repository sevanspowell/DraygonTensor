#pragma once

namespace ds
{
/**
 * @author Samuel Evans-Powell
 */
class IResource
{
public:
    /**
     * Get the file path to the resource.
     *
     * @return  const std::string &, resource file path.
     */
    virtual const std::string &GetResourceFilePath() const = 0;

    /**
     * Set the file path to the resource.
     *
     * @param  filePath  const std::string &, file path of this resource.
     */
    virtual void SetResourceFilePath(const std::string &filePath) = 0;
};
}

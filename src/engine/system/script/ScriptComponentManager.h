#pragma once

#include "engine/entity/ComponentManager.h"
#include "engine/system/script/ScriptComponent.h"

namespace ds
{
class ScriptComponentManager : public ComponentManager<ScriptComponent>
{
public:
    /**
     * Get script path for given script component instance.
     */
    const std::string &GetScriptPath(Instance i) const;

    /**
     * Set the script path for the given script component instance.
     */
    void SetScriptPath(Instance i, const std::string &path);

    /**
     * Is script component initialized?
     */
    bool IsInitialized(Instance i) const;

    /**
     * Set whether or not the script component is initialized.
     */
    void SetInitialized(Instance i, bool isInitialized);
};
}

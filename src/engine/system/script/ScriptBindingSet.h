#pragma once

#include <vector>

#include "engine/system/script/LuaHelper.h"

#define SCRIPT_FN lua_CFunction

namespace ds
{
/**
 * Collection of functions which make up the script bindings of a class.
 */
class ScriptBindingSet
{
public:
    /**
     * Add a function to the script binding set.
     *
     * A function should be thought of as a static method. It is not given an
     * instance to operate on. If a function with the given name already exists
     * in the set, this function replaces it.
     *
     * @param  functionName  const char *, name to give function in the
     * scripting context.
     * @param  functionPtr   SCRIPT_FN, pointer to function.
     */
    void AddFunction(const char *functionName, SCRIPT_FN functionPtr);

    /**
     * Add a method to the script binding set.
     *
     * A method should be thought of as a member method. It IS given an
     * instance to operate on. If a method with the given name already exists
     * in the set, this method replaces it.
     *
     * @param  methodName  const char *, name to give method in the
     * scripting context.
     * @param  methodPtr   SCRIPT_FN, pointer to method.
     */
    void AddMethod(const char *methodName, SCRIPT_FN methodPtr);

    /**
     * Get the function pair at the given index.
     *
     * @param   index  size_t, index of function pair to grab.
     * @return         const std::pair<const char *, SCRIPT_FN> &, function pair
     * at the given index.
     */
    const std::pair<const char *, SCRIPT_FN> &
    GetFunctionPair(size_t index) const;

    /**
     * Get the method pair at the given index.
     *
     * @param   index  size_t, index of method pair to grab.
     * @return         const std::pair<const char *, SCRIPT_FN> &, method pair
     * at the given index.
     */
    const std::pair<const char *, SCRIPT_FN> &
    GetMethodPair(size_t index) const;

    /**
     * Get the number of functions in the set.
     *
     * @return  size_t, number of functions in the set.
     */
    size_t GetNumFunctions() const;

    /**
     * Get the number of methods in the set.
     *
     * @return  size_t, number of methods in the set.
     */
    size_t GetNumMethods() const;

private:
    std::vector<std::pair<const char *, SCRIPT_FN>> m_functions;
    std::vector<std::pair<const char *, SCRIPT_FN>> m_methods;
};
}

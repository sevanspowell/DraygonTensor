#pragma once

#include <string>
#include <vector>

namespace ds
{
/**
 * The StringIntern singleton is used to intern strings.
 *
 * Essentially, a string is entered into the class and an id is returned that
 * uniquely refers to that string. That string can then be retrieved from
 * anywhere in the program using that id. This is useful because our messaging
 * system does not allow the passing of std::strings (a non-POD type).
 */
class StringIntern
{
public:
    /**
     * Assumes StringId is a POD type.
     */
    typedef uint32_t StringId;

    /**
     * Get the (only) StringIntern instance.
     *
     * @return  StringIntern &, (only) instance of StringIntern class.
     */
    static StringIntern &Instance();

    /**
     * Intern the given string, receiving a StringId which can be used to refer
     * to that string.
     *
     * @param   string  std::string, string to intern.
     * @return          StringId, id used to refer to given string.
     */
    StringId Intern(std::string string);

    /**
     * Get the string associated with the given StringId.
     *
     * @param   id  StringId, string id to get string for.
     * @return      const std::string &, string associated with given string id.
     */
    const std::string &GetString(StringId id) const;

private:
    /**
     * Private StringIntern constructor, to ensure that no more than one
     * instance of this class is created.
     */
    StringIntern();

    std::vector<std::string> m_strings;
};
}

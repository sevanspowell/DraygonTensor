#pragma once

#include <vector>

#include "rapidjson/document.h"

namespace ds
{
/**
 * Engine configuration class.
 *
 * This class abstracts away the details of the configuration loaded by the
 * engine and allows users to get data from the configuration file, configuring
 * themselves accordingly.
 */
class Config
{
public:
    /**
     * Default constructor.
     */
    Config();

    /**
     * Attempt to load the configuration from file.
     *
     * This method (or LoadMemory) must be called before the class is used.
     *
     * Note: will clear any previous config loaded.
     *
     * @param   filePath  const std::string &, path to file on disk.
     * @return            bool, TRUE if the file was loaded successfully, FALSE
     *                    otherwise.
     */
    bool LoadFile(const std::string &filePath);

    /**
     * Attempt to load the configuration from memory.
     *
     * This method (or LoadFile) must be called before the class is used.
     *
     * Note: will clear any previous config loaded.
     *
     * @param   filePath  const std::string &, path to file on disk.
     * @return            bool, TRUE if the file was loaded successfully,
     FALSE
     *                    otherwise.
     */
    bool LoadMemory(const std::string &string);

    /**
     * Has a config file been loaded successfully?
     *
     * That is, is the Config class instance in a usable state?
     *
     * @return  bool, TRUE if a config file has beeen loaded successfully, FALSE
     * otherwise.
     */
    bool IsLoaded() const;

    /**
     * Attempt to get an unsigned int value from a string key-value pair in
     * the config file. If the key isn't found or the value isn't an int, the
     * function will return FALSE.
     *
     * Key may be specified in 'table access form' (i.e.
     * 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if no config has been loaded.
     * @post Unsigned Int pointed to by second parameter will not be
     * modified if the method returns FALSE.
     *
     * @param   key   const std::string &, period seperated list of tokens,
     *                uniquely identifying an unsigned int primitive in the
     *                configuration file.
     * @param   int   uint *, where unsigned int value should be placed if
     * successfully retrieved.
     * @return        bool, TRUE if the value was retrieved successfully,
     * FALSE otherwise.
     */
    bool GetUnsignedInt(const std::string &key, unsigned int *uint) const;

    /**
     * Attempt to get an int value from a string key-value pair in
     * the config file. If the key isn't found or the value isn't an int, the
     * function will return FALSE.
     *
     * Key may be specified in 'table access form' (i.e.
     * 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if no config has been loaded.
     * @post Int pointed to by second parameter will not be
     * modified if the method returns FALSE.
     *
     * @param   key      const std::string &, period seperated list of tokens,
     * uniquely identifying an unsigned int primitive in the configuration file.
     * @param   integer  int *, where int value should be placed if successfully
     * retrieved.
     * @return           bool, TRUE if the value was retrieved successfully,
     * FALSE otherwise.
     */
    bool GetInt(const std::string &key, int *integer) const;

    /**
     * Attempt to get a float array from a string key-value pair in the config
     * file. If the key isn't found or the value isn't an ALL float array, the
     * function will return FALSE.
     *
     * Key may be specified in 'table access form' (i.e.
     * 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if no config has been loaded.
     * @post vector pointed to by second parameter will not be
     * modified if the method returns FALSE.
     *
     * @param   key    const std::string &, period seperated list of tokens,
     * uniquely identifying an array in the configuration file.
     * @param   array  std::vector<float> *, where array data will be placed if
     * successfuly retrieved.
     * @return         bool, TRUE if the value was retrieved successfully,
     * FALSE otherwise.
     */
    bool GetFloatArray(const std::string &key, std::vector<float> *array) const;

    /**
     * Attempt to get a bool value from a string key-value pair in the
     * config file. If the key isn't found or the value isn't a bool,
     * the function will return FALSE.
     *
     * Key may be specified in 'table access form' (i.e. 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if no config has been loaded.
     * @post bool pointed to by second parameter will not be modified if
     * the method returns FALSE.
     *
     * @param   key      const std::string &, period seperated list of tokens,
     *                   uniquely identifying a bool primitive in the
     *                   configuration file.
     * @param   boolean  bool *, where bool value should be placed if
     *                   successfully retrieved.
     * @return           bool, TRUE if the value was retrieved successfully,
     *                   FALSE otherwise.
     */
    bool GetBool(const std::string &key, bool *boolean) const;

    /**
     * Attempt to get string value from a string key-value pair in the
     * config file. If the key isnt' found or the value isn't a string,
     * the function will return FALSE.
     *
     * Key may be specified in 'table access form' (i.e. 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if no config has been loaded.
     * @post string pointed to by second parameter will not be modified if the
     * method returns FALSE.
     *
     * @param   key     const std::string &, period seperated list of tokens,
     *                  uniquely identifying a string primitive in the
     *                  configuration file.
     * @param   string  std::string *, where string value should be placed if
     *                  successfully retrieved.
     * @return          bool, TRUE if the value was retrieved successfully,
     *                  FALSE otherwise.
     */
    bool GetString(const std::string &key, std::string *string) const;

    /**
     * Get the string keys of the specified object.
     *
     * For example, given the following config file:
     * "Input": {
     *   "InputContextName": {
     *     "keyName": "message string"
     *   },
     *   "Default": {
     *      "`": "console_toggle"
     *   }
     * }
     * GetObjectContents("Input") would return "InputContextName" and "Default"
     * and GetObjectContents("Input.InputContextName") would return "keyName".
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method. This method will return FALSE if a configuration has not been
     * loaded.
     *
     * @param   key     const std::string &, period seperated list of tokens,
     *                  uniquely identifying an object in the
     *                  configuration file. The object must be made up of
     *                  key-value pairs.
     * @return          std::vector<std::string>, string keys contained within
     *                  specified object.
     */
    std::vector<std::string> GetObjectKeys(const std::string &key) const;

    /**
     * Stringify an object with the given key
     *
     * For example, given the following config file:
     * "Input": {
     *   "InputContextName": {
     *     "keyName": "message string"
     *   },
     *   "Default": {
     *      "`": "console_toggle"
     *   }
     * }
     * StringifyObject("Input.Default") would return:
     * "Default": {
     *   "`": "console_toggle"
     * }
     *
     * @pre  A configuration file has been loaded via this classes 'Load'
     * method.
     *
     * @param   key     const std::string &, period seperated list of tokens,
     *                  uniquely identifying an object in the
     *                  configuration file.
     * @return          std::string, stringified object
     */
    std::string StringifyObject(const std::string &key) const;

    /**
     * Add a float array with the given key to the config.
     *
     * Key may be specified in 'table access form' (i.e.
     * 'Window.dimensions.x').
     * In this case, each successive token (seperated by period character)
     * indexes into the object the previous token refers to.
     *
     * @param   key    const std::string &, period seperated list of tokens,
     * uniquely identifying an array in the configuration file.
     * @param   array  const std::vector<float> &, array data to add to
     * document.
     */
    void AddFloatArray(const std::string &key,
                       const std::vector<float> &array);

private:
    // TODO: Documentation
    rapidjson::Value::ConstMemberIterator
    GetDocumentMember(const std::string &key) const;

    // TODO: Documentation
    rapidjson::Value::MemberIterator
    GetDocumentMember(const std::string &key);

    rapidjson::Document m_document;

    bool m_isLoaded;
};
}

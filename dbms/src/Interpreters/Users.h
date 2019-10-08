#pragma once

#include <Core/Types.h>
#include <Access/Authentication.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace Poco
{
    namespace Net
    {
        class IPAddress;
    }

    namespace Util
    {
        class AbstractConfiguration;
    }
}


namespace DB
{


/// Allow to check that address matches a pattern.
class IAddressPattern
{
public:
    virtual bool contains(const Poco::Net::IPAddress & addr) const = 0;
    virtual ~IAddressPattern() {}
};


class AddressPatterns
{
private:
    using Container = std::vector<std::shared_ptr<IAddressPattern>>;
    Container patterns;

public:
    bool contains(const Poco::Net::IPAddress & addr) const;
    void addFromConfig(const String & config_elem, const Poco::Util::AbstractConfiguration & config);
};


/** User and ACL.
  */
struct User
{
    String name;

    /// Required password.
    Authentication authentication;

    String profile;
    String quota;

    AddressPatterns addresses;

    /// List of allowed databases.
    using DatabaseSet = std::unordered_set<std::string>;
    DatabaseSet databases;

    /// List of allowed dictionaries.
    using DictionarySet = std::unordered_set<std::string>;
    DictionarySet dictionaries;

    /// Table properties.
    using PropertyMap = std::unordered_map<std::string /* name */, std::string /* value */>;
    using TableMap = std::unordered_map<std::string /* table */, PropertyMap /* properties */>;
    using DatabaseMap = std::unordered_map<std::string /* database */, TableMap /* tables */>;
    DatabaseMap table_props;

    User(const String & name_, const String & config_elem, const Poco::Util::AbstractConfiguration & config);
};


}

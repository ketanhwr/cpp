#pragma once

#include <memory>

namespace kg {

template <
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename Allocator = std::allocator<std::pair<const Key, Value>>
> class hashmap
{
    enum class EntryType : uint8_t {
        EMPTY,
        VALUE,
        DELETED
    };

    using MapEntry = std::pair<const Key, Value>;

  public:

    // Default constructor
    hashmap() {
    }

  private:
    std::vector<EntryType> m_types;
    MapEntry* m_entries;
};

}

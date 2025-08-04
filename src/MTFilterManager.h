#pragma once

#include "MTFilter.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

class MTFilter;

class MTFilterManager {
public:
    MTFilterManager() = default;
    ~MTFilterManager() = default;
    MTFilterManager(const MTFilterManager&) = delete;
    MTFilterManager& operator=(const MTFilterManager&) = delete;
    MTFilterManager(MTFilterManager&&) = delete;
    MTFilterManager& operator=(MTFilterManager&&) = delete;

public:
    using Creator = std::function<MTFilter*()>;

    static MTFilterManager* instance() {
        static MTFilterManager g_instance;
        return &g_instance;
    }

    void registerFilter(const std::string& name, const Creator& creator) {
        m_creators[name] = creator;
    }

    MTFilter* getFilter(const std::string& name) {
        if (m_filters.find(name) != m_filters.end()) {
            return m_filters[name].get();
        }
        if (m_creators.find(name) != m_creators.end()) {
            m_filters[name] = std::unique_ptr<MTFilter>(m_creators[name]());
            return m_filters[name].get();
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, Creator> m_creators;
    std::unordered_map<std::string, std::unique_ptr<MTFilter>> m_filters;
};

#define REGISTER_FILTER_CLASS(NAME, CLASS) \
    namespace { \
        struct CLASS##REGISTER { \
            CLASS##REGISTER() { \
                MTFilterManager::instance()->registerFilter(NAME, []() -> MTFilter* { \
                    return new CLASS(); \
                }); \
            } \
        }; \
        static CLASS##REGISTER g_CLASS##REGISTER; \
    }

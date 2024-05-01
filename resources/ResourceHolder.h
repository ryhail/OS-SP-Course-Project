#ifndef COURSE_RESOURCEHOLDER_H
#define COURSE_RESOURCEHOLDER_H

#include <map>
#include <memory>
#include <stdexcept>
#include <cassert>
#include <SFML/Graphics/Texture.hpp>
#include "Textures.h"

template<typename Identifier, typename Resource>
class ResourceHolder {
private:
    std::map<Identifier, std::unique_ptr<Resource>> resourceMap;
public:
    void load(Identifier id, const std::string& filename);
    template<typename Parameter>
    void load(Identifier id, const std::string &filename, const Parameter& parameter);
    Resource& getResource(Identifier id);
    const Resource& getResource(Identifier id) const;
};

#include "ResourceHolder.inl"
#endif //COURSE_RESOURCEHOLDER_H

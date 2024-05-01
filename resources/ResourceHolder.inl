
template<typename Identifier, typename Resource>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename) {
    std::unique_ptr<Resource> resource(new Resource);
    if (!resource->loadFromFile(filename)) {
        throw std::runtime_error("ResourceHolder::load failed to load " + filename);
    }

    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

template<typename Identifier, typename Resource>
template<typename Parameter>
void ResourceHolder<Identifier, Resource>::load(Identifier id, const std::string &filename, const Parameter& parameter) {
    std::unique_ptr<Resource> resource(new Resource);
    if (!resource->loadFromFile(filename, parameter)) {
        throw std::runtime_error("ResourceHolder::load failed to load " + filename);
    }

    auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}
template<typename Identifier, typename Resource>
const Resource& ResourceHolder<Identifier, Resource>::getResource(Identifier id) const {
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}

template<typename Identifier, typename Resource>
Resource &ResourceHolder<Identifier, Resource>::getResource(Identifier id) {
    auto found = resourceMap.find(id);
    assert(found != resourceMap.end());
    return *found->second;
}


//
// Created by stuka on 17.11.2025.
//

#include "Serde.h"

std::vector<std::byte*>& SGCore::Serde::Impl::getExternalSerializers(size_t serializerTypeHash) noexcept
{
    static std::unordered_map<size_t, std::vector<std::byte*>> serializers;
    return serializers[serializerTypeHash];
}

std::vector<std::byte*>& SGCore::Serde::Impl::getExternalDeserializers(size_t serializerTypeHash) noexcept
{
    static std::unordered_map<size_t, std::vector<std::byte*>> deserializers;
    return deserializers[serializerTypeHash];
}

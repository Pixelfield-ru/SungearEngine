#pragma once
#include <iterator>

namespace SGCore::Utils
{
    /**
     * Owns and wraps iterator container (object with methods `.begin()` and `.end()`)
     * @tparam Container Iterator container type
     */
    template<typename ValueType>
    struct IteratorWrapper
    {

        virtual ~IteratorWrapper() noexcept = default;

        struct Iterator
        {
            virtual ~Iterator() noexcept = default;

            // ReSharper disable CppFunctionIsNotImplemented
            virtual Iterator& operator++() noexcept;
            virtual ValueType& operator*() noexcept;
            virtual bool operator!=(const Iterator& other) noexcept;
            // ReSharper restore CppFunctionIsNotImplemented
        };

        // ReSharper disable CppFunctionIsNotImplemented
        virtual Iterator& begin();
        virtual Iterator& end();
        // ReSharper restore CppFunctionIsNotImplemented

        template <typename Container> requires requires (Container container) {
            { container.begin() };
            { container.end() };
            { *std::declval<Container>().begin() } -> std::same_as<ValueType>;
        } && std::forward_iterator<decltype(std::declval<Container>().begin())>
        IteratorWrapper from(Container container) {
            using IterType = decltype(std::declval<Container>().begin());
            using ValueType = decltype(*std::declval<Container>().begin());

            struct IteratorWrapperImplementation : IteratorWrapper
            {
                ~IteratorWrapperImplementation() noexcept override = default;

                struct IteratorImplementation : Iterator
                {
                    ~IteratorImplementation() noexcept override = default;

                    Iterator& operator++() noexcept override {

                    }
                    ValueType& operator*() noexcept override;
                    bool operator!=(const Iterator& other) noexcept override;
                };

                Iterator& begin() noexcept override { return container.begin(); }
                Iterator& end() noexcept override { return container.end(); }
            };
        }
    };
}

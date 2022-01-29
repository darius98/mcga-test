#pragma once

#include <new>

#include <mcga/test/config.hpp>
#include <mcga/test/string.hpp>

namespace mcga::test {

namespace internal {

template<class T>
concept executable_t = requires(const T& obj) {
    { obj() } -> same_as<void>;
};

template<class T>
constexpr T&& move(T&& t) noexcept {
    return static_cast<T&&>(t);
}

template<class T>
constexpr T&& move(T& t) noexcept {
    return static_cast<T&&>(t);
}

}  // namespace internal

class Executable {
    template<internal::executable_t Callable>
    static void te_call(void* d) {
        (*static_cast<Callable*>(d))();
    }

    template<internal::executable_t Callable>
    static void te_mv_ctor(void* d, void* s) {
        new (d) Callable(internal::move(*static_cast<Callable*>(s)));
    }

    template<internal::executable_t Callable>
    static void te_destroy(void* d, bool deallocate) {
        if constexpr (MCGA_TEST_ALLOW_DYNAMIC_MEMORY) {
            if (deallocate) {
                delete static_cast<Callable*>(d);
            } else {
                static_cast<Callable*>(d)->~Callable();
            }
        } else {
            static_cast<Callable*>(d)->~Callable();
        }
    }

    struct VTable {
        void (*body)(void*);
        void (*mv_ctor)(void*, void*);
        void (*dtor)(void*, bool);
    };

    template<internal::executable_t Callable>
    static inline constexpr VTable vtableFor{
      &te_call<Callable>,
      &te_mv_ctor<Callable>,
      &te_destroy<Callable>,
    };

    struct alignas(void*) SBO {
        unsigned char storage[MCGA_TEST_EXECUTABLE_SBO_SIZE];
    };

  public:
    const VTable* vtable;
    void* data;
    Context context;
    SBO sbo;

    template<internal::executable_t Callable>
    requires(sizeof(Callable) <= MCGA_TEST_EXECUTABLE_SBO_SIZE
             && alignof(Callable) <= alignof(SBO))
      Executable(Callable callable, const Context& context = Context())
            : vtable(&vtableFor<Callable>), data(sbo.storage),
              context(context) {
        new (data) Callable(internal::move(callable));
    }

#if MCGA_TEST_ALLOW_DYNAMIC_MEMORY
    template<internal::executable_t Callable>
    Executable(Callable callable, const Context& context = Context())
            : vtable(&vtableFor<Callable>),
              data(new Callable(internal::move(callable))), context(context) {
    }
#endif

    Executable(const Executable&) = delete;

    Executable& operator=(const Executable&) = delete;

    Executable(Executable&& other) noexcept
            : vtable(other.vtable), data(other.data), context(other.context) {
        if (other.data == other.sbo.storage) {
            other.vtable->mv_ctor(sbo.storage, other.sbo.storage);
            data = sbo.storage;
        }
        other.data = nullptr;
    }

    Executable& operator=(Executable&& other) noexcept {
        if (this != &other) {
            if (data != nullptr) {
                vtable->dtor(data, data != sbo.storage);
            }
            vtable = other.vtable;
            data = other.data;
            context = other.context;
            if (other.data == other.sbo.storage) {
                other.vtable->mv_ctor(sbo.storage, other.sbo.storage);
                data = sbo.storage;
            }
            other.data = nullptr;
        }
        return *this;
    }

    ~Executable() {
        if (data != nullptr) {
            vtable->dtor(data, data != sbo.storage);
        }
    }

    void operator()() const {
        vtable->body(data);
    }
};

}  // namespace mcga::test
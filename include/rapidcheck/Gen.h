#pragma once

#include "rapidcheck/Traits.h"
#include "rapidcheck/Shrinkable.h"

namespace rc {

class Random;

/// The reference size. This is not a max limit on the generator size parameter
/// but serves as a guideline. In general, genenerators for which there is a
/// natural limit which is not too expensive to generate should max out at this.
/// This applies to, for example, generation of numbers but not to the
/// generation of collection where there is an associated cost to generating
/// large collections.
constexpr int kNominalSize = 100;

/// This class is the type of RapidCheck generators. A generator is essentially
/// a function which takes a `Random` and some generation parameters and returns
/// a randomly generated `Shrinkable`. This class has value semantics.
///
/// A generator can be created from any type which has the following:
///  - A method `Shrinkable<T> operator()(const Random &, int) const`
///  - A copy constructor which produces a semantically identical object.
template<typename T>
class Gen
{
public:
    /// The type of the values generated by this generator.
    typedef T ValueType;

    template<typename Impl,
             typename = typename std::enable_if<
                 !std::is_same<Decay<Impl>, Gen>::value>::type>
    Gen(Impl &&impl);

    /// Returns a `Shrinkable` generated used the given parameters.
    ///
    /// @param random  The random generator
    /// @param size    The generation size
    ///
    /// @return a random generated `Shrinkable`
    Shrinkable<T> operator()(const Random &random,
                             int size = kNominalSize) const noexcept;

    /// The meaning of this operator depends on the context in which it is used
    /// but mainly, it is used when creating a generator using `gen::exec` to
    /// pick a value in an impure context.
    T operator*() const;

    Gen(const Gen &other);
    Gen &operator=(const Gen &rhs);
    Gen(Gen &&other) = default;
    Gen &operator=(Gen &&rhs) = default;

private:
    class IGenImpl;

    template<typename Impl>
    class GenImpl;

    std::unique_ptr<IGenImpl> m_impl;
};

} // namespace rc

#include "Gen.hpp"

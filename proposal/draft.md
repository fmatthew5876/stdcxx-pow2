Efficient Power of 2 Manipulation
==========================================

* Document Number: NXXXX
* Date: 2014-08-26
* Programming Language C++, Numerics Working Group
* Reply-to: Matthew Fioravante <fmatthew5876@gmail.com>

The latest draft, reference header, and links to past discussions on github: 

* <https://github.com/fmatthew5876/stdcxx-pow2>

Introduction
=============================

This proposal adds support for a set of free functions which allow
one to efficiently detect and manipulate integral powers of 2.

This proposal was originally a small part of \[[N3864](#N3864)\] but has been broken out.

Impact on the standard
=============================

This proposal is a pure library extension. 
It does not require any changes in the core language and
does not depend on any other library extensions.
The proposal is composed entirely of free functions. The
proposed functions are added to the `<cmath>` header.

This proposal is being written for C++. None of these functions
are required to be present in `<math.h>`.

Motivation and Design
================

Current state of the art
=============================

Platform specific Optimal Implementations
=============================

Consider the function `bool ispow2(int x)`.
Here are 2 possible implementations.

    //Return true if x is a power of 2
    bool ispow2(unsigned int x) {
      return (x & (x-1)) == 0;
    }

<!-- -->

    //Returns the number of 1 bits in x, using a single instruction
    unsigned int parity(unsigned int x);

    //Return true if x is a power of 2
    bool ispow2(unsigned int x) {
      return parity(x) == 1;
    }

As we can see here, if the machine has a `parity` instruction in it's instruction set\[[N3864](#N3864)\], then the second implementation
may be more efficient than the first. Core functions like `ispow2()` with a wide general use case and
platform specific optimal implementations are a good candidate for standardization.

Technical Specification
====================

We will now describe the additions to the `<cmath>` header. This is a procedural library implemented
entirely using templated and overloaded free functions.
Each function is `constexpr` and `noexcept`. Unless otherwise noted, each pre-condition violation results in undefined behavior.
Each pre-condition has
either undefined or implementation defines results and each case is documented below.

## Concepts

* `std::is_integral<integral>::value == true`

## Specification

    template <typename integral>
      constexpr bool ispow2(intergal x) noexcept;

    template <typename integral>
      constexpr integral ceilp2(integral x) noexcept;

    template <typename integral>
      constexpr integral floorp2(integral x) noexcept;

## Shared Pre-Conditions 

The result is undefined if any of:

* `a == 0`
* `a` is not a power of 2
* `x < 0`

### Example Implementations

All of these implementations are trivial, efficient, and portable.

    template <typename Integral>
      constexpr bool ispow2(Integral x) noexcept {
        return x > 0 && (x & (x-1)) == 0;
        //return popcount(x) == 1;
      }

    //Round up to the next power of 2
    //Application: Growable containers whose size must be a power of 2
    //Application: Extending a 2d image size to a power of 2 for 3d graphics libraries (OpenGL/DirectX)
    template <typename Integral>
    constexpr14 Integral ceilp2(Integral x) noexcept {
      x = x-1;
      x |= x << 1;
      x |= x << 2;
      x |= x << 4;
      if(sizeof(x) > 1) {
        x |= x << 8;
        if(sizeof(x) > 2) {
          x |= x << 16;
          if(sizeof(x) > 4) {
            x |= x << 32;
          }
        }
      }
      return x + 1;
    }

    //Round down to the previous power of 2
    //Application: See ceilp2
    template <typename Integral>
    constexpr14 Integral floorp2(Integral x) noexcept {
      x |= shlr(x, 1);
      x |= shlr(x, 2);
      x |= shlr(x, 4);
      if(sizeof(x) > 1) {
        x |= shlr(x, 8);
        if(sizeof(x) > 2) {
          x |= shlr(x, 16);
          if(sizeof(x) > 4) {
            x |= shlr(x, 32);
          }
        }
      }
      return x - shlr(x, 1);
    }

NOTE: Implementations should support all extended integral types.

Use Cases
===============

* Binary arithmetic
* Power of 2 sized data structures
    * Efficient array based queues (circular queue)
* Scaling aritrary image dimensions to the next power of 2 for texturing in 3d grpahics API's such as OpenGL.

Acknowledgments
====================

Thank you to everyone who has been credited by N3864 for also helping with this proposal.
Special thanks to everyone on the std-proposals forum for their valuable insight and feedback.

References
==================

* <a name="N3864"></a>[N3864] Fioravante, Matthew *N3864 - A constexpr bitwise operations library for C++*, Available online at <https://github.com/fmatthew5876/stdcxx-bitops>

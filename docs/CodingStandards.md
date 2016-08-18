# Coding Standards

*The source of these standards can be found [here](http://read.pudn.com/downloads106/ebook/438804/CPP101.pdf).
I recommend giving it a read when you get a chance, use the following document as a reference.*

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-generate-toc again -->
**Table of Contents**

  - [Organizational and Policy Issues](#organizational-and-policy-issues)
      - [0. Don't sweat the small stuff.](#0-dont-sweat-the-small-stuff)
      - [1. Compile cleanly at high warning levels.](#1-compile-cleanly-at-high-warning-levels)
      - [2. Use an automated build system.](#2-use-an-automated-build-system)
      - [3. Use a version control system.](#3-use-a-version-control-system)
      - [4. Invest in code reviews.](#4-invest-in-code-reviews)
  - [Design Style](#design-style)
      - [5. Give one entity one cohesive responsibility.](#5-give-one-entity-one-cohesive-responsibility)
      - [6. Correctness, simplicity, and clarity come first.](#6-correctness-simplicity-and-clarity-come-first)
      - [7. Know when and how to code for scalability.](#7-know-when-and-how-to-code-for-scalability)
      - [8. Don't optimize prematurely.](#8-dont-optimize-prematurely)
      - [9. Don't pessimize prematurely.](#9-dont-pessimize-prematurely)
      - [10. Minimize global and shared data](#10-minimize-global-and-shared-data)
      - [11. Hide information](#11-hide-information)
      - [12. Know when and how to code for concurrency.](#12-know-when-and-how-to-code-for-concurrency)
      - [13. Ensure resources are owned by objects. Use explicit RAII and smart pointers.](#13-ensure-resources-are-owned-by-objects-use-explicit-raii-and-smart-pointers)
  - [Coding Style](#coding-style)
      - [14. Prefer compile- and link-time errors to run-time errors.](#14-prefer-compile--and-link-time-errors-to-run-time-errors)
      - [15. Use const proactively.](#15-use-const-proactively)
      - [16. Avoid macros](#16-avoid-macros)
      - [17. Avoid magic numbers](#17-avoid-magic-numbers)
      - [18. Declare variables as locally as possible](#18-declare-variables-as-locally-as-possible)
      - [19. Always initialize variables](#19-always-initialize-variables)
      - [20. Avoid long functions. Avoid deep nesting.](#20-avoid-long-functions-avoid-deep-nesting)
      - [21. Avoid initialization dependencies across compilation units.](#21-avoid-initialization-dependencies-across-compilation-units)
      - [22. Minimize definitional dependencies. Avoid cyclic dependencies.](#22-minimize-definitional-dependencies-avoid-cyclic-dependencies)
      - [23. Make header files self-sufficient.](#23-make-header-files-self-sufficient)
      - [24. Always write internal #include guards. Never write external #include guards.](#24-always-write-internal-include-guards-never-write-external-include-guards)
      - [25. Take parameters appropriately by value, (smart) pointer, or reference.](#25-take-parameters-appropriately-by-value-smart-pointer-or-reference)
      - [26. Preserve natural semantics for overloaded operators.](#26-preserve-natural-semantics-for-overloaded-operators)
      - [27. Prefer the canonical forms of arithmetic and assignment operators.](#27-prefer-the-canonical-forms-of-arithmetic-and-assignment-operators)
      - [28. Prefer the canonical form of ++ and --. Prefer calling the prefix forms.](#28-prefer-the-canonical-form-of--and----prefer-calling-the-prefix-forms)
      - [29. Consider overloading to avoid implicit type conversions.](#29-consider-overloading-to-avoid-implicit-type-conversions)
      - [30. Avoid overloading &&, || or comma (,).](#30-avoid-overloading---or-comma-)
      - [31. Don't write code that depends on the order of evaluation of function arguments.](#31-dont-write-code-that-depends-on-the-order-of-evaluation-of-function-arguments)
      - [32. Be clear what kind of class you're writing.](#32-be-clear-what-kind-of-class-youre-writing)
      - [33. Prefer minimal class to monolithic classes.](#33-prefer-minimal-class-to-monolithic-classes)
      - [34. Prefer composition to inheritance.](#34-prefer-composition-to-inheritance)
      - [35. Avoid inheriting from classes that were not designed to be base classes.](#35-avoid-inheriting-from-classes-that-were-not-designed-to-be-base-classes)
      - [36. Prefer providing abstract interfaces.](#36-prefer-providing-abstract-interfaces)
      - [37. Public inheritance is substitutability. Inherit, not to reuse, but to be reused.](#37-public-inheritance-is-substitutability-inherit-not-to-reuse-but-to-be-reused)
      - [38. Practics safe overriding.](#38-practics-safe-overriding)
      - [39. Consider making virtual functions nonpublic, and public functions nonvirtual.](#39-consider-making-virtual-functions-nonpublic-and-public-functions-nonvirtual)
      - [40. Avoid providing implicit conversions.](#40-avoid-providing-implicit-conversions)
      - [41. Make data members private, except in behaviourless aggregates (C-style structs).](#41-make-data-members-private-except-in-behaviourless-aggregates-c-style-structs)
      - [42. Don't give away your internals.](#42-dont-give-away-your-internals)
      - [43. Pimpl judiciously.](#43-pimpl-judiciously)
      - [44. Prefer writing nonmember nonfriend functions.](#44-prefer-writing-nonmember-nonfriend-functions)
      - [45. Always provide new and delete together.](#45-always-provide-new-and-delete-together)
      - [46. If you provide any class-specific new, provide all of the standard forms (plain, in-place, and nothrow).](#46-if-you-provide-any-class-specific-new-provide-all-of-the-standard-forms-plain-in-place-and-nothrow)
      - [47. Define and initialize member variables in the same order.](#47-define-and-initialize-member-variables-in-the-same-order)
      - [48. Prefer initialization to assignment in constructors.](#48-prefer-initialization-to-assignment-in-constructors)
      - [49. Avoid calling virtual functions in constructors and destructors.](#49-avoid-calling-virtual-functions-in-constructors-and-destructors)
      - [50. Make base class destructors public and virtual, or protected and nonvirtual.](#50-make-base-class-destructors-public-and-virtual-or-protected-and-nonvirtual)
      - [51. Destructors, deallocation, and swap never fail.](#51-destructors-deallocation-and-swap-never-fail)
      - [52. Copy and destroy consistently.](#52-copy-and-destroy-consistently)
      - [53. Explicitly enable or disable copying.](#53-explicitly-enable-or-disable-copying)
      - [54. Avoid slicing. Consider Clone instead of copying in base classes.](#54-avoid-slicing-consider-clone-instead-of-copying-in-base-classes)
      - [55. Prefer the canonical form of assignment.](#55-prefer-the-canonical-form-of-assignment)
      - [56. Whenever it makes sense, provide a no-fail swap (and provide it correctly).](#56-whenever-it-makes-sense-provide-a-no-fail-swap-and-provide-it-correctly)
      - [57. Keep a type and its nonmember function interface in the same namespace.](#57-keep-a-type-and-its-nonmember-function-interface-in-the-same-namespace)
      - [58. Keep types and functions in separate namespaces unless they're specifically intended to work together.](#58-keep-types-and-functions-in-separate-namespaces-unless-theyre-specifically-intended-to-work-together)
      - [59. Don't write namespace usings in a header file or before a #include.](#59-dont-write-namespace-usings-in-a-header-file-or-before-a-include)
      - [60. Avoid allocating and deallocating memory in different modules.](#60-avoid-allocating-and-deallocating-memory-in-different-modules)
      - [61. Don't define entities with linkage in a header file.](#61-dont-define-entities-with-linkage-in-a-header-file)
      - [62. Don't allow exceptions to propagate across module boundaries.](#62-dont-allow-exceptions-to-propagate-across-module-boundaries)
      - [63. Use sufficiently portable types in a module's interface.](#63-use-sufficiently-portable-types-in-a-modules-interface)
      - [64. Blend static and dynamic polymorphism judiciously.](#64-blend-static-and-dynamic-polymorphism-judiciously)
      - [65. Customize intentionally and explicitly.](#65-customize-intentionally-and-explicitly)
      - [66. Don't specialize function templates.](#66-dont-specialize-function-templates)
      - [67. Don't write unintentionally nongeneric code.](#67-dont-write-unintentionally-nongeneric-code)
      - [68. Assert liberally to document internal assumptions and invariants.](#68-assert-liberally-to-document-internal-assumptions-and-invariants)
      - [69. Establish a rational error handling policy, and follow it strictly.](#69-establish-a-rational-error-handling-policy-and-follow-it-strictly)
      - [70. Distinguish between errors and non-errors.](#70-distinguish-between-errors-and-non-errors)
      - [71. Design and write error-safe code.](#71-design-and-write-error-safe-code)
      - [72. Prefer to use exceptions to report errors.](#72-prefer-to-use-exceptions-to-report-errors)
      - [73. Throw by value, catch by reference.](#73-throw-by-value-catch-by-reference)
      - [74. Report, handle, and translate errors appropriately.](#74-report-handle-and-translate-errors-appropriately)
      - [75. Avoid exception specifications.](#75-avoid-exception-specifications)
      - [76. Use vector by default. Otherwise, choose an appropriate container.](#76-use-vector-by-default-otherwise-choose-an-appropriate-container)
      - [77. Use vector and string instead of arrays.](#77-use-vector-and-string-instead-of-arrays)
      - [78. Use vector (and string::c_str) to exchange data with non-C++ APIs.](#78-use-vector-and-stringcstr-to-exchange-data-with-non-c-apis)
      - [79. Store only values and smart pointers in containers.](#79-store-only-values-and-smart-pointers-in-containers)
      - [80. Prefer push_back to other ways of expanding a sequence.](#80-prefer-pushback-to-other-ways-of-expanding-a-sequence)
      - [81. Prefer range operations to single-element operations.](#81-prefer-range-operations-to-single-element-operations)
      - [82. Use the accepted idioms to really shrink capacity and really erase elements.](#82-use-the-accepted-idioms-to-really-shrink-capacity-and-really-erase-elements)
      - [83. Use a checked STL implementation.](#83-use-a-checked-stl-implementation)
      - [84. Prefer algorithm calls to handwritten loops.](#84-prefer-algorithm-calls-to-handwritten-loops)
      - [85. Use the right STL search algorithm.](#85-use-the-right-stl-search-algorithm)
      - [86. Use the right STL sort algorithm.](#86-use-the-right-stl-sort-algorithm)
      - [87. Make predicates pure functions](#87-make-predicates-pure-functions)
      - [88. Prefer function objects over functions as algorithm and comparer arguments.](#88-prefer-function-objects-over-functions-as-algorithm-and-comparer-arguments)
      - [89. Write function objects correctly.](#89-write-function-objects-correctly)
      - [90. Avoid type switching; perfer polymorphism.](#90-avoid-type-switching-perfer-polymorphism)
      - [91. Rely on types, not on representations.](#91-rely-on-types-not-on-representations)
      - [92. Avoid using reinterpret_cast.](#92-avoid-using-reinterpretcast)
      - [93. Avoid using static_cast on pointers.](#93-avoid-using-staticcast-on-pointers)
      - [94. Avoid casting away const.](#94-avoid-casting-away-const)
      - [95. Don't use C-style casts.](#95-dont-use-c-style-casts)
      - [96. Don't memcpy or memcmp non-PODS.](#96-dont-memcpy-or-memcmp-non-pods)
      - [97. Don't use unions to reinterpret representation.](#97-dont-use-unions-to-reinterpret-representation)
      - [98. Don't use varargs (ellipsis).](#98-dont-use-varargs-ellipsis)
      - [99. Don't use invalid objects. Don't use unsafe functions.](#99-dont-use-invalid-objects-dont-use-unsafe-functions)
      - [100. Don't treat arrays polymorphically.](#100-dont-treat-arrays-polymorphically)

<!-- markdown-toc end -->


## Organizational and Policy Issues

### 0. Don't sweat the small stuff.

refer to pg. 2
- Say only what needs saying: Don't enforce personal tastes or obsolete practices.

### 1. Compile cleanly at high warning levels.

refer to pg. 4
- Use your compiler's highest warning level. Require clean (warning-free) builds.

### 2. Use an automated build system.

refer to pg. 7.
- One-action build.
- Ideal build produces no noise and only one log message: "Build succeeded."
- Two build modes: Incremental and full.
- Different build configurations (debug, release, etc.)

### 3. Use a version control system.

refer to pg. 8

### 4. Invest in code reviews.

refer to pg. 9
- Re-view code.

## Design Style

### 5. Give one entity one cohesive responsibility.

refer to pg. 12
Prefer to give each entity (variable, class, function, namespace, module,
library) one well-defined responsibility.

### 6. Correctness, simplicity, and clarity come first.

refer to pg. 13
- KISS (Keep It Simple Software).
- Correct is better than complex.
- Clear is better than cute.
- Safe is better than insecure.
- "Write programs for people first, computers second" - Steve McConnell

### 7. Know when and how to code for scalability.

refer to pg. 14

### 8. Don't optimize prematurely.

refer to pg. 16
- It is far, far easier to make a correct program fast than it is to make a fast
  program correct.
- Profilers are excellent at telling you, by function hit count, what functions
  you should have marked inline byt didn't; profilers are terrible at telling
  you what functions you did mark inline but shouldn't have.

### 9. Don't pessimize prematurely.

refer to pg. 18
- Use pass-by-reference in preference to pass-by-value when appropriate.
- Use prefix ++ instead of postfix ++ when appropriate.
- Use initializer list instead of assigning inside constructor when appropriate.

### 10. Minimize global and shared data

refer to pg. 19

### 11. Hide information

refer to pg. 20
- Don't tell: Don't expose internal information from an entity that provides an
  abstraction.

### 12. Know when and how to code for concurrency.

refer to pg. 21

### 13. Ensure resources are owned by objects. Use explicit RAII and smart pointers.

refer to pg. 24
- When allocating a raw resource, immediately pass it to an owning object.
- Never allocate more than one resource in a single statement.
- Rather than explicitly calling init/shutdown, do in constructor
- Smart pointers can be overused. Raw pointers are fine in code where the
  pointed-to object is visible only to a restricted quantity of code.

## Coding Style

"One man's constant is another man's variable" - Alan Perlis

### 14. Prefer compile- and link-time errors to run-time errors.

refer to pg. 28

### 15. Use const proactively.

refer to pg. 30

### 16. Avoid macros

refer to pg 32

### 17. Avoid magic numbers

refer to pg. 34

### 18. Declare variables as locally as possible

refer to pg. 35

### 19. Always initialize variables

refer to pg. 36

### 20. Avoid long functions. Avoid deep nesting.

refer to pg. 38

### 21. Avoid initialization dependencies across compilation units.

refer to pg. 39

### 22. Minimize definitional dependencies. Avoid cyclic dependencies.

refer to pg. 40

### 23. Make header files self-sufficient.

refer to pg. 42

### 24. Always write internal #include guards. Never write external #include guards.

refer to pg. 43

### 25. Take parameters appropriately by value, (smart) pointer, or reference.

refer to pg. 46
- Always const-qualify all pointers or references to input-only parameters
- Prefer taking inputs of primitive types and value objects that are cheap to copy by value
- Prefer taking inputs of other user-defined types by reference to const
- Consider pass-by-value instead of reference if the function requires a copy of its argument.
This is conceptually identical to taking a reference to const plus doing a copy,
and it can help the compiler to better optimize away temporaries.

### 26. Preserve natural semantics for overloaded operators. 

refer to pg. 47

### 27. Prefer the canonical forms of arithmetic and assignment operators.

refer to pg. 48

### 28. Prefer the canonical form of ++ and --. Prefer calling the prefix forms.

refer to pg. 50

### 29. Consider overloading to avoid implicit type conversions.

refer to pg. 51

### 30. Avoid overloading &&, || or comma (,).

refer to pg. 52

### 31. Don't write code that depends on the order of evaluation of function arguments.

refer to pg. 54

### 32. Be clear what kind of class you're writing.

refer to pg. 56
- value classes
- base classes
- traits classes
- policy classes
- exception classes

### 33. Prefer minimal class to monolithic classes.

refer to pg. 57
- Divide and conquer: Small classes are easier to write, get right, test and use.

### 34. Prefer composition to inheritance.

refer to pg. 58
- Inheritance is the second-tightes coupling relationship in C++, second only to friendship.

### 35. Avoid inheriting from classes that were not designed to be base classes.

refer to pg. 60

### 36. Prefer providing abstract interfaces.

refer to pg. 62
- Abstract interfaces help you focus on getting an abstraction right without
muddling it with implementation or state management details.
- Prefer to follow the Dependency Inversion Principle (DIP):
  - High-level modules should not depend upon low-level modules. Rather, both should depend upon abstractions
  - Abstractions should not depend upon details. Rather, details should depend upon abstractions.

### 37. Public inheritance is substitutability. Inherit, not to reuse, but to be reused.

refer to pg. 64
- Public inheritance must always model "is-a" according to Liskov Substitution Principle.
- Inherit publicly in order to be reused by existing code that already uses base objects polymorphically.

### 38. Practics safe overriding.

refer to pg. 66

### 39. Consider making virtual functions nonpublic, and public functions nonvirtual. 

refer to pg. 68

### 40. Avoid providing implicit conversions.

refer to pg. 70

### 41. Make data members private, except in behaviourless aggregates (C-style structs).

refer to pg. 72

### 42. Don't give away your internals. 

refer to pg. 74

### 43. Pimpl judiciously.

refer to pg. 76

### 44. Prefer writing nonmember nonfriend functions.

refer to pg. 79

### 45. Always provide new and delete together.

refer to pg. 80

### 46. If you provide any class-specific new, provide all of the standard forms (plain, in-place, and nothrow).

refer to pg. 82

### 47. Define and initialize member variables in the same order.

refer to pg. 86

### 48. Prefer initialization to assignment in constructors.

refer to pg. 87

### 49. Avoid calling virtual functions in constructors and destructors.

refer to pg. 88

### 50. Make base class destructors public and virtual, or protected and nonvirtual.

refer to pg. 90

### 51. Destructors, deallocation, and swap never fail.

refer to pg. 92

### 52. Copy and destroy consistently.

refer to pg. 94

### 53. Explicitly enable or disable copying.

refer to pg. 95

### 54. Avoid slicing. Consider Clone instead of copying in base classes.

refer to pg. 96

### 55. Prefer the canonical form of assignment.

refer to pg. 99

### 56. Whenever it makes sense, provide a no-fail swap (and provide it correctly).

refer to pg. 100

### 57. Keep a type and its nonmember function interface in the same namespace.

refer to pg. 104

### 58. Keep types and functions in separate namespaces unless they're specifically intended to work together.

refer to pg. 106

### 59. Don't write namespace usings in a header file or before a #include.

refer to pg. 108

### 60. Avoid allocating and deallocating memory in different modules.

refer to pg. 111

### 61. Don't define entities with linkage in a header file.

refer to pg. 112

### 62. Don't allow exceptions to propagate across module boundaries.

refer to pg. 114

### 63. Use sufficiently portable types in a module's interface.

refer to pg. 116

### 64. Blend static and dynamic polymorphism judiciously.

refer to pg. 120

### 65. Customize intentionally and explicitly.

refer to pg. 122

### 66. Don't specialize function templates.

refer to pg. 126

### 67. Don't write unintentionally nongeneric code.

refer to pg. 128

### 68. Assert liberally to document internal assumptions and invariants.

refer to pg. 130
- Use assert lots!
- Never write expressions with side effects in assert statements.

### 69. Establish a rational error handling policy, and follow it strictly.

refer to pg. 132
- Identification
- Severity
- Detection
- Propagation
- Handling
- Reporting

### 70. Distinguish between errors and non-errors.

refer to pg. 134
- Within a function f, a failure is an error if and only if it violates one of f's preconditions or prevents f from meeting any of its callees' preconditions, achieving any of f's own postconditions, or reestablishing any invariant that f shares responsibility for maintaining.

### 71. Design and write error-safe code.

refer to pg. 137
- Prefer to additionally guarentee that the final state is either the original state (if there was an error) or the intended target state (if there was no error).

### 72. Prefer to use exceptions to report errors.

refer to pg. 140

### 73. Throw by value, catch by reference.

refer to pg. 144

### 74. Report, handle, and translate errors appropriately.

refer to pg. 145
- Handle or translate each error at the nearest possible level that can do so correctly.

### 75. Avoid exception specifications.

refer to pg. 146

### 76. Use vector by default. Otherwise, choose an appropriate container.

refer to pg. 150

### 77. Use vector and string instead of arrays. 

refer to pg. 152

### 78. Use vector (and string::c_str) to exchange data with non-C++ APIs.

refer to pg. 153

### 79. Store only values and smart pointers in containers.

refer to pg. 154

### 80. Prefer push_back to other ways of expanding a sequence.

refer to pg. 155

### 81. Prefer range operations to single-element operations.

refer to pg. 156
- use range operations (e.g., the form of insert that takes a pair of iterators) instead of a series of calls to the single-element form of the operation.

### 82. Use the accepted idioms to really shrink capacity and really erase elements.

refer to pg. 157

### 83. Use a checked STL implementation.

refer to pg. 160

### 84. Prefer algorithm calls to handwritten loops.

refer to pg. 162

### 85. Use the right STL search algorithm.

refer to pg. 165

### 86. Use the right STL sort algorithm.

refer to pg. 166

### 87. Make predicates pure functions 

refer to pg. 168
- A predicate is a function object that returns a yes/no answer (typically a bool).
- A function is pure in the mathematical sense if its result depends only on its arguments

### 88. Prefer function objects over functions as algorithm and comparer arguments.

refer to pg. 170

### 89. Write function objects correctly.

refer to pg. 172

### 90. Avoid type switching; perfer polymorphism.

refer to pg. 174
- Avoid switching on the type of an object to customize behaviour.
- Use templates and virtual functions to let types (not their calling code) decide their behaviour.
- Messaging system :S

### 91. Rely on types, not on representations.

refer to pg. 176

### 92. Avoid using reinterpret_cast.

refer to pg. 177

### 93. Avoid using static_cast on pointers.

refer to pg. 178

### 94. Avoid casting away const.

refer to pg. 179

### 95. Don't use C-style casts.

refer to pg. 180

### 96. Don't memcpy or memcmp non-PODS.

refer to pg. 182

### 97. Don't use unions to reinterpret representation. 

refer to pg. 183

### 98. Don't use varargs (ellipsis).

refer to pg. 184

### 99. Don't use invalid objects. Don't use unsafe functions.

refer to pg. 185

### 100. Don't treat arrays polymorphically.

refer to pg. 186

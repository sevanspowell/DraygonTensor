# Codestyle Guidelines

*The following guidelines were adapted from id software's code style guidelines,
available from [here](http://www.geeks3d.com/downloads/200811/idSoftware-Coding-Conventions-Geeks3D.com.pdf) (as of 22/01/2016)*.

Many of these guidelines are taken from the [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md). Please use them as reference.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-generate-toc again -->
**Table of Contents**

- [Codestyle](#codestyle)
    - [Indentation](#indentation)
    - [Brace positioning](#brace-positioning)
    - [Statement placement](#statement-placement)
    - [General naming](#general-naming)
    - [Function naming](#function-naming)
    - [Variable naming](#variable-naming)
    - [Typedef and struct naming](#typedef-and-struct-naming)
    - [Floating point numbers](#floating-point-numbers)
    - [Classes](#classes)
    - [Interfaces](#interfaces)
    - [Documenting / Commenting](#documenting--commenting)
    - [Enumerators](#enumerators)
    - [Include guards](#include-guards)
    - [Namespaces](#namespaces)
    - [Const usage](#const-usage)
    - [File names](#file-names)
    - [Templates](#templates)
- [Writing Documentation](#writing-documentation)

<!-- markdown-toc end -->


# Codestyle

## Indentation

Each level of indentation should consist of 4 spaces.

Pre-processor directives should not be indented.

Use the .clang-format included with the repository to format your code. This ensures that indentation is consistent across all contributors.

## Brace positioning

Always start braces on a new line (if, else, functions, structures, typedefs,
class definitions, etc.).

```
if (x)
{
}
```

## Statement placement
The else statement of an if-else construct always starts on a new line after the
closing brace of the if statement.

```
if (x)
{
}
else
{
}
```

## General naming

Hash-defined names use all uppercase characters. Multiple words are seperated by an
underscore.

`#define SIDE_FRONT 0`

## Function naming

Both member and non-member function names start with an uppercase.

```
void Function(int a);

class Foo
{
public:
    int GetSize();
}
```

In multi-word function names each other word starts with an uppercase.

```
void ThisFunctionDoesSomething(void);
```

## Variable naming

Variable names start with a lowercase character.

`float x;`

In multi-word variable names the first word starts with a lower case character 
and each successive word starts with an upper case.

`float maxDistanceFromPlane;`

Private class member variables should be prepended with a underscore: "m_".
Otherwise they follow the same naming convention as variables.

```
...
private:
    float *m_speed;
...
```

## Typedef and struct naming

Typedef names use the same naming convention as classes.

```
typedef int Handle;
```

Struct names use the same naming convention as classes.

```
struct Render;
```

Struct member variables do not have to be preceded by an "m_".

```
struct PlatformEvent
{
    uint32_t type;
}
```

## Floating point numbers

All floating point numbers need a decimal point and should have an f appended to
them.

```
float f = 0.4f;
```

Instead of:

```
float g = 1.0;
```

Unless you specifically want a double.

## Classes

Class names start with an uppercase character and each successive word also
starts with an uppercase.

`class Vec3;`

Member variable names are given the prefix “m_”, otherwise they follow normal 
variable naming rules.

```
class MyClass
{
private:
    int     m_myInt;
    float   m_myFloat;
};
```

Class methods have the same naming convention as functions.

```
class Vec3
{
    void NormalizeVector();
};
```

Be wary of listing multiple variables as in the following.

`const float *a, b;`

This code declares one float ptr (a) and one float (b), NOT two float ptrs.

Public, private and protected qualifiers should be placed on the same indentation
level as the class definition.

```
class MyClass
{
public:
    void SomeFunc();
private:
    int   m_myInt;
    float m_myFloat;
protected:
    unsigned int m_stuff;
};
```

Ordering of class variables and methods should be as follows:
* 1. List of friend classes
* 2. public methods
* 3. public variables
* 4. protected methods
* 5. protected variables
* 6. private methods
* 7. private variables

This allows the public interface to be easily found at the beginning of the class.

## Interfaces

Classes that contain one pure virtual function (interfaces) are named according
to the same naming convention as class names, except prepended with an "I".

```class IApplication;```

## Documenting / Commenting

Always use Doxygen-style comments to document public interfaces. Indent
doxygen comments into nice columns. Leave a single space between the comment 
line '*' and the first character of that line's comment.

Make sure to use the @author tag somewhere.

```
/**
 * Description
 *
 * @author  Samuel Evans-Powell
 *
 * @param   paramName   int, param description
 * @return              int, return value description.
 */
int MyFunction(int param);
```

Regular C/C++-style comments may be used internally.

```
int MyFunction(int param)
{
	// Some constant
	const int a;
	
	/* Another 
	 * constant.
	 */
	const int b;
}
```

## Enumerators

Enum names use the same naming convention as classes. Enum values should also use
the same naming convention as classes ([don't use ALL_CAPS for enums](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Renum-caps)). Always prefer enum classes to plain enums
([here's](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Renum-class) why).

```
enum class Contact {
	None,
	Edge,
	ModelVertex,
	TrmVertex
};
```

## Include guards

Use pragmas instead of include guards. The pragma is [widely supported](https://en.wikipedia.org/wiki/Pragma_once).

```
#pragma once
```

## Namespaces 

Use namespaces as much as possible. Namespaces are primarily used to prevent 
name conflicts between our code and external libraries.

```
namespace sea 
{
	class MyClass
	{
	public:
	private:
	};
}
```

`sea::MyClass variableName = sea::MyClass();`

Namespaces can also be used to group classes by function.

```
namespace sea_math
{
	class Mat4
	{
	public:
	private:
	};
}
```

`sea_math::Mat4 matrix = sea_math::Mat4();`

New namespaces are always lowercase and prepended with the string “sea_”.

## Const usage

Use `const` as much as possible.

All class member function should be ['const-correct'](https://isocpp.org/wiki/faq/const-correctness#overview-const).

Always make class methods `const` when they do not modify any class variables.

Return `const` objects unless the general usage of the object involves changing
it's state.

## File names

Each class should be in a seperate source file unless it makes sense to group
several smaller classes.

The file name should be the name of the class.

Each class should have two files, one header file and one source file. Do not
place implementation into the header file unless specifically required (templates, inline functions).

```
class Triangle;

Triangle.h
Triangle.cpp
```

## Templates

If using a single template parameter, the typename should be a single letter: 
'T'. Or a a camel-case name prefixed with a lower case 't'.

```
template <typename T>
class Octree;

template <typename tMessage>
class Channel;
```

If using multiple template parameters, the typenames used should consist of a
camel-case name prefixed with a lower case 't'.

```
template <typename tMessage, typename tChannel>
class Channel;
```

# Writing Documentation

All code should be sufficiently well-documented using Doxygen-style comments. In
addition to this, systems or larger bodies of code should be appropriately
documentated in the [Programmer's Manual](https://github.com/Izabia/DrunkenSailorEngine/blob/master/docs/PROGRAMMERSMANUAL.md), this should include appropriate
usage examples.

# Compiler Construction Lite

If you are reading this, you have just started the course *Compiler
Construction* or *Compilerbouw* at the University of Amsterdam. Good choice! The
course you're about to follow is great fun and you will learn a ton of
interesting things about compilers and programming in general. However, there is
one caveat to the course: the framework provided is a convoluted mess. It
consists of more than 8700 lines of code (14600 when fully generated) spread
over 105 (167) files and is based on a completely unreadable mass of macro's and
automatically generated code.

The official framework is of course not entirely bad, but I was greatly
frustrated by it and felt like I spent more time understanding the framework
than actually developing the compiler. One option is to begin writing a compiler
from scratch entirely, but this takes a great amount of effort, especially when
you are not familiar with the general structure of compilers.

I spent several weeks of the course digging away at the framework and removing
as much junk code as possible. I memorised the structure of the entire compiler
inside and out just to remove as much of the code as possible. The result is the
alternative framework that you are looking at right now. I've stripped it down
to be as basic (or even more basic) than the official compiler framework, as to
not give away any of the answers. This *lite* version of the framework clocks in
at 900 lines of code in 20 files and you have full control over every single
one.

## Goals

 * Get rid of as much unnecessary code from the framework as possible.
 * Make the framework easy to understand using by mostly basic C features.
 * Increase compilation speed and lower complexity by removing generated code.
 * Provide good documentation, something the old framework lacks severely.
 * Make the framework extensible, both for adding new backends and new phases.
 * Improve the debuggability of the code.
 * Release the untapped potential of 20 years of development on the C language.
 * Fix several blatant errors in the official framework.
 * Give you a handle to learn things about compilers, not about old frameworks.

## A word of warning

This framework is in no way endorsed or approved by the people that teach the
Compiler Construction course. The course requirements could change at any time
and you might be out of luck. Before you consider using this framework, please
analyse your choices carefully and remember that neither the author of this
code nor the Compiler Construction staff are responsible for what happens to you
when you use this code. It is only here as a guideline and possible resource.
**Use at your own risk**.

## Cons

Nothing is perfect and neither is this framework. There are some things that the
official framework does much better than this one. If you decide to use this
code, please note the following:

 * In the beginning of the course, you will have to work with an abstract
   syntax tree and submit some files generated from it. This framework currently
   does not allow any automatic generation of documentation, but this might be
   implemented later. You will have to manually create your graph and HTML page.
 * The automatic code generation in the original framework makes it easier to
   create new nodes. Implementing new nodes in this framework will be a little
   more manual labour, but this is mostly a one-time investment of time.

## Additional resources

If you are struggling to understand the big picture behind this framework, a
wiki is available on the Github page for this framework where you can get a
more global documentation about what is happening. You can also refer to the
official framework, because then general structure is somewhat similar. The
reverse is also true: if you are using the official framework, the documentation
of this code might be useful to you.

You can find another Compiler Construction related repository on the Github
account of the creator of this framework. It contains a number of tests for your
compiler and other useful files. This is useful for people using either
framework.

## To do

 * Actually create the wiki
 * Add more documentation to the code

## Contributing

Sharing is caring. If you have any improvements or comments on this code, please
do feel free to email me, send me snail mail, postal dove or simply create an
issue or pull request on Github. One special condition with this code is that
I don't want it to conflict with the rules and goals of the Compilerbouw course
so it is imperative that the framework gives away nothing more than the original
code so that every student has a fair and equal chance.

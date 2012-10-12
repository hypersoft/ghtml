ghtml
=====

*The Graphical HTML Windowing Toolkit*

It features its own lightweight build configuration system using **bash** and **pkg-config** as 
its "special" runtime requirements. The configuration system also makes it extremely 
easy for **C** programmers adept at bash scripting to insert their own build instructions, 
and roll their own implementation defined **make** scripts such as **makefile** or **make.sh**, 
**make.js**, etc, ... All of this is made possible by auto platform detection using 
**config.guess**.

The project is built by compiling the main file, which includes all other parts of the 
C program assembly. The project builds in less than 10 seconds on an intel atom. It is 
lightweight and task oriented yet still in its initial planning stages of release.

What the project does is display Privileged JavaScript HTML User Interfaces, using a 
barebones environment layer provided by the program assembly, and the Gnome project's 
**GObjectIntrospection** importing layer for **Webkit's** **JavaScriptCore**. The application 
supports nearly all things one would expect from a modern browser with a few security 
considerations.

 1. You must load your document from **stdin**, or local **file://** (by standard pathname)

 2. There shall be no extended javascript features provided by the Toolkit for external
     document resources.

 3. The main (local) frame may access and modify any child frame, including provide any
     javascript value or reference defined in its context, to a child at will. Yes this
     means data sharing mastered by the localhost.

 4. Private browsing is enabled for both infrastructure and user security

**ghtml** operates in a discipline based security format. This means all security is 
based on **local content trust**, and **local user access rights**.

Anouncements
============

HOLD UP: Webkit & Gnome's G Series doesn't seem to be up to the task at hand. Its taking serious involvement with runtime code for lack of adequately descriptive documentation contracts. Please use this software at your discretion, it is bleeding edge, and most likely bleeding memory with system calls like exec. Some values needed by scripts may also be garbage collected prematurely as there are no adequate examples or complete refrence points I know of to follow. The current master branch should be useful enough to read environment variables, into items like textboxes who have a set property that is not a refrence to the source, and print things out to the console without issues. We have not had much time to test the seed extensions, but it is likely that would currently be a better option if you are brave enough for the quest. Had GTK+ message dialogs up from within Toolkit the other day... d8d3969bcfa3f63ecd27caa532af0c8033cd61bc

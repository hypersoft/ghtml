ghtml
=====

The Graphical HTML Windowing Toolkit

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

This is a disciplinary security format. All security is based on **local content trust**,
and **local user access rights**.

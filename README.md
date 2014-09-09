CodeParser
==========

This code was made in order to track code and find where you need to implement your own changes on large projects.  I used it on the XServer and Mesa in order to see what functions were being called and when.  Note this does not take into account multiple threads or processors but will still give you a good idea.

There are two main programs from the following files: main.c and maincpp.c.  The first one parses C files and adds a code snippet to each function to print the name of the function and filepath to a specified file based on a shared memory switch.  The second one does the same but for C++ files.  Both execute recursively through a folder and subfolders specified as an argument in the command line.

switchFab.c must be installed and compiled as a library for the project you are researching to use.  You must compile and link to it when you build the project from source.  I was using it on graphics so it was bogging down my system to much so I made a toggle for it in shared memory.  This is what turnOff.c and turnOn.c are for.

There are some problems especially with structures.  It thinks they're a function and adds the code snippet and you get a compile error when building the project you ran the parser on.  I just changed these manually.

Documents function calls sequentially.

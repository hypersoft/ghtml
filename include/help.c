/*
	This file is part of ghtml.

    ghtml is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ghtml is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ghtml.  If not, see <http://www.gnu.org/licenses/>.

*/

char *GHTML_HELP_SECTION_MAIN 				= "--help";
char *GHTML_HELP_SECTION_VERSION 			= "--help-version";
char *GHTML_HELP_SECTION_JS 				= "--help-js";

void ghtml_help(char *section) {

	if (g_str_equal(section, GHTML_HELP_SECTION_VERSION)) {

		puts("\nTitle: ghtml\n");

		  puts("Author: Triston J. Taylor 2012 (pc.wiz.tt@gmail.com)\n");

		  puts("Version: 0.76");
		  puts("License: GPL Version 3");
		  puts("Toolkit: WebKitGTK+ 1.8.1\n");

		return;

	}

	if (g_str_has_prefix(section, GHTML_HELP_SECTION_JS)) {

		puts("\nJavascript Runtime Help - ghtml\n");

		  puts("Run Time Global Functions\n");

		  puts("  getenv(name)         -   get environment variable"); 
		  puts("  setenv(name, value)  -   set environment variable");
		  puts("  delenv(string)       -   delete environment variable");
		  puts("  chdir(name)          -   change current directory\n"); 

		  puts("  exec(string)    -   capture stdout, stderr and status of command");
		  puts("  puts(string)    -   print line terminated data on console");
		  puts("  chomp(string)   -   remove trailing whitespace from string");
		  puts("  quit(number)    -   quit program with exit status\n");

		  puts("-\n");

		  puts("Console Functions\n");

		  puts("  console.exec(string)   -  capture execution status of command");
		  puts("  console.print(string)  -  write raw string to stdout");
		  puts("  console.error(string)  -  write raw string to stderr\n");

		  puts("-\n");

		  puts("Console Properties\n");

		  puts("  console.arguments[]    -  Command line arguments not used by GTK+,");
		  puts("                            ghtml, or seed. Specify -- to signify script");
		  puts("                            arguments.");

		  puts("-\n");

		  puts("Global Objects\n");

		  puts("  Seed          -  \"Gnome Seed\" environment provisions");
		  puts("  imports       -  \"Gnome Seed\" runtime importer\n");

		  puts("-\n");

		  puts("JavaScript Window Mastering\n");

		  puts("ghtml allows JavaScript access to window frames, whenever the calling");
		  puts("frame is from a local source.\n");

		return;

	}

	if (g_str_equal(section, GHTML_HELP_SECTION_MAIN)) {

        printf ("\nUsage: %s [OPTIONS]\n\n", ghtml_app_name);

		printf ("Description: The Graphical HTML Windowing Toolkit\n\n");

		  puts("parse: {ARGUMENTS} -  When the first argument has the prefix: `parse: '");
		  puts("                      ghtml will parse the first argument as a script");
		  puts("                      environment directive.\n");

		printf("--width  {INTEGER} -  Set the window width\n");
		printf("--height {INTEGER} -  Set the window height\n\n");

		  puts("--title  or -t  {STRING}  -  Set the window title (if not supplied");
		  puts("                             document title is used.)\n");

		  puts("--file   or -f  {PATH}    -  Load a file into the web view (if not");
		  puts("                             supplied, file is stdin. content-type");
		  puts("                             is text/html. Specifying this option,)");
		  puts("                             forwards remaining arguments to file.");


		printf("--mime-type {STRING} -  Specify document content-type\n");
		printf("--charset   {STRING} -  Specify document encoding\n\n");

		  puts("--style-sheet or -s {URI} -  specify base motif URI\n");

		  puts("--onload  or -o  {STATEMENTS} -\n");

		  puts("  Run javascript statements onload.\n");

		  puts("  You may issue multiple instances of this directive. These statements");
		  puts("  are executed when the webview has finished loading.\n");

		  puts("--dialog  or -d  -  Use GTK+ Dialog decoration");
		  puts("--explode or -e  -  Activate embedded device quirks\n");

		  puts("--help           -  Show this help screen");
		  puts("--help-version   -  Show version info help screen");
		  puts("--help-js        -  Show javascript help screen\n");

		return;

	}

	fprintf(stderr, 
		"%s: error: unrecognized help screen request: %s\n", 
	ghtml_app_name, section);

}


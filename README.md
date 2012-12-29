#Ghtml

### *The GTK+ HTML Windowing Toolkit*

Ghtml features its own lightweight build configuration system using **bash** and 
**pkg-config** as its "special" runtime requirements.

Ghtml's configuration system also makes it extremely easy for **C** programmers 
adept at bash scripting to insert their own build instructions, and roll their 
own implementation defined **make** scripts such as **makefile** or **make.sh**,
**make.js**, etc, ... All of this is made possible by auto platform detection 
using **config.guess**.

The project is built by compiling the main file, which includes all other parts 
of the C program assembly. The project builds in less than 10 seconds on an 
intel atom. It is lightweight and task oriented yet still in its initial 
planning stages of release.

What the project does is display Privileged JavaScript HTML User Interfaces,
using a barebones environment layer provided by the program assembly, and the 
Gnome project's **GObjectIntrospection** importing layer for **Webkit's** 
**JavaScriptCore**. The application supports nearly all things one would expect 
from a modern browser with a few security considerations.

 1. You must load your document from **stdin**, or local **file://** 
	(by standard pathname)

 2. There shall be no extended javascript features provided by the Toolkit for 
	external document resources.

 3. The main (local) frame may access and modify any child frame, including 
	provide any javascript value or reference defined in its context, to a 
	child at will. Yes this means data sharing mastered by the localhost.

 4. Private browsing is enabled for both infrastructure and user security

**ghtml** operates in a discipline based security format. This means all 
security is based on **local content trust**, and **local user access rights**.

***
Quick Installation Guide
=====

For linux users, installation is straightforward

	  git clone https://github.com/hypersoft/ghtml.git;
	  cd ghtml;
	  ./configure && make && sudo make install;


You may get some errors if you:

* Aren't running a [GNU/Linux](http://www.gnu.org/gnu/linux-and-gnu.html) system
* Don't have Gnome, or one of the required development packages installed
  

*<note>Most errors can be fixed, they just weren't planned for during development</note>*

***
Anouncements
============

###New markdown viewer

**Installation**

  	sudo apps/install mrkdnv

**Description**

  	Graphical Markdown Document Rendering Engine

**Synopsis**

	mrkdnv [OPTIONS] [TITLE] [PIXDIMENSIONS] [FILENAME]
	
**Invocation**

	OPTIONS
	
		--style		-s		Head stylesheet.
		--engine	-e		Head script.
		
		--theme		-t		Head theme script. Themes are located in
							/usr/share/mrkdnv/themes.
		
		--					Break option switch
		
	TITLE		
		
		String to set for window content identity. If not supplied
		the title is "Markdown Viewer".

	PIXDIMENSIONS
		
		Dual integer window pixel dimension specifier of the format WxH.
		The default value is 720x480.
	
	FILENAME
		
		Document to render. Default value is standard input.

	
**Example**

	mrkdnv "README" < README.md

>	<hr>

###New Javascript Additions <note>(unstable)</note>

**Window Centering**

	window.center()

<note>For window centering to work during load, you need to have the window, size set from the command line.</note>

>	<hr>

###Updated app installation methods

	USAGE: apps/install group [-e] [title];
	  
	DESCRIPTION: Install the application by its group name and
	title to environment variable INSTALLDIR or /usr/bin
	  
	group: The name of the path in ./apps
	title: The name of the file in: ./apps/group
	  
	If option -e is supplied, execute the title, with remaining arguments,
	instead of copying it. If title is not supplied with -e list group executables
	  
	If title is not supplied, execute ./apps/group/"@group"
	if that program is not present, try to install
	./apps/group/group and if that fails show a listing
	of all valid resources belonging to group
	  
	if title is -- all following arguments are passed to
	./apps/group/"@group"
	  
	You must have sufficient privileges to perform this operation.

<hr>
#####Ready and waiting

<markup theme='hypersoft'>

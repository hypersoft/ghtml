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

const char * GHTML_JS_INVALID_PARAMS = "invalid parameters"; 

#include "js/native.c"

#include "js/chomp.c"
#include "js/exec.c"
#include "js/quit.c"
#include "js/puts.c"
#include "js/chdir.c"

#include "js/getenv.c"
#include "js/setenv.c"
#include "js/delenv.c"

#include "js/console/exec.c"
#include "js/console/error.c"
#include "js/console/print.c"
#include "js/console/arguments.c"

#include "js/window/hide.c"
#include "js/window/show.c"
#include "js/window/icon.c"
//#include "js/window/center.c"

#include "js/init.c"


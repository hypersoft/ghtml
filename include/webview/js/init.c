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

void ghtml_webview_js_init(void *thisContext, void *thisFrame) {

	/*  This should only run once for the main context. */

	if (! ghtml_webview_main_global_object ) {

		ghtml_webview_seed = seed_init_with_context (NULL, NULL, thisContext);

		ghtml_webview_main_frame = thisFrame;

		ghtml_webview_main_context = thisContext;

		ghtml_webview_main_global_object = JSContextGetGlobalObject(thisContext);

		seed_simple_evaluate(thisContext, "imports.gi.Gtk; imports.gi.Gst; imports.gi.Gio;", NULL);

		seed_simple_evaluate(thisContext, "Seed.include(\"/usr/share/ghtml/lib/jquery.js\");", NULL);
		seed_simple_evaluate(thisContext, "Seed.include(\"/usr/share/ghtml/lib/xtag.js\");", NULL);
		seed_simple_evaluate(thisContext, "Seed.include(\"/usr/share/ghtml/lib/sigma.js\");", NULL);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "quit",
			ghtml_webview_js_quit, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "exec",
			ghtml_webview_js_exec, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "chdir",
			ghtml_webview_js_chdir, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "puts",
			ghtml_webview_js_puts, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "chomp",
			ghtml_webview_js_chomp, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "setenv",
			ghtml_webview_js_setenv, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "delenv",
			ghtml_webview_js_delenv, NULL
		);

		ghtml_webview_js_native( thisContext, ghtml_webview_main_global_object, "getenv",
			ghtml_webview_js_getenv, NULL
		);

		/* CONSOLE BUILTINS */

		void *jstemp = 0;

	 	void *console = (void *) JSObjectGetProperty(
			thisContext, ghtml_webview_main_global_object, 
			jstemp = JSStringCreateWithUTF8CString("console"), NULL
		);  JSStringRelease(jstemp);

		ghtml_webview_js_native( thisContext, console, "error",
			ghtml_webview_js_console_error, NULL
		);

		ghtml_webview_js_native( thisContext, console, "print", 
			ghtml_webview_js_console_print, NULL
		);

		ghtml_webview_js_native( thisContext, console, "exec", 
			ghtml_webview_js_console_exec, NULL
		);

		ghtml_webview_js_console_arguments( thisContext, console,
			ghtml_app_argc, ghtml_app_argv
		);

	 	void *window = (void *) JSObjectGetProperty(
			thisContext, ghtml_webview_main_global_object, 
			jstemp = JSStringCreateWithUTF8CString("window"), NULL
		);  JSStringRelease(jstemp);

		ghtml_webview_js_native( thisContext, window, "hide", 
			ghtml_webview_js_window_hide, NULL
		);

	}

}


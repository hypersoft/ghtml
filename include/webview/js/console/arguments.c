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

void ghtml_webview_js_console_arguments (void * environment, void * console, int argc, char * argv[]) {

	void *jsArgData[argc + 1], *jsTemp;
	void **jsUsrArgs = (jsArgData + 1);

	int i = 0; 

	// Set the "application path" argument.
	jsArgData[0] = (void *) JSValueMakeString(
		environment, jsTemp = JSStringCreateWithUTF8CString(ghtml_app_file)
	);  JSStringRelease(jsTemp);

	// Set remaining arguments
	for (i = 0; i < argc ; i++) {
		jsUsrArgs[i] = (void *) JSValueMakeString(
			environment, jsTemp = JSStringCreateWithUTF8CString(argv[i])
		);  JSStringRelease(jsTemp);
	}

	void *jsarguments = seed_make_array(
		environment, jsArgData, argc + 1, NULL
	);

	JSObjectSetProperty(
		environment, console, jsTemp = JSStringCreateWithUTF8CString("arguments"),
		jsarguments, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete,
		NULL
	);  JSStringRelease(jsTemp);

}


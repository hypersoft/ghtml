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

void ghtml_webview_js_native(void *context, void *object, const char * name, void * function, void * exception) {

	JSStringRef fname = JSStringCreateWithUTF8CString(name);
	JSObjectSetProperty(context, object, fname,
		seed_make_function(context, function, name),
		kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete, exception
	);  JSStringRelease(fname);
}


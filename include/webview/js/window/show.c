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

SeedValue ghtml_webview_js_window_show (SeedContext ctx, SeedObject function, SeedObject thisObject, size_t argumentCount, SeedValue arguments[], SeedException * exception) {

	if (argumentCount) {
		seed_make_exception (ctx, exception, GHTML_JS_INVALID_PARAMS,
			"window.show expected 0 arguments, got %zd", argumentCount
		);  return seed_make_null (ctx);
	}

	gtk_widget_show_all(ghtml_window);
 
	return seed_make_null (ctx);

}


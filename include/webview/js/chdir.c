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

SeedValue ghtml_webview_js_chdir (SeedContext ctx, SeedObject function, SeedObject thisObject, size_t argumentCount, SeedValue arguments[], SeedException * exception) {

	if (argumentCount != 1) {
		seed_make_exception (ctx, exception, GHTML_JS_INVALID_PARAMS,
			"chdir expected 1 argument, got %zd", argumentCount
		);  return seed_make_null (ctx);
	}

	gchar * val = seed_value_to_string(ctx, arguments[0], exception);
	SeedValue result = (SeedValue) JSValueMakeNumber(ctx, g_chdir(val));

	g_free(val);

	return result;
}


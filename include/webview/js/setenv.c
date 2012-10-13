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

JSValueRef ghtml_webview_js_setenv (SeedContext ctx, SeedObject function, SeedObject thisObject, size_t argumentCount, SeedValue arguments[], SeedException * exception) {

	if (argumentCount < 2 || argumentCount > 3) {
		seed_make_exception (ctx, exception, GHTML_JS_INVALID_PARAMS,
			"setenv expects 2 or 3 arguments, got %zd", argumentCount
		);  return seed_make_null (ctx);
	}

	gchar * var = seed_value_to_string(ctx, arguments[0], exception);
	gchar * val = seed_value_to_string(ctx, arguments[1], exception);

	bool overwrite = false;

	if (argumentCount == 3) {
		overwrite = seed_value_to_boolean(ctx, arguments[2], exception);
	}

	// TODO: use third argument as (boolean) overwrite here
	JSValueRef result = JSValueMakeBoolean(ctx, g_setenv(var, val, overwrite));

	g_free(var); g_free(val);

	return result;

}


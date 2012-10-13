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

SeedValue ghtml_webview_js_console_exec (SeedContext ctx, SeedObject function, SeedObject thisObject, size_t argumentCount, SeedValue arguments[], SeedException * exception) {

	if (argumentCount != 1) {
		seed_make_exception (ctx, exception, GHTML_JS_INVALID_PARAMS,
			"console.exec expected 1 argument, got %zd", argumentCount
		);  return seed_make_null (ctx);
	}

	char ** nargv;
	gchar * usrcommand = seed_value_to_string(ctx, arguments[0], exception);

	int nargc, child_status;

	SeedValue exec = (SeedValue) JSValueMakeUndefined(ctx);

	if (g_shell_parse_argv(usrcommand, &nargc, &nargv, NULL)) {

		if (g_spawn_sync(NULL, nargv, NULL, G_SPAWN_LEAVE_DESCRIPTORS_OPEN | G_SPAWN_SEARCH_PATH | G_SPAWN_CHILD_INHERITS_STDIN, NULL, NULL, NULL, NULL, &child_status, NULL)) {

			exec = (JSObjectRef) JSValueMakeNumber(ctx, WEXITSTATUS(child_status));
		}

		g_strfreev(nargv);

	}

	g_free(usrcommand);

	return exec;

}


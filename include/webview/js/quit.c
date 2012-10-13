
SeedValue ghtml_webview_quit (SeedContext ctx, SeedObject function, SeedObject thisObject, gsize argumentCount, const SeedValue arguments[], SeedException * exception) {

	if (argumentCount == 1) {

		exit(seed_value_to_int(ctx, arguments[0], exception));

	} else if (argumentCount > 1) {

		seed_make_exception(
			ctx, exception, "ArgumentError",
			"quit expected 1 argument, got %zd",
			argumentCount
		);

	}

	exit (EXIT_SUCCESS);

}


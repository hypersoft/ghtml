
JSValueRef ghtml_webview_getenv (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);

	usrstring = (void*) g_getenv(usrstring);
	if (usrstring == NULL) return JSValueMakeUndefined(ctx);
	return JSValueMakeString(ctx,JSStringCreateWithUTF8CString(usrstring));

}



JSValueRef ghtml_webview_setenv (JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
	void *usrstring;
	void *usrstring2;
	size_t length;
	void * jsstr = JSValueToStringCopy(ctx, arguments[0], NULL);
	void * jsstr2 = JSValueToStringCopy(ctx, arguments[1], NULL);
	length = JSStringGetMaximumUTF8CStringSize (jsstr);
	usrstring = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr, usrstring, length);
	JSStringRelease(jsstr);
	length = JSStringGetMaximumUTF8CStringSize (jsstr2);
	usrstring2 = g_alloca (length * sizeof (gchar));
	JSStringGetUTF8CString (jsstr2, usrstring2, length);
	JSStringRelease(jsstr2);

	return JSValueMakeBoolean(ctx, g_setenv(usrstring, usrstring2, TRUE));

}


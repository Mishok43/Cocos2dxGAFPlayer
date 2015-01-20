#include "jsb_gaf_manual.h"
#include "cocos2d_specifics.hpp"
#include "GAF_JS.h"
#include "jsb_gaf.hpp"

/*
jsval anonEvaluate(JSContext *cx, JSObject *thisObj, const char* string) {
    jsval out;
    JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
        if (JS_EvaluateScript(cx, thisObj, string, strlen(string), "(string)", 1, &out) == true) {
        return out;
        }
    return JSVAL_VOID;
}
*/
void register_gaf(JSContext* cx, JSObject* global)
{
    register_all_gaf(cx, global);
    JSObject* tmpObj = JSVAL_TO_OBJECT(anonEvaluate(cx, global, "(function () { return gaf.Object; })()"));
    JS_DefineFunction(cx, tmpObj, "setSequenceDelegate", js_gaf_GAFObject_setSequenceDelegate, 1, JSPROP_READONLY | JSPROP_PERMANENT);
}


bool js_gaf_GAFObject_setSequenceDelegate(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    bool ok = true;
    JSObject *obj = JS_THIS_OBJECT(cx, vp);
    js_proxy_t *proxy = jsb_get_js_proxy(obj);
    gaf::GAFObject* cobj = (gaf::GAFObject *)(proxy ? proxy->ptr : NULL);
    JSB_PRECONDITION2( cobj, cx, false, "js_gaf_GAFObject_setSequenceDelegate : Invalid Native Object");
    if (argc == 1) {
        std::function<void (gaf::GAFObject *, const std::basic_string<char> &)> arg0;
        do {
            if(JS_TypeOfValue(cx, argv[0]) == JSTYPE_FUNCTION)
            {
                std::shared_ptr<JSFunctionWrapper> func(new JSFunctionWrapper(cx, JS_THIS_OBJECT(cx, vp), argv[0]));
                auto lambda = [=](gaf::GAFObject* larg0, const std::basic_string<char> & larg1) -> void {
                    jsval largv[2];
                    do {
                    if (larg0) {
                        js_proxy_t *jsProxy = js_get_or_create_proxy<gaf::GAFObject>(cx, (gaf::GAFObject*)larg0);
                        largv[0] = OBJECT_TO_JSVAL(jsProxy->obj);
                    } else {
                        largv[0] = JSVAL_NULL;
                    }
                } while (0);
                 do {
                    largv[1] = std_string_to_jsval(cx, larg1);
                } while (0);
                    jsval rval;
                    bool ok = func->invoke(2, &largv[0], rval);
                    if (!ok && JS_IsExceptionPending(cx)) {
                        JS_ReportPendingException(cx);
                    }
                };
                arg0 = lambda;
            }
            else
            {
                arg0 = nullptr;
            }
        } while(0)
        ;
        JSB_PRECONDITION2(ok, cx, false, "js_gaf_GAFObject_setSequenceDelegate : Error processing arguments");
        cobj->setSequenceDelegate(arg0);
        JS_SET_RVAL(cx, vp, JSVAL_VOID);
        return true;
    }

    JS_ReportError(cx, "js_gaf_GAFObject_setSequenceDelegate : wrong number of arguments: %d, was expecting %d", argc, 1);
    return false;
}
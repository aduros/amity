LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := mozjs

LOCAL_CPPFLAGS := \
    -include $(LOCAL_PATH)/android-build/js/js-confdefs.h \
    -DNDEBUG -DTRIMMED -DMOZILLA_CLIENT -DEXPORT_JS_API -DUSE_SYSTEM_MALLOC=1 -DENABLE_ASSEMBLER=1 -DENABLE_JIT=1

LOCAL_SRC_FILES := \
    mozilla-central/js/src/jsanalyze.cpp \
    mozilla-central/js/src/jsapi.cpp \
    mozilla-central/js/src/jsarena.cpp \
    mozilla-central/js/src/jsarray.cpp \
    mozilla-central/js/src/jsatom.cpp \
    mozilla-central/js/src/jsbool.cpp \
    mozilla-central/js/src/jsbuiltins.cpp \
    mozilla-central/js/src/jsclone.cpp \
    mozilla-central/js/src/jscntxt.cpp \
    mozilla-central/js/src/jscompartment.cpp \
    mozilla-central/js/src/jscpucfg.cpp \
    mozilla-central/js/src/jsdate.cpp \
    mozilla-central/js/src/jsdbgapi.cpp \
    mozilla-central/js/src/jsdhash.cpp \
    mozilla-central/js/src/jsdtoa.cpp \
    mozilla-central/js/src/jsemit.cpp \
    mozilla-central/js/src/jsexn.cpp \
    mozilla-central/js/src/jsfriendapi.cpp \
    mozilla-central/js/src/jsfun.cpp \
    mozilla-central/js/src/jsgcchunk.cpp \
    mozilla-central/js/src/jsgc.cpp \
    mozilla-central/js/src/jsgcstats.cpp \
    mozilla-central/js/src/jshash.cpp \
    mozilla-central/js/src/jsinterp.cpp \
    mozilla-central/js/src/jsinvoke.cpp \
    mozilla-central/js/src/jsiter.cpp \
    mozilla-central/js/src/jskwgen.cpp \
    mozilla-central/js/src/jslock.cpp \
    mozilla-central/js/src/jslog2.cpp \
    mozilla-central/js/src/jsmath.cpp \
    mozilla-central/js/src/jsnativestack.cpp \
    mozilla-central/js/src/jsnum.cpp \
    mozilla-central/js/src/jsobj.cpp \
    mozilla-central/js/src/json.cpp \
    mozilla-central/js/src/jsopcode.cpp \
    mozilla-central/js/src/jsoplengen.cpp \
    mozilla-central/js/src/jsparse.cpp \
    mozilla-central/js/src/jsprf.cpp \
    mozilla-central/js/src/jsprobes.cpp \
    mozilla-central/js/src/jspropertycache.cpp \
    mozilla-central/js/src/jspropertytree.cpp \
    mozilla-central/js/src/jsproxy.cpp \
    mozilla-central/js/src/jsreflect.cpp \
    mozilla-central/js/src/jsregexp.cpp \
    mozilla-central/js/src/jsscan.cpp \
    mozilla-central/js/src/jsscope.cpp \
    mozilla-central/js/src/jsscript.cpp \
    mozilla-central/js/src/jsstr.cpp \
    mozilla-central/js/src/jstracer.cpp \
    mozilla-central/js/src/jstypedarray.cpp \
    mozilla-central/js/src/jsutil.cpp \
    mozilla-central/js/src/jswrapper.cpp \
    mozilla-central/js/src/jsxdrapi.cpp \
    mozilla-central/js/src/jsxml.cpp \
    mozilla-central/js/src/assembler/assembler/ARMAssembler.cpp \
    mozilla-central/js/src/assembler/assembler/MacroAssemblerARM.cpp \
    mozilla-central/js/src/assembler/jit/ExecutableAllocator.cpp \
    mozilla-central/js/src/assembler/wtf/Assertions.cpp \
    mozilla-central/js/src/methodjit/Compiler.cpp \
    mozilla-central/js/src/methodjit/FastArithmetic.cpp \
    mozilla-central/js/src/methodjit/FastOps.cpp \
    mozilla-central/js/src/methodjit/FrameState.cpp \
    mozilla-central/js/src/methodjit/ImmutableSync.cpp \
    mozilla-central/js/src/methodjit/InvokeHelpers.cpp \
    mozilla-central/js/src/methodjit/Logging.cpp \
    mozilla-central/js/src/methodjit/MethodJIT.cpp \
    mozilla-central/js/src/methodjit/MonoIC.cpp \
    mozilla-central/js/src/methodjit/PolyIC.cpp \
    mozilla-central/js/src/methodjit/Retcon.cpp \
    mozilla-central/js/src/methodjit/StubCalls.cpp \
    mozilla-central/js/src/methodjit/StubCompiler.cpp \
    mozilla-central/js/src/methodjit/TrampolineCompiler.cpp \
    mozilla-central/js/src/nanojit/Allocator.cpp \
    mozilla-central/js/src/nanojit/Assembler.cpp \
    mozilla-central/js/src/nanojit/avmplus.cpp \
    mozilla-central/js/src/nanojit/CodeAlloc.cpp \
    mozilla-central/js/src/nanojit/Containers.cpp \
    mozilla-central/js/src/nanojit/Fragmento.cpp \
    mozilla-central/js/src/nanojit/LIR.cpp \
    mozilla-central/js/src/nanojit/NativeARM.cpp \
    mozilla-central/js/src/nanojit/njconfig.cpp \
    mozilla-central/js/src/nanojit/RegAlloc.cpp \
    mozilla-central/js/src/nanojit/VMPI.cpp \
    mozilla-central/js/src/prmjtime.cpp \
    mozilla-central/js/src/sharkctl.cpp \
    mozilla-central/js/src/tracejit/Writer.cpp \
    mozilla-central/js/src/yarr/pcre/pcre_compile.cpp \
    mozilla-central/js/src/yarr/pcre/pcre_exec.cpp \
    mozilla-central/js/src/yarr/pcre/pcre_tables.cpp \
    mozilla-central/js/src/yarr/pcre/pcre_ucp_searchfuncs.cpp \
    mozilla-central/js/src/yarr/pcre/pcre_xclass.cpp \
    mozilla-central/js/src/yarr/pcre/ucptable.cpp \
    mozilla-central/js/src/yarr/yarr/RegexCompiler.cpp \
    mozilla-central/js/src/yarr/yarr/RegexJIT.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/android-build/js \
    $(LOCAL_PATH)/android-build/nspr/dist/include/nspr \
    $(LOCAL_PATH)/mozilla-central/js/src \
    $(LOCAL_PATH)/mozilla-central/js/src/assembler \
    $(LOCAL_PATH)/mozilla-central/js/src/assembler/assembler \
    $(LOCAL_PATH)/mozilla-central/js/src/assembler/jit \
    $(LOCAL_PATH)/mozilla-central/js/src/assembler/wtf \
    $(LOCAL_PATH)/mozilla-central/js/src/yarr/yarr \
    $(LOCAL_PATH)/mozilla-central/js/src/yarr/wtf \
    $(LOCAL_PATH)/mozilla-central/js/src/yarr/pcre \
    $(LOCAL_PATH)/mozilla-central/js/src/yarr/ \
    $(LOCAL_PATH)/mozilla-central/js/src/methodjit
    #$(LOCAL_PATH)/android-build/js/dist/include \

include $(BUILD_SHARED_LIBRARY)

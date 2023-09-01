# Makefile generated by XPJ for linux64
-include Makefile.custom
ProjectName = SnippetConvert
SnippetConvert_cppfiles   += ./../../SnippetCommon/ClassicMain.cpp
SnippetConvert_cppfiles   += ./../../SnippetConvert/SnippetConvert.cpp

SnippetConvert_cpp_debug_dep    = $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(SnippetConvert_cppfiles)))))
SnippetConvert_cc_debug_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.debug.P, $(SnippetConvert_ccfiles)))))
SnippetConvert_c_debug_dep      = $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(SnippetConvert_cfiles)))))
SnippetConvert_debug_dep      = $(SnippetConvert_cpp_debug_dep) $(SnippetConvert_cc_debug_dep) $(SnippetConvert_c_debug_dep)
-include $(SnippetConvert_debug_dep)
SnippetConvert_cpp_checked_dep    = $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(SnippetConvert_cppfiles)))))
SnippetConvert_cc_checked_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.checked.P, $(SnippetConvert_ccfiles)))))
SnippetConvert_c_checked_dep      = $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(SnippetConvert_cfiles)))))
SnippetConvert_checked_dep      = $(SnippetConvert_cpp_checked_dep) $(SnippetConvert_cc_checked_dep) $(SnippetConvert_c_checked_dep)
-include $(SnippetConvert_checked_dep)
SnippetConvert_cpp_profile_dep    = $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(SnippetConvert_cppfiles)))))
SnippetConvert_cc_profile_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.profile.P, $(SnippetConvert_ccfiles)))))
SnippetConvert_c_profile_dep      = $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(SnippetConvert_cfiles)))))
SnippetConvert_profile_dep      = $(SnippetConvert_cpp_profile_dep) $(SnippetConvert_cc_profile_dep) $(SnippetConvert_c_profile_dep)
-include $(SnippetConvert_profile_dep)
SnippetConvert_cpp_release_dep    = $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(SnippetConvert_cppfiles)))))
SnippetConvert_cc_release_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.release.P, $(SnippetConvert_ccfiles)))))
SnippetConvert_c_release_dep      = $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(SnippetConvert_cfiles)))))
SnippetConvert_release_dep      = $(SnippetConvert_cpp_release_dep) $(SnippetConvert_cc_release_dep) $(SnippetConvert_c_release_dep)
-include $(SnippetConvert_release_dep)
SnippetConvert_debug_hpaths    := 
SnippetConvert_debug_hpaths    += ./../../../Include
SnippetConvert_debug_lpaths    := 
SnippetConvert_debug_lpaths    += ./../../../Lib/linux64
SnippetConvert_debug_lpaths    += ./../../lib/linux64
SnippetConvert_debug_lpaths    += ./../../../Bin/linux64
SnippetConvert_debug_lpaths    += ./../../lib/linux64
SnippetConvert_debug_defines   := $(SnippetConvert_custom_defines)
SnippetConvert_debug_defines   += PHYSX_PROFILE_SDK
SnippetConvert_debug_defines   += RENDER_SNIPPET
SnippetConvert_debug_defines   += _DEBUG
SnippetConvert_debug_defines   += PX_DEBUG
SnippetConvert_debug_defines   += PX_CHECKED
SnippetConvert_debug_defines   += PX_SUPPORT_VISUAL_DEBUGGER
SnippetConvert_debug_libraries := 
SnippetConvert_debug_libraries += SnippetRenderDEBUG
SnippetConvert_debug_libraries += SnippetUtilsDEBUG
SnippetConvert_debug_libraries += PhysX3DEBUG_x64
SnippetConvert_debug_libraries += PhysX3CommonDEBUG_x64
SnippetConvert_debug_libraries += PhysX3CookingDEBUG_x64
SnippetConvert_debug_libraries += PhysX3CharacterKinematicDEBUG_x64
SnippetConvert_debug_libraries += PhysX3ExtensionsDEBUG
SnippetConvert_debug_libraries += PhysX3VehicleDEBUG
SnippetConvert_debug_libraries += PhysXProfileSDKDEBUG
SnippetConvert_debug_libraries += PhysXVisualDebuggerSDKDEBUG
SnippetConvert_debug_libraries += PxTaskDEBUG
SnippetConvert_debug_libraries += SnippetUtilsDEBUG
SnippetConvert_debug_libraries += SnippetRenderDEBUG
SnippetConvert_debug_libraries += GL
SnippetConvert_debug_libraries += GLU
SnippetConvert_debug_libraries += glut
SnippetConvert_debug_libraries += X11
SnippetConvert_debug_libraries += rt
SnippetConvert_debug_libraries += pthread
SnippetConvert_debug_common_cflags	:= $(SnippetConvert_custom_cflags)
SnippetConvert_debug_common_cflags    += -MMD
SnippetConvert_debug_common_cflags    += $(addprefix -D, $(SnippetConvert_debug_defines))
SnippetConvert_debug_common_cflags    += $(addprefix -I, $(SnippetConvert_debug_hpaths))
SnippetConvert_debug_common_cflags  += -m64
SnippetConvert_debug_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
SnippetConvert_debug_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
SnippetConvert_debug_common_cflags  += -Wno-long-long
SnippetConvert_debug_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized
SnippetConvert_debug_common_cflags  += -Wno-unused-parameter
SnippetConvert_debug_common_cflags  += -g3 -gdwarf-2
SnippetConvert_debug_cflags	:= $(SnippetConvert_debug_common_cflags)
SnippetConvert_debug_cppflags	:= $(SnippetConvert_debug_common_cflags)
SnippetConvert_debug_lflags    := $(SnippetConvert_custom_lflags)
SnippetConvert_debug_lflags    += $(addprefix -L, $(SnippetConvert_debug_lpaths))
SnippetConvert_debug_lflags    += -Wl,--start-group $(addprefix -l, $(SnippetConvert_debug_libraries)) -Wl,--end-group
SnippetConvert_debug_lflags  += -lrt
SnippetConvert_debug_lflags  += -Wl,-rpath ./
SnippetConvert_debug_lflags  += -m64
SnippetConvert_debug_objsdir  = $(OBJS_DIR)/SnippetConvert_debug
SnippetConvert_debug_cpp_o    = $(addprefix $(SnippetConvert_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(SnippetConvert_cppfiles)))))
SnippetConvert_debug_cc_o    = $(addprefix $(SnippetConvert_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(SnippetConvert_ccfiles)))))
SnippetConvert_debug_c_o      = $(addprefix $(SnippetConvert_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(SnippetConvert_cfiles)))))
SnippetConvert_debug_obj      = $(SnippetConvert_debug_cpp_o) $(SnippetConvert_debug_cc_o) $(SnippetConvert_debug_c_o)
SnippetConvert_debug_bin      := ./../../../Bin/linux64/SnippetConvertDEBUG

clean_SnippetConvert_debug: 
	@$(ECHO) clean SnippetConvert debug
	@$(RMDIR) $(SnippetConvert_debug_objsdir)
	@$(RMDIR) $(SnippetConvert_debug_bin)
	@$(RMDIR) $(DEPSDIR)/SnippetConvert/debug

build_SnippetConvert_debug: postbuild_SnippetConvert_debug
postbuild_SnippetConvert_debug: mainbuild_SnippetConvert_debug
mainbuild_SnippetConvert_debug: prebuild_SnippetConvert_debug $(SnippetConvert_debug_bin)
prebuild_SnippetConvert_debug:

$(SnippetConvert_debug_bin): $(SnippetConvert_debug_obj) build_SnippetRender_debug build_SnippetUtils_debug 
	mkdir -p `dirname ./../../../Bin/linux64/SnippetConvertDEBUG`
	$(CCLD) $(SnippetConvert_debug_obj) $(SnippetConvert_debug_lflags) -o $(SnippetConvert_debug_bin) 
	$(ECHO) building $@ complete!

SnippetConvert_debug_DEPDIR = $(dir $(@))/$(*F)
$(SnippetConvert_debug_cpp_o): $(SnippetConvert_debug_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling debug $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_debug_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cppfiles))))))
	cp $(SnippetConvert_debug_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  rm -f $(SnippetConvert_debug_DEPDIR).d

$(SnippetConvert_debug_cc_o): $(SnippetConvert_debug_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling debug $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_debug_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_ccfiles))))))
	cp $(SnippetConvert_debug_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_ccfiles))))).debug.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_ccfiles))))).debug.P; \
	  rm -f $(SnippetConvert_debug_DEPDIR).d

$(SnippetConvert_debug_c_o): $(SnippetConvert_debug_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling debug $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(SnippetConvert_debug_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cfiles))))))
	cp $(SnippetConvert_debug_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_debug_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  rm -f $(SnippetConvert_debug_DEPDIR).d

SnippetConvert_checked_hpaths    := 
SnippetConvert_checked_hpaths    += ./../../../Include
SnippetConvert_checked_lpaths    := 
SnippetConvert_checked_lpaths    += ./../../../Lib/linux64
SnippetConvert_checked_lpaths    += ./../../lib/linux64
SnippetConvert_checked_lpaths    += ./../../../Bin/linux64
SnippetConvert_checked_lpaths    += ./../../lib/linux64
SnippetConvert_checked_defines   := $(SnippetConvert_custom_defines)
SnippetConvert_checked_defines   += PHYSX_PROFILE_SDK
SnippetConvert_checked_defines   += RENDER_SNIPPET
SnippetConvert_checked_defines   += NDEBUG
SnippetConvert_checked_defines   += PX_CHECKED
SnippetConvert_checked_defines   += PX_SUPPORT_VISUAL_DEBUGGER
SnippetConvert_checked_libraries := 
SnippetConvert_checked_libraries += SnippetRenderCHECKED
SnippetConvert_checked_libraries += SnippetUtilsCHECKED
SnippetConvert_checked_libraries += PhysX3CHECKED_x64
SnippetConvert_checked_libraries += PhysX3CommonCHECKED_x64
SnippetConvert_checked_libraries += PhysX3CookingCHECKED_x64
SnippetConvert_checked_libraries += PhysX3CharacterKinematicCHECKED_x64
SnippetConvert_checked_libraries += PhysX3ExtensionsCHECKED
SnippetConvert_checked_libraries += PhysX3VehicleCHECKED
SnippetConvert_checked_libraries += PhysXProfileSDKCHECKED
SnippetConvert_checked_libraries += PhysXVisualDebuggerSDKCHECKED
SnippetConvert_checked_libraries += PxTaskCHECKED
SnippetConvert_checked_libraries += SnippetUtilsCHECKED
SnippetConvert_checked_libraries += SnippetRenderCHECKED
SnippetConvert_checked_libraries += GL
SnippetConvert_checked_libraries += GLU
SnippetConvert_checked_libraries += glut
SnippetConvert_checked_libraries += X11
SnippetConvert_checked_libraries += rt
SnippetConvert_checked_libraries += pthread
SnippetConvert_checked_common_cflags	:= $(SnippetConvert_custom_cflags)
SnippetConvert_checked_common_cflags    += -MMD
SnippetConvert_checked_common_cflags    += $(addprefix -D, $(SnippetConvert_checked_defines))
SnippetConvert_checked_common_cflags    += $(addprefix -I, $(SnippetConvert_checked_hpaths))
SnippetConvert_checked_common_cflags  += -m64
SnippetConvert_checked_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
SnippetConvert_checked_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
SnippetConvert_checked_common_cflags  += -Wno-long-long
SnippetConvert_checked_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized
SnippetConvert_checked_common_cflags  += -Wno-unused-parameter
SnippetConvert_checked_common_cflags  += -g3 -gdwarf-2 -O3 -fno-strict-aliasing
SnippetConvert_checked_cflags	:= $(SnippetConvert_checked_common_cflags)
SnippetConvert_checked_cppflags	:= $(SnippetConvert_checked_common_cflags)
SnippetConvert_checked_lflags    := $(SnippetConvert_custom_lflags)
SnippetConvert_checked_lflags    += $(addprefix -L, $(SnippetConvert_checked_lpaths))
SnippetConvert_checked_lflags    += -Wl,--start-group $(addprefix -l, $(SnippetConvert_checked_libraries)) -Wl,--end-group
SnippetConvert_checked_lflags  += -lrt
SnippetConvert_checked_lflags  += -Wl,-rpath ./
SnippetConvert_checked_lflags  += -m64
SnippetConvert_checked_objsdir  = $(OBJS_DIR)/SnippetConvert_checked
SnippetConvert_checked_cpp_o    = $(addprefix $(SnippetConvert_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(SnippetConvert_cppfiles)))))
SnippetConvert_checked_cc_o    = $(addprefix $(SnippetConvert_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(SnippetConvert_ccfiles)))))
SnippetConvert_checked_c_o      = $(addprefix $(SnippetConvert_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(SnippetConvert_cfiles)))))
SnippetConvert_checked_obj      = $(SnippetConvert_checked_cpp_o) $(SnippetConvert_checked_cc_o) $(SnippetConvert_checked_c_o)
SnippetConvert_checked_bin      := ./../../../Bin/linux64/SnippetConvertCHECKED

clean_SnippetConvert_checked: 
	@$(ECHO) clean SnippetConvert checked
	@$(RMDIR) $(SnippetConvert_checked_objsdir)
	@$(RMDIR) $(SnippetConvert_checked_bin)
	@$(RMDIR) $(DEPSDIR)/SnippetConvert/checked

build_SnippetConvert_checked: postbuild_SnippetConvert_checked
postbuild_SnippetConvert_checked: mainbuild_SnippetConvert_checked
mainbuild_SnippetConvert_checked: prebuild_SnippetConvert_checked $(SnippetConvert_checked_bin)
prebuild_SnippetConvert_checked:

$(SnippetConvert_checked_bin): $(SnippetConvert_checked_obj) build_SnippetRender_checked build_SnippetUtils_checked 
	mkdir -p `dirname ./../../../Bin/linux64/SnippetConvertCHECKED`
	$(CCLD) $(SnippetConvert_checked_obj) $(SnippetConvert_checked_lflags) -o $(SnippetConvert_checked_bin) 
	$(ECHO) building $@ complete!

SnippetConvert_checked_DEPDIR = $(dir $(@))/$(*F)
$(SnippetConvert_checked_cpp_o): $(SnippetConvert_checked_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling checked $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_checked_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cppfiles))))))
	cp $(SnippetConvert_checked_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  rm -f $(SnippetConvert_checked_DEPDIR).d

$(SnippetConvert_checked_cc_o): $(SnippetConvert_checked_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling checked $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_checked_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_ccfiles))))))
	cp $(SnippetConvert_checked_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_ccfiles))))).checked.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_ccfiles))))).checked.P; \
	  rm -f $(SnippetConvert_checked_DEPDIR).d

$(SnippetConvert_checked_c_o): $(SnippetConvert_checked_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling checked $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(SnippetConvert_checked_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cfiles))))))
	cp $(SnippetConvert_checked_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_checked_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  rm -f $(SnippetConvert_checked_DEPDIR).d

SnippetConvert_profile_hpaths    := 
SnippetConvert_profile_hpaths    += ./../../../Include
SnippetConvert_profile_lpaths    := 
SnippetConvert_profile_lpaths    += ./../../../Lib/linux64
SnippetConvert_profile_lpaths    += ./../../lib/linux64
SnippetConvert_profile_lpaths    += ./../../../Bin/linux64
SnippetConvert_profile_lpaths    += ./../../lib/linux64
SnippetConvert_profile_defines   := $(SnippetConvert_custom_defines)
SnippetConvert_profile_defines   += PHYSX_PROFILE_SDK
SnippetConvert_profile_defines   += RENDER_SNIPPET
SnippetConvert_profile_defines   += NDEBUG
SnippetConvert_profile_defines   += PX_PROFILE
SnippetConvert_profile_defines   += PX_SUPPORT_VISUAL_DEBUGGER
SnippetConvert_profile_libraries := 
SnippetConvert_profile_libraries += SnippetRenderPROFILE
SnippetConvert_profile_libraries += SnippetUtilsPROFILE
SnippetConvert_profile_libraries += PhysX3PROFILE_x64
SnippetConvert_profile_libraries += PhysX3CommonPROFILE_x64
SnippetConvert_profile_libraries += PhysX3CookingPROFILE_x64
SnippetConvert_profile_libraries += PhysX3CharacterKinematicPROFILE_x64
SnippetConvert_profile_libraries += PhysX3ExtensionsPROFILE
SnippetConvert_profile_libraries += PhysX3VehiclePROFILE
SnippetConvert_profile_libraries += PhysXProfileSDKPROFILE
SnippetConvert_profile_libraries += PhysXVisualDebuggerSDKPROFILE
SnippetConvert_profile_libraries += PxTaskPROFILE
SnippetConvert_profile_libraries += SnippetUtilsPROFILE
SnippetConvert_profile_libraries += SnippetRenderPROFILE
SnippetConvert_profile_libraries += GL
SnippetConvert_profile_libraries += GLU
SnippetConvert_profile_libraries += glut
SnippetConvert_profile_libraries += X11
SnippetConvert_profile_libraries += rt
SnippetConvert_profile_libraries += pthread
SnippetConvert_profile_common_cflags	:= $(SnippetConvert_custom_cflags)
SnippetConvert_profile_common_cflags    += -MMD
SnippetConvert_profile_common_cflags    += $(addprefix -D, $(SnippetConvert_profile_defines))
SnippetConvert_profile_common_cflags    += $(addprefix -I, $(SnippetConvert_profile_hpaths))
SnippetConvert_profile_common_cflags  += -m64
SnippetConvert_profile_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
SnippetConvert_profile_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
SnippetConvert_profile_common_cflags  += -Wno-long-long
SnippetConvert_profile_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized
SnippetConvert_profile_common_cflags  += -Wno-unused-parameter
SnippetConvert_profile_common_cflags  += -O3 -fno-strict-aliasing
SnippetConvert_profile_cflags	:= $(SnippetConvert_profile_common_cflags)
SnippetConvert_profile_cppflags	:= $(SnippetConvert_profile_common_cflags)
SnippetConvert_profile_lflags    := $(SnippetConvert_custom_lflags)
SnippetConvert_profile_lflags    += $(addprefix -L, $(SnippetConvert_profile_lpaths))
SnippetConvert_profile_lflags    += -Wl,--start-group $(addprefix -l, $(SnippetConvert_profile_libraries)) -Wl,--end-group
SnippetConvert_profile_lflags  += -lrt
SnippetConvert_profile_lflags  += -Wl,-rpath ./
SnippetConvert_profile_lflags  += -m64
SnippetConvert_profile_objsdir  = $(OBJS_DIR)/SnippetConvert_profile
SnippetConvert_profile_cpp_o    = $(addprefix $(SnippetConvert_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(SnippetConvert_cppfiles)))))
SnippetConvert_profile_cc_o    = $(addprefix $(SnippetConvert_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(SnippetConvert_ccfiles)))))
SnippetConvert_profile_c_o      = $(addprefix $(SnippetConvert_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(SnippetConvert_cfiles)))))
SnippetConvert_profile_obj      = $(SnippetConvert_profile_cpp_o) $(SnippetConvert_profile_cc_o) $(SnippetConvert_profile_c_o)
SnippetConvert_profile_bin      := ./../../../Bin/linux64/SnippetConvertPROFILE

clean_SnippetConvert_profile: 
	@$(ECHO) clean SnippetConvert profile
	@$(RMDIR) $(SnippetConvert_profile_objsdir)
	@$(RMDIR) $(SnippetConvert_profile_bin)
	@$(RMDIR) $(DEPSDIR)/SnippetConvert/profile

build_SnippetConvert_profile: postbuild_SnippetConvert_profile
postbuild_SnippetConvert_profile: mainbuild_SnippetConvert_profile
mainbuild_SnippetConvert_profile: prebuild_SnippetConvert_profile $(SnippetConvert_profile_bin)
prebuild_SnippetConvert_profile:

$(SnippetConvert_profile_bin): $(SnippetConvert_profile_obj) build_SnippetRender_profile build_SnippetUtils_profile 
	mkdir -p `dirname ./../../../Bin/linux64/SnippetConvertPROFILE`
	$(CCLD) $(SnippetConvert_profile_obj) $(SnippetConvert_profile_lflags) -o $(SnippetConvert_profile_bin) 
	$(ECHO) building $@ complete!

SnippetConvert_profile_DEPDIR = $(dir $(@))/$(*F)
$(SnippetConvert_profile_cpp_o): $(SnippetConvert_profile_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling profile $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_profile_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cppfiles))))))
	cp $(SnippetConvert_profile_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  rm -f $(SnippetConvert_profile_DEPDIR).d

$(SnippetConvert_profile_cc_o): $(SnippetConvert_profile_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling profile $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_profile_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_ccfiles))))))
	cp $(SnippetConvert_profile_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_ccfiles))))).profile.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_ccfiles))))).profile.P; \
	  rm -f $(SnippetConvert_profile_DEPDIR).d

$(SnippetConvert_profile_c_o): $(SnippetConvert_profile_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling profile $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(SnippetConvert_profile_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cfiles))))))
	cp $(SnippetConvert_profile_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_profile_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  rm -f $(SnippetConvert_profile_DEPDIR).d

SnippetConvert_release_hpaths    := 
SnippetConvert_release_hpaths    += ./../../../Include
SnippetConvert_release_lpaths    := 
SnippetConvert_release_lpaths    += ./../../../Lib/linux64
SnippetConvert_release_lpaths    += ./../../lib/linux64
SnippetConvert_release_lpaths    += ./../../../Bin/linux64
SnippetConvert_release_lpaths    += ./../../lib/linux64
SnippetConvert_release_defines   := $(SnippetConvert_custom_defines)
SnippetConvert_release_defines   += PHYSX_PROFILE_SDK
SnippetConvert_release_defines   += RENDER_SNIPPET
SnippetConvert_release_defines   += NDEBUG
SnippetConvert_release_libraries := 
SnippetConvert_release_libraries += SnippetRender
SnippetConvert_release_libraries += SnippetUtils
SnippetConvert_release_libraries += PhysX3_x64
SnippetConvert_release_libraries += PhysX3Common_x64
SnippetConvert_release_libraries += PhysX3Cooking_x64
SnippetConvert_release_libraries += PhysX3CharacterKinematic_x64
SnippetConvert_release_libraries += PhysX3Extensions
SnippetConvert_release_libraries += PhysX3Vehicle
SnippetConvert_release_libraries += PhysXProfileSDK
SnippetConvert_release_libraries += PhysXVisualDebuggerSDK
SnippetConvert_release_libraries += PxTask
SnippetConvert_release_libraries += SnippetUtils
SnippetConvert_release_libraries += SnippetRender
SnippetConvert_release_libraries += GL
SnippetConvert_release_libraries += GLU
SnippetConvert_release_libraries += glut
SnippetConvert_release_libraries += X11
SnippetConvert_release_libraries += rt
SnippetConvert_release_libraries += pthread
SnippetConvert_release_common_cflags	:= $(SnippetConvert_custom_cflags)
SnippetConvert_release_common_cflags    += -MMD
SnippetConvert_release_common_cflags    += $(addprefix -D, $(SnippetConvert_release_defines))
SnippetConvert_release_common_cflags    += $(addprefix -I, $(SnippetConvert_release_hpaths))
SnippetConvert_release_common_cflags  += -m64
SnippetConvert_release_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
SnippetConvert_release_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
SnippetConvert_release_common_cflags  += -Wno-long-long
SnippetConvert_release_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized
SnippetConvert_release_common_cflags  += -Wno-unused-parameter
SnippetConvert_release_common_cflags  += -O3 -fno-strict-aliasing
SnippetConvert_release_cflags	:= $(SnippetConvert_release_common_cflags)
SnippetConvert_release_cppflags	:= $(SnippetConvert_release_common_cflags)
SnippetConvert_release_lflags    := $(SnippetConvert_custom_lflags)
SnippetConvert_release_lflags    += $(addprefix -L, $(SnippetConvert_release_lpaths))
SnippetConvert_release_lflags    += -Wl,--start-group $(addprefix -l, $(SnippetConvert_release_libraries)) -Wl,--end-group
SnippetConvert_release_lflags  += -lrt
SnippetConvert_release_lflags  += -Wl,-rpath ./
SnippetConvert_release_lflags  += -m64
SnippetConvert_release_objsdir  = $(OBJS_DIR)/SnippetConvert_release
SnippetConvert_release_cpp_o    = $(addprefix $(SnippetConvert_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(SnippetConvert_cppfiles)))))
SnippetConvert_release_cc_o    = $(addprefix $(SnippetConvert_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(SnippetConvert_ccfiles)))))
SnippetConvert_release_c_o      = $(addprefix $(SnippetConvert_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(SnippetConvert_cfiles)))))
SnippetConvert_release_obj      = $(SnippetConvert_release_cpp_o) $(SnippetConvert_release_cc_o) $(SnippetConvert_release_c_o)
SnippetConvert_release_bin      := ./../../../Bin/linux64/SnippetConvert

clean_SnippetConvert_release: 
	@$(ECHO) clean SnippetConvert release
	@$(RMDIR) $(SnippetConvert_release_objsdir)
	@$(RMDIR) $(SnippetConvert_release_bin)
	@$(RMDIR) $(DEPSDIR)/SnippetConvert/release

build_SnippetConvert_release: postbuild_SnippetConvert_release
postbuild_SnippetConvert_release: mainbuild_SnippetConvert_release
mainbuild_SnippetConvert_release: prebuild_SnippetConvert_release $(SnippetConvert_release_bin)
prebuild_SnippetConvert_release:

$(SnippetConvert_release_bin): $(SnippetConvert_release_obj) build_SnippetRender_release build_SnippetUtils_release 
	mkdir -p `dirname ./../../../Bin/linux64/SnippetConvert`
	$(CCLD) $(SnippetConvert_release_obj) $(SnippetConvert_release_lflags) -o $(SnippetConvert_release_bin) 
	$(ECHO) building $@ complete!

SnippetConvert_release_DEPDIR = $(dir $(@))/$(*F)
$(SnippetConvert_release_cpp_o): $(SnippetConvert_release_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling release $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_release_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cppfiles))))))
	cp $(SnippetConvert_release_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cppfiles))))).P; \
	  rm -f $(SnippetConvert_release_DEPDIR).d

$(SnippetConvert_release_cc_o): $(SnippetConvert_release_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling release $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(SnippetConvert_release_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_ccfiles))))))
	cp $(SnippetConvert_release_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_ccfiles))))).release.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_ccfiles))))).release.P; \
	  rm -f $(SnippetConvert_release_DEPDIR).d

$(SnippetConvert_release_c_o): $(SnippetConvert_release_objsdir)/%.o:
	$(ECHO) SnippetConvert: compiling release $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(SnippetConvert_release_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cfiles))))))
	cp $(SnippetConvert_release_DEPDIR).d $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(SnippetConvert_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/SnippetConvert/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(SnippetConvert_release_objsdir),, $@))), $(SnippetConvert_cfiles))))).P; \
	  rm -f $(SnippetConvert_release_DEPDIR).d

clean_SnippetConvert:  clean_SnippetConvert_debug clean_SnippetConvert_checked clean_SnippetConvert_profile clean_SnippetConvert_release
	rm -rf $(DEPSDIR)

export VERBOSE
ifndef VERBOSE
.SILENT:
endif

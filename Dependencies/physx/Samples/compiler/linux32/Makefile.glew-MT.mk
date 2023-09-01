# Makefile generated by XPJ for linux32
-include Makefile.custom
ProjectName = glew-MT
glew-MT_cfiles   += ./../../../externals/glew/src/glew.c

glew-MT_cpp_debug_dep    = $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(glew-MT_cppfiles)))))
glew-MT_cc_debug_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.debug.P, $(glew-MT_ccfiles)))))
glew-MT_c_debug_dep      = $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(glew-MT_cfiles)))))
glew-MT_debug_dep      = $(glew-MT_cpp_debug_dep) $(glew-MT_cc_debug_dep) $(glew-MT_c_debug_dep)
-include $(glew-MT_debug_dep)
glew-MT_cpp_release_dep    = $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(glew-MT_cppfiles)))))
glew-MT_cc_release_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.release.P, $(glew-MT_ccfiles)))))
glew-MT_c_release_dep      = $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(glew-MT_cfiles)))))
glew-MT_release_dep      = $(glew-MT_cpp_release_dep) $(glew-MT_cc_release_dep) $(glew-MT_c_release_dep)
-include $(glew-MT_release_dep)
glew-MT_cpp_profile_dep    = $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(glew-MT_cppfiles)))))
glew-MT_cc_profile_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.profile.P, $(glew-MT_ccfiles)))))
glew-MT_c_profile_dep      = $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(glew-MT_cfiles)))))
glew-MT_profile_dep      = $(glew-MT_cpp_profile_dep) $(glew-MT_cc_profile_dep) $(glew-MT_c_profile_dep)
-include $(glew-MT_profile_dep)
glew-MT_cpp_checked_dep    = $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(glew-MT_cppfiles)))))
glew-MT_cc_checked_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.checked.P, $(glew-MT_ccfiles)))))
glew-MT_c_checked_dep      = $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(glew-MT_cfiles)))))
glew-MT_checked_dep      = $(glew-MT_cpp_checked_dep) $(glew-MT_cc_checked_dep) $(glew-MT_c_checked_dep)
-include $(glew-MT_checked_dep)
glew-MT_debug_hpaths    := 
glew-MT_debug_hpaths    += ./../../../externals/glew/include
glew-MT_debug_lpaths    := 
glew-MT_debug_defines   := $(glew-MT_custom_defines)
glew-MT_debug_defines   += GLEW_STATIC
glew-MT_debug_defines   += _DEBUG
glew-MT_debug_libraries := 
glew-MT_debug_common_cflags	:= $(glew-MT_custom_cflags)
glew-MT_debug_common_cflags    += -MMD
glew-MT_debug_common_cflags    += $(addprefix -D, $(glew-MT_debug_defines))
glew-MT_debug_common_cflags    += $(addprefix -I, $(glew-MT_debug_hpaths))
glew-MT_debug_common_cflags  += -m32
glew-MT_debug_common_cflags  += -O0 -g3
glew-MT_debug_cflags	:= $(glew-MT_debug_common_cflags)
glew-MT_debug_cppflags	:= $(glew-MT_debug_common_cflags)
glew-MT_debug_lflags    := $(glew-MT_custom_lflags)
glew-MT_debug_lflags    += $(addprefix -L, $(glew-MT_debug_lpaths))
glew-MT_debug_lflags    += -Wl,--start-group $(addprefix -l, $(glew-MT_debug_libraries)) -Wl,--end-group
glew-MT_debug_lflags  += -m32
glew-MT_debug_objsdir  = $(OBJS_DIR)/glew-MT_debug
glew-MT_debug_cpp_o    = $(addprefix $(glew-MT_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(glew-MT_cppfiles)))))
glew-MT_debug_cc_o    = $(addprefix $(glew-MT_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(glew-MT_ccfiles)))))
glew-MT_debug_c_o      = $(addprefix $(glew-MT_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(glew-MT_cfiles)))))
glew-MT_debug_obj      = $(glew-MT_debug_cpp_o) $(glew-MT_debug_cc_o) $(glew-MT_debug_c_o)
glew-MT_debug_bin      := ./../../../externals/glew/lib/linux32/libglew_debug.a

clean_glew-MT_debug: 
	@$(ECHO) clean glew-MT debug
	@$(RMDIR) $(glew-MT_debug_objsdir)
	@$(RMDIR) $(glew-MT_debug_bin)
	@$(RMDIR) $(DEPSDIR)/glew-MT/debug

build_glew-MT_debug: postbuild_glew-MT_debug
postbuild_glew-MT_debug: mainbuild_glew-MT_debug
mainbuild_glew-MT_debug: prebuild_glew-MT_debug $(glew-MT_debug_bin)
prebuild_glew-MT_debug:

$(glew-MT_debug_bin): $(glew-MT_debug_obj) 
	mkdir -p `dirname ./../../../externals/glew/lib/linux32/libglew_debug.a`
	@$(AR) rcs $(glew-MT_debug_bin) $(glew-MT_debug_obj)
	$(ECHO) building $@ complete!

glew-MT_debug_DEPDIR = $(dir $(@))/$(*F)
$(glew-MT_debug_cpp_o): $(glew-MT_debug_objsdir)/%.o:
	$(ECHO) glew-MT: compiling debug $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_debug_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cppfiles))))))
	cp $(glew-MT_debug_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  rm -f $(glew-MT_debug_DEPDIR).d

$(glew-MT_debug_cc_o): $(glew-MT_debug_objsdir)/%.o:
	$(ECHO) glew-MT: compiling debug $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_debug_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_ccfiles))))))
	cp $(glew-MT_debug_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_ccfiles))))).debug.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_ccfiles))))).debug.P; \
	  rm -f $(glew-MT_debug_DEPDIR).d

$(glew-MT_debug_c_o): $(glew-MT_debug_objsdir)/%.o:
	$(ECHO) glew-MT: compiling debug $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(glew-MT_debug_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cfiles))))))
	cp $(glew-MT_debug_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_debug_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  rm -f $(glew-MT_debug_DEPDIR).d

glew-MT_release_hpaths    := 
glew-MT_release_hpaths    += ./../../../externals/glew/include
glew-MT_release_lpaths    := 
glew-MT_release_defines   := $(glew-MT_custom_defines)
glew-MT_release_defines   += GLEW_STATIC
glew-MT_release_defines   += NDEBUG
glew-MT_release_libraries := 
glew-MT_release_common_cflags	:= $(glew-MT_custom_cflags)
glew-MT_release_common_cflags    += -MMD
glew-MT_release_common_cflags    += $(addprefix -D, $(glew-MT_release_defines))
glew-MT_release_common_cflags    += $(addprefix -I, $(glew-MT_release_hpaths))
glew-MT_release_common_cflags  += -m32
glew-MT_release_common_cflags  += -Os
glew-MT_release_cflags	:= $(glew-MT_release_common_cflags)
glew-MT_release_cppflags	:= $(glew-MT_release_common_cflags)
glew-MT_release_lflags    := $(glew-MT_custom_lflags)
glew-MT_release_lflags    += $(addprefix -L, $(glew-MT_release_lpaths))
glew-MT_release_lflags    += -Wl,--start-group $(addprefix -l, $(glew-MT_release_libraries)) -Wl,--end-group
glew-MT_release_lflags  += -m32
glew-MT_release_objsdir  = $(OBJS_DIR)/glew-MT_release
glew-MT_release_cpp_o    = $(addprefix $(glew-MT_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(glew-MT_cppfiles)))))
glew-MT_release_cc_o    = $(addprefix $(glew-MT_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(glew-MT_ccfiles)))))
glew-MT_release_c_o      = $(addprefix $(glew-MT_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(glew-MT_cfiles)))))
glew-MT_release_obj      = $(glew-MT_release_cpp_o) $(glew-MT_release_cc_o) $(glew-MT_release_c_o)
glew-MT_release_bin      := ./../../../externals/glew/lib/linux32/libglew_release.a

clean_glew-MT_release: 
	@$(ECHO) clean glew-MT release
	@$(RMDIR) $(glew-MT_release_objsdir)
	@$(RMDIR) $(glew-MT_release_bin)
	@$(RMDIR) $(DEPSDIR)/glew-MT/release

build_glew-MT_release: postbuild_glew-MT_release
postbuild_glew-MT_release: mainbuild_glew-MT_release
mainbuild_glew-MT_release: prebuild_glew-MT_release $(glew-MT_release_bin)
prebuild_glew-MT_release:

$(glew-MT_release_bin): $(glew-MT_release_obj) 
	mkdir -p `dirname ./../../../externals/glew/lib/linux32/libglew_release.a`
	@$(AR) rcs $(glew-MT_release_bin) $(glew-MT_release_obj)
	$(ECHO) building $@ complete!

glew-MT_release_DEPDIR = $(dir $(@))/$(*F)
$(glew-MT_release_cpp_o): $(glew-MT_release_objsdir)/%.o:
	$(ECHO) glew-MT: compiling release $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_release_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cppfiles))))))
	cp $(glew-MT_release_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  rm -f $(glew-MT_release_DEPDIR).d

$(glew-MT_release_cc_o): $(glew-MT_release_objsdir)/%.o:
	$(ECHO) glew-MT: compiling release $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_release_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_ccfiles))))))
	cp $(glew-MT_release_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_ccfiles))))).release.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_ccfiles))))).release.P; \
	  rm -f $(glew-MT_release_DEPDIR).d

$(glew-MT_release_c_o): $(glew-MT_release_objsdir)/%.o:
	$(ECHO) glew-MT: compiling release $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(glew-MT_release_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cfiles))))))
	cp $(glew-MT_release_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_release_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  rm -f $(glew-MT_release_DEPDIR).d

glew-MT_profile_hpaths    := 
glew-MT_profile_hpaths    += ./../../../externals/glew/include
glew-MT_profile_lpaths    := 
glew-MT_profile_defines   := $(glew-MT_custom_defines)
glew-MT_profile_defines   += GLEW_STATIC
glew-MT_profile_defines   += NDEBUG
glew-MT_profile_libraries := 
glew-MT_profile_common_cflags	:= $(glew-MT_custom_cflags)
glew-MT_profile_common_cflags    += -MMD
glew-MT_profile_common_cflags    += $(addprefix -D, $(glew-MT_profile_defines))
glew-MT_profile_common_cflags    += $(addprefix -I, $(glew-MT_profile_hpaths))
glew-MT_profile_common_cflags  += -m32
glew-MT_profile_common_cflags  += -Os
glew-MT_profile_cflags	:= $(glew-MT_profile_common_cflags)
glew-MT_profile_cppflags	:= $(glew-MT_profile_common_cflags)
glew-MT_profile_lflags    := $(glew-MT_custom_lflags)
glew-MT_profile_lflags    += $(addprefix -L, $(glew-MT_profile_lpaths))
glew-MT_profile_lflags    += -Wl,--start-group $(addprefix -l, $(glew-MT_profile_libraries)) -Wl,--end-group
glew-MT_profile_lflags  += -m32
glew-MT_profile_objsdir  = $(OBJS_DIR)/glew-MT_profile
glew-MT_profile_cpp_o    = $(addprefix $(glew-MT_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(glew-MT_cppfiles)))))
glew-MT_profile_cc_o    = $(addprefix $(glew-MT_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(glew-MT_ccfiles)))))
glew-MT_profile_c_o      = $(addprefix $(glew-MT_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(glew-MT_cfiles)))))
glew-MT_profile_obj      = $(glew-MT_profile_cpp_o) $(glew-MT_profile_cc_o) $(glew-MT_profile_c_o)
glew-MT_profile_bin      := ./../../../externals/glew/lib/linux32/libglew_profile.a

clean_glew-MT_profile: 
	@$(ECHO) clean glew-MT profile
	@$(RMDIR) $(glew-MT_profile_objsdir)
	@$(RMDIR) $(glew-MT_profile_bin)
	@$(RMDIR) $(DEPSDIR)/glew-MT/profile

build_glew-MT_profile: postbuild_glew-MT_profile
postbuild_glew-MT_profile: mainbuild_glew-MT_profile
mainbuild_glew-MT_profile: prebuild_glew-MT_profile $(glew-MT_profile_bin)
prebuild_glew-MT_profile:

$(glew-MT_profile_bin): $(glew-MT_profile_obj) 
	mkdir -p `dirname ./../../../externals/glew/lib/linux32/libglew_profile.a`
	@$(AR) rcs $(glew-MT_profile_bin) $(glew-MT_profile_obj)
	$(ECHO) building $@ complete!

glew-MT_profile_DEPDIR = $(dir $(@))/$(*F)
$(glew-MT_profile_cpp_o): $(glew-MT_profile_objsdir)/%.o:
	$(ECHO) glew-MT: compiling profile $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_profile_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cppfiles))))))
	cp $(glew-MT_profile_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  rm -f $(glew-MT_profile_DEPDIR).d

$(glew-MT_profile_cc_o): $(glew-MT_profile_objsdir)/%.o:
	$(ECHO) glew-MT: compiling profile $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_profile_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_ccfiles))))))
	cp $(glew-MT_profile_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_ccfiles))))).profile.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_ccfiles))))).profile.P; \
	  rm -f $(glew-MT_profile_DEPDIR).d

$(glew-MT_profile_c_o): $(glew-MT_profile_objsdir)/%.o:
	$(ECHO) glew-MT: compiling profile $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(glew-MT_profile_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cfiles))))))
	cp $(glew-MT_profile_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_profile_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  rm -f $(glew-MT_profile_DEPDIR).d

glew-MT_checked_hpaths    := 
glew-MT_checked_hpaths    += ./../../../externals/glew/include
glew-MT_checked_lpaths    := 
glew-MT_checked_defines   := $(glew-MT_custom_defines)
glew-MT_checked_defines   += GLEW_STATIC
glew-MT_checked_defines   += NDEBUG
glew-MT_checked_libraries := 
glew-MT_checked_common_cflags	:= $(glew-MT_custom_cflags)
glew-MT_checked_common_cflags    += -MMD
glew-MT_checked_common_cflags    += $(addprefix -D, $(glew-MT_checked_defines))
glew-MT_checked_common_cflags    += $(addprefix -I, $(glew-MT_checked_hpaths))
glew-MT_checked_common_cflags  += -m32
glew-MT_checked_common_cflags  += -Os
glew-MT_checked_cflags	:= $(glew-MT_checked_common_cflags)
glew-MT_checked_cppflags	:= $(glew-MT_checked_common_cflags)
glew-MT_checked_lflags    := $(glew-MT_custom_lflags)
glew-MT_checked_lflags    += $(addprefix -L, $(glew-MT_checked_lpaths))
glew-MT_checked_lflags    += -Wl,--start-group $(addprefix -l, $(glew-MT_checked_libraries)) -Wl,--end-group
glew-MT_checked_lflags  += -m32
glew-MT_checked_objsdir  = $(OBJS_DIR)/glew-MT_checked
glew-MT_checked_cpp_o    = $(addprefix $(glew-MT_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(glew-MT_cppfiles)))))
glew-MT_checked_cc_o    = $(addprefix $(glew-MT_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(glew-MT_ccfiles)))))
glew-MT_checked_c_o      = $(addprefix $(glew-MT_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(glew-MT_cfiles)))))
glew-MT_checked_obj      = $(glew-MT_checked_cpp_o) $(glew-MT_checked_cc_o) $(glew-MT_checked_c_o)
glew-MT_checked_bin      := ./../../../externals/glew/lib/linux32/libglew_checked.a

clean_glew-MT_checked: 
	@$(ECHO) clean glew-MT checked
	@$(RMDIR) $(glew-MT_checked_objsdir)
	@$(RMDIR) $(glew-MT_checked_bin)
	@$(RMDIR) $(DEPSDIR)/glew-MT/checked

build_glew-MT_checked: postbuild_glew-MT_checked
postbuild_glew-MT_checked: mainbuild_glew-MT_checked
mainbuild_glew-MT_checked: prebuild_glew-MT_checked $(glew-MT_checked_bin)
prebuild_glew-MT_checked:

$(glew-MT_checked_bin): $(glew-MT_checked_obj) 
	mkdir -p `dirname ./../../../externals/glew/lib/linux32/libglew_checked.a`
	@$(AR) rcs $(glew-MT_checked_bin) $(glew-MT_checked_obj)
	$(ECHO) building $@ complete!

glew-MT_checked_DEPDIR = $(dir $(@))/$(*F)
$(glew-MT_checked_cpp_o): $(glew-MT_checked_objsdir)/%.o:
	$(ECHO) glew-MT: compiling checked $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_checked_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cppfiles))))))
	cp $(glew-MT_checked_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cppfiles))))).P; \
	  rm -f $(glew-MT_checked_DEPDIR).d

$(glew-MT_checked_cc_o): $(glew-MT_checked_objsdir)/%.o:
	$(ECHO) glew-MT: compiling checked $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(glew-MT_checked_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_ccfiles))))))
	cp $(glew-MT_checked_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_ccfiles))))).checked.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_ccfiles))))).checked.P; \
	  rm -f $(glew-MT_checked_DEPDIR).d

$(glew-MT_checked_c_o): $(glew-MT_checked_objsdir)/%.o:
	$(ECHO) glew-MT: compiling checked $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(glew-MT_checked_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cfiles))))))
	cp $(glew-MT_checked_DEPDIR).d $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(glew-MT_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/glew-MT/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(glew-MT_checked_objsdir),, $@))), $(glew-MT_cfiles))))).P; \
	  rm -f $(glew-MT_checked_DEPDIR).d

clean_glew-MT:  clean_glew-MT_debug clean_glew-MT_release clean_glew-MT_profile clean_glew-MT_checked
	rm -rf $(DEPSDIR)

export VERBOSE
ifndef VERBOSE
.SILENT:
endif

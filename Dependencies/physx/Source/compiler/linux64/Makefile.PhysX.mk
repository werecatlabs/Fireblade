# Makefile generated by XPJ for linux64
-include Makefile.custom
ProjectName = PhysX
PhysX_cppfiles   += ./../../PhysX/src/gpu/NpPhysicsGpu.cpp
PhysX_cppfiles   += ./../../PhysX/src/gpu/PxParticleDeviceExclusive.cpp
PhysX_cppfiles   += ./../../PhysX/src/gpu/PxParticleGpu.cpp
PhysX_cppfiles   += ./../../PhysX/src/gpu/PxPhysXIndicatorDeviceExclusive.cpp
PhysX_cppfiles   += ./../../PhysX/src/device/linux/PhysXIndicatorLinux.cpp
PhysX_cppfiles   += ./../../PhysX/src/GrbEvents.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpActor.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpAggregate.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpArticulation.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpArticulationJoint.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpArticulationLink.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpBatchQuery.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpConstraint.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpFactory.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpMaterial.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpMetaData.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpPhysics.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpPvdSceneQueryCollector.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpReadCheck.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpRigidDynamic.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpRigidStatic.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpScene.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpSceneQueries.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpSerializerAdapter.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpShape.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpShapeManager.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpSpatialIndex.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpVolumeCache.cpp
PhysX_cppfiles   += ./../../PhysX/src/NpWriteCheck.cpp
PhysX_cppfiles   += ./../../PhysX/src/particles/NpParticleFluid.cpp
PhysX_cppfiles   += ./../../PhysX/src/particles/NpParticleSystem.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbActor.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbAggregate.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbBase.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbCloth.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbMetaData.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbParticleSystem.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbScene.cpp
PhysX_cppfiles   += ./../../PhysX/src/buffering/ScbShape.cpp
PhysX_cppfiles   += ./../../PhysX/src/cloth/NpCloth.cpp
PhysX_cppfiles   += ./../../PhysX/src/cloth/NpClothFabric.cpp
PhysX_cppfiles   += ./../../PhysX/src/cloth/NpClothParticleData.cpp
PhysX_cppfiles   += ./../../PhysXMetaData/core/src/PxAutoGeneratedMetaDataObjects.cpp
PhysX_cppfiles   += ./../../PhysXMetaData/core/src/PxMetaDataObjects.cpp

PhysX_cpp_release_dep    = $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(PhysX_cppfiles)))))
PhysX_cc_release_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.release.P, $(PhysX_ccfiles)))))
PhysX_c_release_dep      = $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(PhysX_cfiles)))))
PhysX_release_dep      = $(PhysX_cpp_release_dep) $(PhysX_cc_release_dep) $(PhysX_c_release_dep)
-include $(PhysX_release_dep)
PhysX_cpp_debug_dep    = $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(PhysX_cppfiles)))))
PhysX_cc_debug_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.debug.P, $(PhysX_ccfiles)))))
PhysX_c_debug_dep      = $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(PhysX_cfiles)))))
PhysX_debug_dep      = $(PhysX_cpp_debug_dep) $(PhysX_cc_debug_dep) $(PhysX_c_debug_dep)
-include $(PhysX_debug_dep)
PhysX_cpp_checked_dep    = $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(PhysX_cppfiles)))))
PhysX_cc_checked_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.checked.P, $(PhysX_ccfiles)))))
PhysX_c_checked_dep      = $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(PhysX_cfiles)))))
PhysX_checked_dep      = $(PhysX_cpp_checked_dep) $(PhysX_cc_checked_dep) $(PhysX_c_checked_dep)
-include $(PhysX_checked_dep)
PhysX_cpp_profile_dep    = $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(PhysX_cppfiles)))))
PhysX_cc_profile_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.profile.P, $(PhysX_ccfiles)))))
PhysX_c_profile_dep      = $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(PhysX_cfiles)))))
PhysX_profile_dep      = $(PhysX_cpp_profile_dep) $(PhysX_cc_profile_dep) $(PhysX_c_profile_dep)
-include $(PhysX_profile_dep)
PhysX_release_hpaths    := 
PhysX_release_hpaths    += ./../../../Include/gpu
PhysX_release_hpaths    += ./../../PhysXGpu/include
PhysX_release_hpaths    += ./../../PhysX/src/device
PhysX_release_hpaths    += ./../../../Include/foundation
PhysX_release_hpaths    += ./../../foundation/include
PhysX_release_hpaths    += ./../../../Include/physxprofilesdk
PhysX_release_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_release_hpaths    += ./../../../Include/common
PhysX_release_hpaths    += ./../../../Include/geometry
PhysX_release_hpaths    += ./../../../Include/pvd
PhysX_release_hpaths    += ./../../../Include/particles
PhysX_release_hpaths    += ./../../../Include/cloth
PhysX_release_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_release_hpaths    += ./../../../Include/pxtask
PhysX_release_hpaths    += ./../../../Include
PhysX_release_hpaths    += ./../../Common/src
PhysX_release_hpaths    += ./../../GeomUtils/headers
PhysX_release_hpaths    += ./../../GeomUtils/src
PhysX_release_hpaths    += ./../../GeomUtils/src/contact
PhysX_release_hpaths    += ./../../GeomUtils/src/common
PhysX_release_hpaths    += ./../../GeomUtils/src/convex
PhysX_release_hpaths    += ./../../GeomUtils/src/distance
PhysX_release_hpaths    += ./../../GeomUtils/src/sweep
PhysX_release_hpaths    += ./../../GeomUtils/src/gjk
PhysX_release_hpaths    += ./../../GeomUtils/src/intersection
PhysX_release_hpaths    += ./../../GeomUtils/src/mesh
PhysX_release_hpaths    += ./../../GeomUtils/src/hf
PhysX_release_hpaths    += ./../../GeomUtils/src/pcm
PhysX_release_hpaths    += ./../../LowLevel/API/include
PhysX_release_hpaths    += ./../../PhysX/src
PhysX_release_hpaths    += ./../../PhysX/src/buffering
PhysX_release_hpaths    += ./../../PhysX/src/particles
PhysX_release_hpaths    += ./../../PhysX/src/cloth
PhysX_release_hpaths    += ./../../SimulationController/include
PhysX_release_hpaths    += ./../../PhysXCooking/src
PhysX_release_hpaths    += ./../../PhysXCooking/src/mesh
PhysX_release_hpaths    += ./../../PhysXCooking/src/convex
PhysX_release_hpaths    += ./../../SceneQuery
PhysX_release_hpaths    += ./../../PvdRuntime/src
PhysX_release_hpaths    += ./../../PhysXMetaData/core/include
PhysX_release_lpaths    := 
PhysX_release_lpaths    += ./../../../Lib/linux64
PhysX_release_defines   := $(PhysX_custom_defines)
PhysX_release_defines   += PX_PHYSX_STATIC_LIB
PhysX_release_defines   += NDEBUG
PhysX_release_libraries := 
PhysX_release_common_cflags	:= $(PhysX_custom_cflags)
PhysX_release_common_cflags    += -MMD
PhysX_release_common_cflags    += $(addprefix -D, $(PhysX_release_defines))
PhysX_release_common_cflags    += $(addprefix -I, $(PhysX_release_hpaths))
PhysX_release_common_cflags  += -m64
PhysX_release_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
PhysX_release_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
PhysX_release_common_cflags  += -Wno-long-long
PhysX_release_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized -Wno-attributes -Wno-unused-local-typedefs
PhysX_release_common_cflags  += -Wno-unused-parameter -Wno-missing-field-initializers -Wno-ignored-qualifiers
PhysX_release_common_cflags  += -O3 -fno-strict-aliasing
PhysX_release_cflags	:= $(PhysX_release_common_cflags)
PhysX_release_cppflags	:= $(PhysX_release_common_cflags)
PhysX_release_lflags    := $(PhysX_custom_lflags)
PhysX_release_lflags    += $(addprefix -L, $(PhysX_release_lpaths))
PhysX_release_lflags    += -Wl,--start-group $(addprefix -l, $(PhysX_release_libraries)) -Wl,--end-group
PhysX_release_lflags  += -lrt
PhysX_release_lflags  += -m64
PhysX_release_objsdir  = $(OBJS_DIR)/PhysX_release
PhysX_release_cpp_o    = $(addprefix $(PhysX_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(PhysX_cppfiles)))))
PhysX_release_cc_o    = $(addprefix $(PhysX_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(PhysX_ccfiles)))))
PhysX_release_c_o      = $(addprefix $(PhysX_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(PhysX_cfiles)))))
PhysX_release_obj      = $(PhysX_release_cpp_o) $(PhysX_release_cc_o) $(PhysX_release_c_o)
PhysX_release_bin      := ./../../../Lib/linux64/libPhysX3.a

clean_PhysX_release: 
	@$(ECHO) clean PhysX release
	@$(RMDIR) $(PhysX_release_objsdir)
	@$(RMDIR) $(PhysX_release_bin)
	@$(RMDIR) $(DEPSDIR)/PhysX/release

build_PhysX_release: postbuild_PhysX_release
postbuild_PhysX_release: mainbuild_PhysX_release
mainbuild_PhysX_release: prebuild_PhysX_release $(PhysX_release_bin)
prebuild_PhysX_release:

$(PhysX_release_bin): $(PhysX_release_obj) build_LowLevel_release build_LowLevelCloth_release build_PhysXProfileSDK_release build_PhysXVisualDebuggerSDK_release build_PvdRuntime_release build_PxTask_release build_SceneQuery_release build_SimulationController_release 
	mkdir -p `dirname ./../../../Lib/linux64/libPhysX3.a`
	@$(AR) rcs $(PhysX_release_bin) $(PhysX_release_obj)
	$(ECHO) building $@ complete!

PhysX_release_DEPDIR = $(dir $(@))/$(*F)
$(PhysX_release_cpp_o): $(PhysX_release_objsdir)/%.o:
	$(ECHO) PhysX: compiling release $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_release_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cppfiles))))))
	cp $(PhysX_release_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  rm -f $(PhysX_release_DEPDIR).d

$(PhysX_release_cc_o): $(PhysX_release_objsdir)/%.o:
	$(ECHO) PhysX: compiling release $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_release_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_ccfiles))))))
	cp $(PhysX_release_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_ccfiles))))).release.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_ccfiles))))).release.P; \
	  rm -f $(PhysX_release_DEPDIR).d

$(PhysX_release_c_o): $(PhysX_release_objsdir)/%.o:
	$(ECHO) PhysX: compiling release $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(PhysX_release_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cfiles))))))
	cp $(PhysX_release_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_release_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  rm -f $(PhysX_release_DEPDIR).d

PhysX_debug_hpaths    := 
PhysX_debug_hpaths    += ./../../../Include/gpu
PhysX_debug_hpaths    += ./../../PhysXGpu/include
PhysX_debug_hpaths    += ./../../PhysX/src/device
PhysX_debug_hpaths    += ./../../../Include/foundation
PhysX_debug_hpaths    += ./../../foundation/include
PhysX_debug_hpaths    += ./../../../Include/physxprofilesdk
PhysX_debug_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_debug_hpaths    += ./../../../Include/common
PhysX_debug_hpaths    += ./../../../Include/geometry
PhysX_debug_hpaths    += ./../../../Include/pvd
PhysX_debug_hpaths    += ./../../../Include/particles
PhysX_debug_hpaths    += ./../../../Include/cloth
PhysX_debug_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_debug_hpaths    += ./../../../Include/pxtask
PhysX_debug_hpaths    += ./../../../Include
PhysX_debug_hpaths    += ./../../Common/src
PhysX_debug_hpaths    += ./../../GeomUtils/headers
PhysX_debug_hpaths    += ./../../GeomUtils/src
PhysX_debug_hpaths    += ./../../GeomUtils/src/contact
PhysX_debug_hpaths    += ./../../GeomUtils/src/common
PhysX_debug_hpaths    += ./../../GeomUtils/src/convex
PhysX_debug_hpaths    += ./../../GeomUtils/src/distance
PhysX_debug_hpaths    += ./../../GeomUtils/src/sweep
PhysX_debug_hpaths    += ./../../GeomUtils/src/gjk
PhysX_debug_hpaths    += ./../../GeomUtils/src/intersection
PhysX_debug_hpaths    += ./../../GeomUtils/src/mesh
PhysX_debug_hpaths    += ./../../GeomUtils/src/hf
PhysX_debug_hpaths    += ./../../GeomUtils/src/pcm
PhysX_debug_hpaths    += ./../../LowLevel/API/include
PhysX_debug_hpaths    += ./../../PhysX/src
PhysX_debug_hpaths    += ./../../PhysX/src/buffering
PhysX_debug_hpaths    += ./../../PhysX/src/particles
PhysX_debug_hpaths    += ./../../PhysX/src/cloth
PhysX_debug_hpaths    += ./../../SimulationController/include
PhysX_debug_hpaths    += ./../../PhysXCooking/src
PhysX_debug_hpaths    += ./../../PhysXCooking/src/mesh
PhysX_debug_hpaths    += ./../../PhysXCooking/src/convex
PhysX_debug_hpaths    += ./../../SceneQuery
PhysX_debug_hpaths    += ./../../PvdRuntime/src
PhysX_debug_hpaths    += ./../../PhysXMetaData/core/include
PhysX_debug_lpaths    := 
PhysX_debug_lpaths    += ./../../../Lib/linux64
PhysX_debug_defines   := $(PhysX_custom_defines)
PhysX_debug_defines   += PX_PHYSX_STATIC_LIB
PhysX_debug_defines   += _DEBUG
PhysX_debug_defines   += PX_DEBUG
PhysX_debug_defines   += PX_CHECKED
PhysX_debug_defines   += PX_SUPPORT_VISUAL_DEBUGGER
PhysX_debug_defines   += PX_PHYSX_DLL_NAME_POSTFIX=DEBUG
PhysX_debug_libraries := 
PhysX_debug_common_cflags	:= $(PhysX_custom_cflags)
PhysX_debug_common_cflags    += -MMD
PhysX_debug_common_cflags    += $(addprefix -D, $(PhysX_debug_defines))
PhysX_debug_common_cflags    += $(addprefix -I, $(PhysX_debug_hpaths))
PhysX_debug_common_cflags  += -m64
PhysX_debug_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
PhysX_debug_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
PhysX_debug_common_cflags  += -Wno-long-long
PhysX_debug_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized -Wno-attributes -Wno-unused-local-typedefs
PhysX_debug_common_cflags  += -Wno-unused-parameter -Wno-missing-field-initializers -Wno-ignored-qualifiers
PhysX_debug_common_cflags  += -g3 -gdwarf-2
PhysX_debug_cflags	:= $(PhysX_debug_common_cflags)
PhysX_debug_cppflags	:= $(PhysX_debug_common_cflags)
PhysX_debug_lflags    := $(PhysX_custom_lflags)
PhysX_debug_lflags    += $(addprefix -L, $(PhysX_debug_lpaths))
PhysX_debug_lflags    += -Wl,--start-group $(addprefix -l, $(PhysX_debug_libraries)) -Wl,--end-group
PhysX_debug_lflags  += -lrt
PhysX_debug_lflags  += -m64
PhysX_debug_objsdir  = $(OBJS_DIR)/PhysX_debug
PhysX_debug_cpp_o    = $(addprefix $(PhysX_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(PhysX_cppfiles)))))
PhysX_debug_cc_o    = $(addprefix $(PhysX_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(PhysX_ccfiles)))))
PhysX_debug_c_o      = $(addprefix $(PhysX_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(PhysX_cfiles)))))
PhysX_debug_obj      = $(PhysX_debug_cpp_o) $(PhysX_debug_cc_o) $(PhysX_debug_c_o)
PhysX_debug_bin      := ./../../../Lib/linux64/libPhysX3DEBUG.a

clean_PhysX_debug: 
	@$(ECHO) clean PhysX debug
	@$(RMDIR) $(PhysX_debug_objsdir)
	@$(RMDIR) $(PhysX_debug_bin)
	@$(RMDIR) $(DEPSDIR)/PhysX/debug

build_PhysX_debug: postbuild_PhysX_debug
postbuild_PhysX_debug: mainbuild_PhysX_debug
mainbuild_PhysX_debug: prebuild_PhysX_debug $(PhysX_debug_bin)
prebuild_PhysX_debug:

$(PhysX_debug_bin): $(PhysX_debug_obj) build_LowLevel_debug build_LowLevelCloth_debug build_PhysXProfileSDK_debug build_PhysXVisualDebuggerSDK_debug build_PvdRuntime_debug build_PxTask_debug build_SceneQuery_debug build_SimulationController_debug 
	mkdir -p `dirname ./../../../Lib/linux64/libPhysX3DEBUG.a`
	@$(AR) rcs $(PhysX_debug_bin) $(PhysX_debug_obj)
	$(ECHO) building $@ complete!

PhysX_debug_DEPDIR = $(dir $(@))/$(*F)
$(PhysX_debug_cpp_o): $(PhysX_debug_objsdir)/%.o:
	$(ECHO) PhysX: compiling debug $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_debug_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cppfiles))))))
	cp $(PhysX_debug_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  rm -f $(PhysX_debug_DEPDIR).d

$(PhysX_debug_cc_o): $(PhysX_debug_objsdir)/%.o:
	$(ECHO) PhysX: compiling debug $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_debug_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_ccfiles))))))
	cp $(PhysX_debug_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_ccfiles))))).debug.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_ccfiles))))).debug.P; \
	  rm -f $(PhysX_debug_DEPDIR).d

$(PhysX_debug_c_o): $(PhysX_debug_objsdir)/%.o:
	$(ECHO) PhysX: compiling debug $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(PhysX_debug_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cfiles))))))
	cp $(PhysX_debug_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_debug_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  rm -f $(PhysX_debug_DEPDIR).d

PhysX_checked_hpaths    := 
PhysX_checked_hpaths    += ./../../../Include/gpu
PhysX_checked_hpaths    += ./../../PhysXGpu/include
PhysX_checked_hpaths    += ./../../PhysX/src/device
PhysX_checked_hpaths    += ./../../../Include/foundation
PhysX_checked_hpaths    += ./../../foundation/include
PhysX_checked_hpaths    += ./../../../Include/physxprofilesdk
PhysX_checked_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_checked_hpaths    += ./../../../Include/common
PhysX_checked_hpaths    += ./../../../Include/geometry
PhysX_checked_hpaths    += ./../../../Include/pvd
PhysX_checked_hpaths    += ./../../../Include/particles
PhysX_checked_hpaths    += ./../../../Include/cloth
PhysX_checked_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_checked_hpaths    += ./../../../Include/pxtask
PhysX_checked_hpaths    += ./../../../Include
PhysX_checked_hpaths    += ./../../Common/src
PhysX_checked_hpaths    += ./../../GeomUtils/headers
PhysX_checked_hpaths    += ./../../GeomUtils/src
PhysX_checked_hpaths    += ./../../GeomUtils/src/contact
PhysX_checked_hpaths    += ./../../GeomUtils/src/common
PhysX_checked_hpaths    += ./../../GeomUtils/src/convex
PhysX_checked_hpaths    += ./../../GeomUtils/src/distance
PhysX_checked_hpaths    += ./../../GeomUtils/src/sweep
PhysX_checked_hpaths    += ./../../GeomUtils/src/gjk
PhysX_checked_hpaths    += ./../../GeomUtils/src/intersection
PhysX_checked_hpaths    += ./../../GeomUtils/src/mesh
PhysX_checked_hpaths    += ./../../GeomUtils/src/hf
PhysX_checked_hpaths    += ./../../GeomUtils/src/pcm
PhysX_checked_hpaths    += ./../../LowLevel/API/include
PhysX_checked_hpaths    += ./../../PhysX/src
PhysX_checked_hpaths    += ./../../PhysX/src/buffering
PhysX_checked_hpaths    += ./../../PhysX/src/particles
PhysX_checked_hpaths    += ./../../PhysX/src/cloth
PhysX_checked_hpaths    += ./../../SimulationController/include
PhysX_checked_hpaths    += ./../../PhysXCooking/src
PhysX_checked_hpaths    += ./../../PhysXCooking/src/mesh
PhysX_checked_hpaths    += ./../../PhysXCooking/src/convex
PhysX_checked_hpaths    += ./../../SceneQuery
PhysX_checked_hpaths    += ./../../PvdRuntime/src
PhysX_checked_hpaths    += ./../../PhysXMetaData/core/include
PhysX_checked_lpaths    := 
PhysX_checked_lpaths    += ./../../../Lib/linux64
PhysX_checked_defines   := $(PhysX_custom_defines)
PhysX_checked_defines   += PX_PHYSX_STATIC_LIB
PhysX_checked_defines   += NDEBUG
PhysX_checked_defines   += PX_CHECKED
PhysX_checked_defines   += PX_SUPPORT_VISUAL_DEBUGGER
PhysX_checked_defines   += PX_PHYSX_DLL_NAME_POSTFIX=CHECKED
PhysX_checked_libraries := 
PhysX_checked_common_cflags	:= $(PhysX_custom_cflags)
PhysX_checked_common_cflags    += -MMD
PhysX_checked_common_cflags    += $(addprefix -D, $(PhysX_checked_defines))
PhysX_checked_common_cflags    += $(addprefix -I, $(PhysX_checked_hpaths))
PhysX_checked_common_cflags  += -m64
PhysX_checked_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
PhysX_checked_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
PhysX_checked_common_cflags  += -Wno-long-long
PhysX_checked_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized -Wno-attributes -Wno-unused-local-typedefs
PhysX_checked_common_cflags  += -Wno-unused-parameter -Wno-missing-field-initializers -Wno-ignored-qualifiers
PhysX_checked_common_cflags  += -g3 -gdwarf-2 -O3 -fno-strict-aliasing
PhysX_checked_cflags	:= $(PhysX_checked_common_cflags)
PhysX_checked_cppflags	:= $(PhysX_checked_common_cflags)
PhysX_checked_lflags    := $(PhysX_custom_lflags)
PhysX_checked_lflags    += $(addprefix -L, $(PhysX_checked_lpaths))
PhysX_checked_lflags    += -Wl,--start-group $(addprefix -l, $(PhysX_checked_libraries)) -Wl,--end-group
PhysX_checked_lflags  += -lrt
PhysX_checked_lflags  += -m64
PhysX_checked_objsdir  = $(OBJS_DIR)/PhysX_checked
PhysX_checked_cpp_o    = $(addprefix $(PhysX_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(PhysX_cppfiles)))))
PhysX_checked_cc_o    = $(addprefix $(PhysX_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(PhysX_ccfiles)))))
PhysX_checked_c_o      = $(addprefix $(PhysX_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(PhysX_cfiles)))))
PhysX_checked_obj      = $(PhysX_checked_cpp_o) $(PhysX_checked_cc_o) $(PhysX_checked_c_o)
PhysX_checked_bin      := ./../../../Lib/linux64/libPhysX3CHECKED.a

clean_PhysX_checked: 
	@$(ECHO) clean PhysX checked
	@$(RMDIR) $(PhysX_checked_objsdir)
	@$(RMDIR) $(PhysX_checked_bin)
	@$(RMDIR) $(DEPSDIR)/PhysX/checked

build_PhysX_checked: postbuild_PhysX_checked
postbuild_PhysX_checked: mainbuild_PhysX_checked
mainbuild_PhysX_checked: prebuild_PhysX_checked $(PhysX_checked_bin)
prebuild_PhysX_checked:

$(PhysX_checked_bin): $(PhysX_checked_obj) build_LowLevel_checked build_LowLevelCloth_checked build_PhysXProfileSDK_checked build_PhysXVisualDebuggerSDK_checked build_PvdRuntime_checked build_PxTask_checked build_SceneQuery_checked build_SimulationController_checked 
	mkdir -p `dirname ./../../../Lib/linux64/libPhysX3CHECKED.a`
	@$(AR) rcs $(PhysX_checked_bin) $(PhysX_checked_obj)
	$(ECHO) building $@ complete!

PhysX_checked_DEPDIR = $(dir $(@))/$(*F)
$(PhysX_checked_cpp_o): $(PhysX_checked_objsdir)/%.o:
	$(ECHO) PhysX: compiling checked $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_checked_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cppfiles))))))
	cp $(PhysX_checked_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  rm -f $(PhysX_checked_DEPDIR).d

$(PhysX_checked_cc_o): $(PhysX_checked_objsdir)/%.o:
	$(ECHO) PhysX: compiling checked $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_checked_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_ccfiles))))))
	cp $(PhysX_checked_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_ccfiles))))).checked.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_ccfiles))))).checked.P; \
	  rm -f $(PhysX_checked_DEPDIR).d

$(PhysX_checked_c_o): $(PhysX_checked_objsdir)/%.o:
	$(ECHO) PhysX: compiling checked $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(PhysX_checked_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cfiles))))))
	cp $(PhysX_checked_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_checked_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  rm -f $(PhysX_checked_DEPDIR).d

PhysX_profile_hpaths    := 
PhysX_profile_hpaths    += ./../../../Include/gpu
PhysX_profile_hpaths    += ./../../PhysXGpu/include
PhysX_profile_hpaths    += ./../../PhysX/src/device
PhysX_profile_hpaths    += ./../../../Include/foundation
PhysX_profile_hpaths    += ./../../foundation/include
PhysX_profile_hpaths    += ./../../../Include/physxprofilesdk
PhysX_profile_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_profile_hpaths    += ./../../../Include/common
PhysX_profile_hpaths    += ./../../../Include/geometry
PhysX_profile_hpaths    += ./../../../Include/pvd
PhysX_profile_hpaths    += ./../../../Include/particles
PhysX_profile_hpaths    += ./../../../Include/cloth
PhysX_profile_hpaths    += ./../../../Include/physxvisualdebuggersdk
PhysX_profile_hpaths    += ./../../../Include/pxtask
PhysX_profile_hpaths    += ./../../../Include
PhysX_profile_hpaths    += ./../../Common/src
PhysX_profile_hpaths    += ./../../GeomUtils/headers
PhysX_profile_hpaths    += ./../../GeomUtils/src
PhysX_profile_hpaths    += ./../../GeomUtils/src/contact
PhysX_profile_hpaths    += ./../../GeomUtils/src/common
PhysX_profile_hpaths    += ./../../GeomUtils/src/convex
PhysX_profile_hpaths    += ./../../GeomUtils/src/distance
PhysX_profile_hpaths    += ./../../GeomUtils/src/sweep
PhysX_profile_hpaths    += ./../../GeomUtils/src/gjk
PhysX_profile_hpaths    += ./../../GeomUtils/src/intersection
PhysX_profile_hpaths    += ./../../GeomUtils/src/mesh
PhysX_profile_hpaths    += ./../../GeomUtils/src/hf
PhysX_profile_hpaths    += ./../../GeomUtils/src/pcm
PhysX_profile_hpaths    += ./../../LowLevel/API/include
PhysX_profile_hpaths    += ./../../PhysX/src
PhysX_profile_hpaths    += ./../../PhysX/src/buffering
PhysX_profile_hpaths    += ./../../PhysX/src/particles
PhysX_profile_hpaths    += ./../../PhysX/src/cloth
PhysX_profile_hpaths    += ./../../SimulationController/include
PhysX_profile_hpaths    += ./../../PhysXCooking/src
PhysX_profile_hpaths    += ./../../PhysXCooking/src/mesh
PhysX_profile_hpaths    += ./../../PhysXCooking/src/convex
PhysX_profile_hpaths    += ./../../SceneQuery
PhysX_profile_hpaths    += ./../../PvdRuntime/src
PhysX_profile_hpaths    += ./../../PhysXMetaData/core/include
PhysX_profile_lpaths    := 
PhysX_profile_lpaths    += ./../../../Lib/linux64
PhysX_profile_defines   := $(PhysX_custom_defines)
PhysX_profile_defines   += PX_PHYSX_STATIC_LIB
PhysX_profile_defines   += NDEBUG
PhysX_profile_defines   += PX_PROFILE
PhysX_profile_defines   += PX_SUPPORT_VISUAL_DEBUGGER
PhysX_profile_defines   += PX_PHYSX_DLL_NAME_POSTFIX=PROFILE
PhysX_profile_libraries := 
PhysX_profile_common_cflags	:= $(PhysX_custom_cflags)
PhysX_profile_common_cflags    += -MMD
PhysX_profile_common_cflags    += $(addprefix -D, $(PhysX_profile_defines))
PhysX_profile_common_cflags    += $(addprefix -I, $(PhysX_profile_hpaths))
PhysX_profile_common_cflags  += -m64
PhysX_profile_common_cflags  += -Werror -m64 -fPIC -msse2 -mfpmath=sse -ffast-math -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden
PhysX_profile_common_cflags  += -Wall -Wextra -Wstrict-aliasing=2 -fdiagnostics-show-option
PhysX_profile_common_cflags  += -Wno-long-long
PhysX_profile_common_cflags  += -Wno-unknown-pragmas -Wno-invalid-offsetof -Wno-uninitialized -Wno-attributes -Wno-unused-local-typedefs
PhysX_profile_common_cflags  += -Wno-unused-parameter -Wno-missing-field-initializers -Wno-ignored-qualifiers
PhysX_profile_common_cflags  += -O3 -fno-strict-aliasing
PhysX_profile_cflags	:= $(PhysX_profile_common_cflags)
PhysX_profile_cppflags	:= $(PhysX_profile_common_cflags)
PhysX_profile_lflags    := $(PhysX_custom_lflags)
PhysX_profile_lflags    += $(addprefix -L, $(PhysX_profile_lpaths))
PhysX_profile_lflags    += -Wl,--start-group $(addprefix -l, $(PhysX_profile_libraries)) -Wl,--end-group
PhysX_profile_lflags  += -lrt
PhysX_profile_lflags  += -m64
PhysX_profile_objsdir  = $(OBJS_DIR)/PhysX_profile
PhysX_profile_cpp_o    = $(addprefix $(PhysX_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(PhysX_cppfiles)))))
PhysX_profile_cc_o    = $(addprefix $(PhysX_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(PhysX_ccfiles)))))
PhysX_profile_c_o      = $(addprefix $(PhysX_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(PhysX_cfiles)))))
PhysX_profile_obj      = $(PhysX_profile_cpp_o) $(PhysX_profile_cc_o) $(PhysX_profile_c_o)
PhysX_profile_bin      := ./../../../Lib/linux64/libPhysX3PROFILE.a

clean_PhysX_profile: 
	@$(ECHO) clean PhysX profile
	@$(RMDIR) $(PhysX_profile_objsdir)
	@$(RMDIR) $(PhysX_profile_bin)
	@$(RMDIR) $(DEPSDIR)/PhysX/profile

build_PhysX_profile: postbuild_PhysX_profile
postbuild_PhysX_profile: mainbuild_PhysX_profile
mainbuild_PhysX_profile: prebuild_PhysX_profile $(PhysX_profile_bin)
prebuild_PhysX_profile:

$(PhysX_profile_bin): $(PhysX_profile_obj) build_LowLevel_profile build_LowLevelCloth_profile build_PhysXProfileSDK_profile build_PhysXVisualDebuggerSDK_profile build_PvdRuntime_profile build_PxTask_profile build_SceneQuery_profile build_SimulationController_profile 
	mkdir -p `dirname ./../../../Lib/linux64/libPhysX3PROFILE.a`
	@$(AR) rcs $(PhysX_profile_bin) $(PhysX_profile_obj)
	$(ECHO) building $@ complete!

PhysX_profile_DEPDIR = $(dir $(@))/$(*F)
$(PhysX_profile_cpp_o): $(PhysX_profile_objsdir)/%.o:
	$(ECHO) PhysX: compiling profile $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_profile_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cppfiles))))))
	cp $(PhysX_profile_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cppfiles))))).P; \
	  rm -f $(PhysX_profile_DEPDIR).d

$(PhysX_profile_cc_o): $(PhysX_profile_objsdir)/%.o:
	$(ECHO) PhysX: compiling profile $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(PhysX_profile_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_ccfiles))))))
	cp $(PhysX_profile_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_ccfiles))))).profile.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_ccfiles))))).profile.P; \
	  rm -f $(PhysX_profile_DEPDIR).d

$(PhysX_profile_c_o): $(PhysX_profile_objsdir)/%.o:
	$(ECHO) PhysX: compiling profile $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(PhysX_profile_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cfiles))))))
	cp $(PhysX_profile_DEPDIR).d $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(PhysX_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/PhysX/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(PhysX_profile_objsdir),, $@))), $(PhysX_cfiles))))).P; \
	  rm -f $(PhysX_profile_DEPDIR).d

clean_PhysX:  clean_PhysX_release clean_PhysX_debug clean_PhysX_checked clean_PhysX_profile
	rm -rf $(DEPSDIR)

export VERBOSE
ifndef VERBOSE
.SILENT:
endif

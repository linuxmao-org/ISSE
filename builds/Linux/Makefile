# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "__SEPARATION_FLOAT_PRECISION__" -D "__UNIX__" -D "JUCER_LINUX_MAKE_697B860A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../sdks/juce/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -msse -msse2 -std=c++0x -fopenmp -I../../sdks/eigen/
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lpthread -lfftw3f -lgomp
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "__SEPARATION_FLOAT_PRECISION__" -D "__UNIX__" -D "JUCER_LINUX_MAKE_697B860A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../sdks/juce/modules
  TARGET := ISSE
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "__SEPARATION_FLOAT_PRECISION__" -D "__UNIX__" -D "JUCER_LINUX_MAKE_697B860A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../sdks/juce/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3 -msse -msse2 -std=c++0x -fopenmp -I../../sdks/eigen/
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lpthread -lfftw3f -lgomp
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "__SEPARATION_FLOAT_PRECISION__" -D "__UNIX__" -D "JUCER_LINUX_MAKE_697B860A=1" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../sdks/juce/modules
  TARGET := ISSE
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/SeparationEngine_d2eb08e8.o \
  $(OBJDIR)/ISSEngine_cc442500.o \
  $(OBJDIR)/PluginComponent_832e8a54.o \
  $(OBJDIR)/AudioFormatReaderLoopSource_7534cfd9.o \
  $(OBJDIR)/AudioSink_3d0c2313.o \
  $(OBJDIR)/AudioSource_480d01db.o \
  $(OBJDIR)/EnergyEnvelope_e17808f8.o \
  $(OBJDIR)/PluginThread_8bb2fddd.o \
  $(OBJDIR)/ISSFft_e94a1000.o \
  $(OBJDIR)/ISSStft_56dcc16d.o \
  $(OBJDIR)/VirtualTestUser_765627f2.o \
  $(OBJDIR)/VirtualUser_c9e85140.o \
  $(OBJDIR)/FilterGraph_dc171f9a.o \
  $(OBJDIR)/GraphEditorPanel_749e22cf.o \
  $(OBJDIR)/MainHostWindow_703df6f7.o \
  $(OBJDIR)/PluginListWindow_447a6a27.o \
  $(OBJDIR)/FilePlayerProcessor_e4475cdf.o \
  $(OBJDIR)/Layer_4f6eb35b.o \
  $(OBJDIR)/ImageLayer_25f695b6.o \
  $(OBJDIR)/VizRenderThread_cf5b59d1.o \
  $(OBJDIR)/MaskLayer_1ba8e34f.o \
  $(OBJDIR)/TrainingSelectionAction_ea3811f2.o \
  $(OBJDIR)/AddToMaskAction_95fd49a8.o \
  $(OBJDIR)/ClearMaskAction_28857f39.o \
  $(OBJDIR)/BoxBrushImgAction_5be14a74.o \
  $(OBJDIR)/ClearImageAction_176be0a4.o \
  $(OBJDIR)/PaintBrushImgAction_9679a507.o \
  $(OBJDIR)/ColorMap_49e95539.o \
  $(OBJDIR)/SpectrogramSettings_dd3a9c7a.o \
  $(OBJDIR)/Timeline_e871a681.o \
  $(OBJDIR)/TransportComp_9247fb62.o \
  $(OBJDIR)/MainToolbar_b3f2554c.o \
  $(OBJDIR)/ToolbarProgress_73bd4b12.o \
  $(OBJDIR)/BrushPanel_1dc375ea.o \
  $(OBJDIR)/AboutPage_7e5c29c6.o \
  $(OBJDIR)/ImgBinaryData_eae345f8.o \
  $(OBJDIR)/ImgToolbarButton_8a572e4a.o \
  $(OBJDIR)/ToolbarItemFact_8cdb4404.o \
  $(OBJDIR)/ToolbarSlider_695df3c6.o \
  $(OBJDIR)/AppCommandIDs_bccb2718.o \
  $(OBJDIR)/Main_36be1979.o \
  $(OBJDIR)/DocWindow_8d609b52.o \
  $(OBJDIR)/TimelineView_d890a4c6.o \
  $(OBJDIR)/MainContentComp_be93f079.o \
  $(OBJDIR)/Track_de244255.o \
  $(OBJDIR)/MultiTrack_65a07292.o \
  $(OBJDIR)/MultiPaintView_6a46446a.o \
  $(OBJDIR)/ProgramSettings_d2ba791.o \
  $(OBJDIR)/ThreadHelpers_2964b525.o \
  $(OBJDIR)/ToolbarState_4b124016.o \
  $(OBJDIR)/NewProjectMenu_c8dd4d8.o \
  $(OBJDIR)/ISSLogger_c0dcc303.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/juce_audio_basics_35b680b2.o \
  $(OBJDIR)/juce_audio_devices_27c470f0.o \
  $(OBJDIR)/juce_audio_formats_5645ba90.o \
  $(OBJDIR)/juce_audio_processors_77c0886a.o \
  $(OBJDIR)/juce_audio_utils_15f300b0.o \
  $(OBJDIR)/juce_core_9c6aad94.o \
  $(OBJDIR)/juce_data_structures_66c87a50.o \
  $(OBJDIR)/juce_events_3043008.o \
  $(OBJDIR)/juce_graphics_5581cb6c.o \
  $(OBJDIR)/juce_gui_basics_dd199ce8.o \
  $(OBJDIR)/juce_gui_extra_1f8de870.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking ISSE
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning ISSE
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

strip:
	@echo Stripping ISSE
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/SeparationEngine_d2eb08e8.o: ../../src/SeparationEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SeparationEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ISSEngine_cc442500.o: ../../src/api/ISSEngine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ISSEngine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginComponent_832e8a54.o: ../../src/PluginComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioFormatReaderLoopSource_7534cfd9.o: ../../src/AudioFormatReaderLoopSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioFormatReaderLoopSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioSink_3d0c2313.o: ../../src/AudioSink.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioSink.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioSource_480d01db.o: ../../src/AudioSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EnergyEnvelope_e17808f8.o: ../../src/EnergyEnvelope.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EnergyEnvelope.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginThread_8bb2fddd.o: ../../src/PluginThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ISSFft_e94a1000.o: ../../src/api/ISSFft.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ISSFft.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ISSStft_56dcc16d.o: ../../src/api/ISSStft.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ISSStft.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VirtualTestUser_765627f2.o: ../../src/VirtualTestUser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling VirtualTestUser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VirtualUser_c9e85140.o: ../../src/VirtualUser.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling VirtualUser.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterGraph_dc171f9a.o: ../../src/plugin/FilterGraph.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterGraph.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/GraphEditorPanel_749e22cf.o: ../../src/plugin/GraphEditorPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling GraphEditorPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainHostWindow_703df6f7.o: ../../src/plugin/MainHostWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainHostWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginListWindow_447a6a27.o: ../../src/plugin/PluginListWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginListWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilePlayerProcessor_e4475cdf.o: ../../src/FilePlayerProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilePlayerProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Layer_4f6eb35b.o: ../../src/Layer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Layer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ImageLayer_25f695b6.o: ../../src/ImageLayer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ImageLayer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/VizRenderThread_cf5b59d1.o: ../../src/VizRenderThread.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling VizRenderThread.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MaskLayer_1ba8e34f.o: ../../src/MaskLayer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MaskLayer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrainingSelectionAction_ea3811f2.o: ../../src/TrainingSelectionAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrainingSelectionAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AddToMaskAction_95fd49a8.o: ../../src/AddToMaskAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AddToMaskAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ClearMaskAction_28857f39.o: ../../src/ClearMaskAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ClearMaskAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BoxBrushImgAction_5be14a74.o: ../../src/BoxBrushImgAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BoxBrushImgAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ClearImageAction_176be0a4.o: ../../src/ClearImageAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ClearImageAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PaintBrushImgAction_9679a507.o: ../../src/PaintBrushImgAction.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PaintBrushImgAction.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ColorMap_49e95539.o: ../../src/ColorMap.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ColorMap.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SpectrogramSettings_dd3a9c7a.o: ../../src/SpectrogramSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SpectrogramSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Timeline_e871a681.o: ../../src/Timeline.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Timeline.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TransportComp_9247fb62.o: ../../src/TransportComp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TransportComp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainToolbar_b3f2554c.o: ../../src/MainToolbar.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainToolbar.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ToolbarProgress_73bd4b12.o: ../../src/ToolbarProgress.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ToolbarProgress.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BrushPanel_1dc375ea.o: ../../src/BrushPanel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BrushPanel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AboutPage_7e5c29c6.o: ../../src/AboutPage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AboutPage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ImgBinaryData_eae345f8.o: ../../src/ImgBinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ImgBinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ImgToolbarButton_8a572e4a.o: ../../src/ImgToolbarButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ImgToolbarButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ToolbarItemFact_8cdb4404.o: ../../src/ToolbarItemFact.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ToolbarItemFact.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ToolbarSlider_695df3c6.o: ../../src/ToolbarSlider.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ToolbarSlider.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AppCommandIDs_bccb2718.o: ../../src/AppCommandIDs.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AppCommandIDs.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_36be1979.o: ../../src/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DocWindow_8d609b52.o: ../../src/DocWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DocWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimelineView_d890a4c6.o: ../../src/TimelineView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TimelineView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainContentComp_be93f079.o: ../../src/MainContentComp.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainContentComp.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Track_de244255.o: ../../src/Track.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Track.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MultiTrack_65a07292.o: ../../src/MultiTrack.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MultiTrack.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MultiPaintView_6a46446a.o: ../../src/MultiPaintView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MultiPaintView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProgramSettings_d2ba791.o: ../../src/ProgramSettings.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProgramSettings.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ThreadHelpers_2964b525.o: ../../src/ThreadHelpers.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ThreadHelpers.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ToolbarState_4b124016.o: ../../src/ToolbarState.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ToolbarState.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NewProjectMenu_c8dd4d8.o: ../../src/NewProjectMenu.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling NewProjectMenu.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ISSLogger_c0dcc303.o: ../../src/ISSLogger.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ISSLogger.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_35b680b2.o: ../../sdks/juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_27c470f0.o: ../../sdks/juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5645ba90.o: ../../sdks/juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_77c0886a.o: ../../sdks/juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_15f300b0.o: ../../sdks/juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_9c6aad94.o: ../../sdks/juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_66c87a50.o: ../../sdks/juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_3043008.o: ../../sdks/juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_5581cb6c.o: ../../sdks/juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_dd199ce8.o: ../../sdks/juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_1f8de870.o: ../../sdks/juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)

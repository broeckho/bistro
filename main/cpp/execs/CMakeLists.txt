#============================================================================
# Build the Stride binary.
#============================================================================
add_executable(stride stride.cpp)
if(Qt5_FOUND)
    target_link_libraries(stride ${LIBS} libstride libgengeopop libmapviewer libguicontroller)
else()
    target_link_libraries(stride ${LIBS} libstride libgengeopop)
endif()
install(TARGETS stride DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Build the GuiLauncher binary.
#============================================================================
if(Qt5_FOUND)
    add_executable(guilauncher guilauncher.cpp)
    target_link_libraries(guilauncher libguilauncher libgengeopop libmapviewer ${QT_LIBRARIES})
    install(TARGETS guilauncher DESTINATION ${BIN_INSTALL_LOCATION})
endif()

#============================================================================
# Build the GenGeoPop binary.
#============================================================================
add_executable(gengeopop gengeopop.cpp)
target_link_libraries(gengeopop ${LIBS} libprotobuf_insrc libproto libstride libgengeopop)
install(TARGETS gengeopop DESTINATION ${BIN_INSTALL_LOCATION})

#============================================================================
# Build the MapViewer binary.
#============================================================================
if(Qt5_FOUND)
    add_executable(mapviewer mapviewer.cpp)
    target_link_libraries(mapviewer libmapviewer)
    install(TARGETS mapviewer DESTINATION ${BIN_INSTALL_LOCATION})
endif()

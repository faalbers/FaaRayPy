pybind11_add_module(faaray_bindings faaray_bindings.cpp)
target_link_libraries(faaray_bindings PRIVATE FaaRay GFA)
install(TARGETS faaray_bindings DESTINATION ${CMAKE_CURRENT_SOURCE_DIR})

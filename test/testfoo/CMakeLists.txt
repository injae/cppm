set(third_party_library)

file(GLOB_RECURSE find_file "${MODULE_PATH}/*.cmake")
foreach(file ${find_file})
        include(${file})
endforeach()

foreach(lib ${third_party_library})
    message(${lib})
endforeach()

file(GLOB SOURCE *.cpp)

add_executable(testfoo ${SOURCE})


add_test(test_foo testfoo)

install(TARGETS testfoo DESTINATION bin)

target_link_libraries(testfoo
  ${third_party_library}
)

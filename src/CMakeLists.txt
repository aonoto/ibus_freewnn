include_directories(
    ${IBUS_INCLUDE_DIRS}
)

add_executable(ibus-engine-fwnn
  main.c
  engine.c
  convert.c
  fwnnserver.c
)

include_directories(${Intl_INCLUDE_DIR})

target_link_libraries(ibus-engine-fwnn
    ${IBUS_LDFLAGS}
    ${Intl_LIBRARIES}
    ${Iconv_LIBRARIES}
    wnn
    # for libwnn bundled with older FreeWnn (< 1.1.1-a022)
    # crypt
)

install(TARGETS ibus-engine-fwnn DESTINATION ${IBUS_FWNN_LIB_DIR})

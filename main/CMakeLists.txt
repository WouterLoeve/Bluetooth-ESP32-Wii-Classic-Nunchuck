# idf_component_register(SRCS "jpg_http_stream.c"
#                     INCLUDE_DIRS ".")

idf_component_register(SRCS "main.c" "wifi.c"
    INCLUDE_DIRS ".")


#target_sources(SRCS PUBLIC "jpg_http_stream.c")
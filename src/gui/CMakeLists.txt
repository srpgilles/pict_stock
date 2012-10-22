add_library(gui_pict_stock STATIC

    main_window.cpp
    main_window.hpp
)

target_link_libraries(gui_pict_stock
    ${QT_LIBRARIES}
)


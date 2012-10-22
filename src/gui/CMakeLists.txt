add_library(gui_pict_stock STATIC

    main_window.cpp
    main_window.hpp
    tab_sort_photos.cpp
    tab_sort_photos.hpp
    tab_pict_frame.cpp
    tab_pict_frame.hpp
)

target_link_libraries(gui_pict_stock
    ${QT_LIBRARIES}
)


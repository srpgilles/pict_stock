add_library(gui_pict_stock STATIC

    main_window.cpp
    main_window_db_operations.cpp
    main_window.hpp
    tab_photographers_cameras.cpp
    tab_photographers_cameras.hpp
    tab_change_date.cpp
    tab_change_date.hpp
    tab_sort_photos.cpp
    tab_sort_photos.hpp
    tab_pict_frame.cpp
    tab_pict_frame.hpp

)

target_link_libraries(gui_pict_stock
    ${QT_LIBRARIES}
    tools
    yuni-static-core
)


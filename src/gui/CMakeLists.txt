# Let CMake handle itself all moc related stuff
set(CMAKE_AUTOMOC TRUE)

add_library(gui_pict_stock STATIC
    main_window.cpp
    main_window.hpp
    main_window.hxx        
    tab_change_date.cpp
    tab_change_date.hpp
    tab_photographers_cameras.cpp
    tab_photographers_cameras.hpp
    tab_pict_frame.cpp
    tab_pict_frame.hpp
    tab_sort_photos.cpp
    tab_sort_photos.hpp    
    prepare_database.cpp
    prepare_database.hpp
)

target_link_libraries(gui_pict_stock
    ${QT_LIBRARIES}
    tools
    yuni-static-core
)


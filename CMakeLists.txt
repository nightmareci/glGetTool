cmake_minimum_required(VERSION 3.10)
project(glGetTool LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
if(OpenGL_FOUND AND SDL2_FOUND)
	include_directories(thirdparty)

	add_executable(glGet
		glGet.c
		thirdparty/glad/glad.c
		thirdparty/glad/glad.h
		thirdparty/glad/khrplatform.h
	)

	target_link_libraries(glGet PUBLIC
		SDL2::SDL2main
		${OPENGL_LIBRARIES}
	)

   	if(UNIX)
        target_link_libraries(glGet PUBLIC SDL2::SDL2)
		include(GNUInstallDirs)
        install(TARGETS glGet DESTINATION ${CMAKE_INSTALL_BINDIR})
    else()
        if(WIN32)
            target_link_libraries(glGet PUBLIC SDL2::SDL2-static)
        else()
            target_link_libraries(glGet PUBLIC SDL2::SDL2)
        endif()
        install(TARGETS glGet DESTINATION .)
    endif()

else()
	message("Missing OpenGL and/or SDL 2.0.")
endif()



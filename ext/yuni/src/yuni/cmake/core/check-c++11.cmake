
# C++0x
check_cxx_compiler_flag("-std=c++0x" YUNI_HAS_GCC_CPP0X_SUPPORT)
if(YUNI_HAS_GCC_CPP0X_SUPPORT)
	if(MINGW)
		set(YUNI_GCC_CPP0X_FLAG "-std=gnu++0x")
	else()
		set(YUNI_GCC_CPP0X_FLAG "-std=c++0x")
	endif()
	LIBYUNI_CONFIG_CFLAG("both" "core"	"${YUNI_GCC_CPP0X_FLAG}")
endif()



# C++11 nullptr
if(YUNI_HAS_GCC_CPP0X_SUPPORT)
	if(MINGW)
		set(CMAKE_REQUIRED_FLAGS "-std=gnu++0x")
	else()
		set(CMAKE_REQUIRED_FLAGS "-std=c++0x")
	endif()
endif()
check_cxx_source_compiles("
    #include <cstddef>
	typedef decltype(nullptr) NullPtr;
	class A {}; int main() {A* a = nullptr;return 0;}" YUNI_HAS_NULLPTR)



# constexpr
if(YUNI_HAS_GCC_CPP0X_SUPPORT)
	set(CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
	set(CMAKE_REQUIRED_FLAGS "${YUNI_GCC_CPP0X_FLAG}")
	check_cxx_source_compiles("
		class Foo
		{
		public:
			constexpr static int dummy = 42;
		};
		int main() {return 0;}"
		YUNI_HAS_CONSTEXPR)
	set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE})
else()
	check_cxx_source_compiles("
	class Foo
		{
		public:
			constexpr static int dummy = 42;
		};
		int main() {return 0;}"
		YUNI_HAS_CONSTEXPR)
endif()



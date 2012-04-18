
# Macro va_copy
check_cxx_source_compiles("#include <stdarg.h>
	int main() {va_list a, b; va_copy(a, b);}" YUNI_HAS_VA_COPY)


# GCC
if (NOT MSVC)
	check_cxx_source_compiles("
		int main() {   int i = 3;   int j = __sync_add_and_fetch(&i, 1);   return 0; } " YUNI_HAS_SYNC_ADD_AND_FETCH)
endif()




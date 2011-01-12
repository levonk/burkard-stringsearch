Before building the native library, you MUST copy at least jni.h to this directory.

Try

	> locate jni.h

and copy all the include files to this directory.

To build the native library,

	> ./configure
	> make

should be enough. The library is compiled in the .libs directory.

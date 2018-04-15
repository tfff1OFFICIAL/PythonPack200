from os.path import join
from setuptools import setup, Extension

JDK_source_path = input("Where is the JDK source located? ")
JDK_include_path = input("Where is the JDK include located? ")

unpack_natives_path = join(JDK_source_path, "jdk.pack", "share", "native")

setup(
    name="unpack200",
    version="1.0",
    ext_modules=[Extension(
        "unpack200",
        [
            join(unpack_natives_path, "common-unpack", "zip.cpp"),
            "PyUnpack200.cpp",
            join(unpack_natives_path, "common-unpack", "unpack.cpp"),
            join(unpack_natives_path, "common-unpack", "bands.cpp"),
            join(unpack_natives_path, "common-unpack", "bytes.cpp"),
            join(unpack_natives_path, "common-unpack", "coding.cpp"),
            join(unpack_natives_path, "common-unpack", "utils.cpp")
        ],
        include_dirs=[
            JDK_include_path,
            join(JDK_include_path, "win32"),
            join(unpack_natives_path, "common-unpack"),
            join(unpack_natives_path, "unpack200"),
            #join(unpack_natives_path, "libunpack"),
            join(JDK_source_path, "java.base", "share", "native", "libjava"),
            join(JDK_source_path, "java.base", "windows", "native", "libjava"),
        ],
        define_macros=[("ZLIB_WINAPI",), ("NO_ZLIB", 1)]
    )]
)

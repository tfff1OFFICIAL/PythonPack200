from os.path import join
from setuptools import setup, Extension

LONG_DESC = """# PythonUnpack200
A Python wrapper for the JDK unpack200 utility.

## Usage

The unpack200.unpack function takes three parameters:
* infile: string, full path to the pack200 file to extract
* outfile: string, full path to the jar file you'd like to extract to
* remove_source: bool (optional, default False), whether to remove the pack200 file specified in <infile> after extraction is complete

For example:
```python
import unpack200

unpack200.unpack(
	r"C:\path\to\packfile",
	r"C:\path\to\outfile"
)
```

## Building

Setup.py is currently only set up for Windows at the moment - feel free to contribute a Linux or Mac PR!

To build, first download and extract the following dependencies to your PC:
* Java JDK source code, e.g. from: https://github.com/dmlloyd/openjdk
* Java JDK built

run:
```bash
python3 setup.py build
```

It will ask you two questions, enter a full path into both, e.g:
```bash
Where is the JDK source located? C:\JDK\src
Where is the JDK include located? C:\Program Files\Java\jdk1.8.0_131\include
```
It will attempt to derive the correct locations for everything from these paths, and should build the extension.

## Issues

Found an issue? Please report it either in the issues section of this repo, or to the developers of JDK. Any fix PRs would be welcome!
"""

JDK_source_path = input("Where is the JDK source located? ")
JDK_include_path = input("Where is the JDK include located? ")

unpack_natives_path = join(JDK_source_path, "jdk.pack", "share", "native")

setup(
    name="unpack200",
	author="tfff1",
	license="MIT",
	description="A Python wrapper for the JDK unpack200 utility",
	long_description=LONG_DESC,
	long_description_content_type="text/markdown",
	classifiers=[
		'License :: OSI Approved :: MIT License',
		"Programming Language :: Python :: 3.6"
	],
	keywords="pack pack200 java packfile",
	url="https://github.com/tfff1OFFICIAL/PythonUnpack200/",
	project_urls={
		"Source": "https://github.com/tfff1OFFICIAL/PythonUnpack200/"
	},
	python_requires=">=3",
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

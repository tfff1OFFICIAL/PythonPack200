# PythonUnpack200
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
```
Where is the JDK source located? C:\JDK\src
Where is the JDK include located? C:\Program Files\Java\jdk1.8.0_131\include
```
It will attempt to derive the correct locations for everything from these paths, and should build the extension.

## Issues

Found an issue? Please report it either in the issues section of this repo, or to the developers of JDK. Any fix PRs would be welcome!

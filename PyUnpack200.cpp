#define PY_SSIZE_T_CLEAN
#include "main.cpp"
#include <iostream>
#include "Python.h"

#ifndef ZLIB_WINAPI
#define ZLIB_WINAPI
#endif


// anti-zlib fix
void gunzip::start(int magic) {}
void gunzip::init(unpacker* u) {}

int unpack_wrapper_extract(char* infile, char* outfile, bool remove_source) {
    // modelled after the unpacker::run() method
    unpacker u;
    u.init(read_input_via_stdio);
    set_current_unpacker(&u);

    jar jarout;
    jarout.init(&u);

    u.redirect_stdio();

    u.infileptr = fopen(infile, "rb");
    if (u.infileptr == null) {
        //char errMsg[sizeof(infile) + 29];
        //strcpy(errMsg, "Failed to open file at path: ");
        //strcat(errMsg, infile);
        PyErr_SetString(PyExc_ValueError, "");
        return NULL;
        //std::cout << "ERROR: failed to open file" << //std::endl;
    }

    jarout.openJarFile(outfile);

    if (jarout.jarfp == null) {
        //char errMsg[sizeof(outfile) + 29];
        //strcpy(errMsg, "Failed to open file at path: ");
        //strcat(errMsg, outfile);

        PyErr_SetString(PyExc_ValueError, "errMsg");
        return NULL;
    }
    //assert(jarout.jarfp != null);

    /*char peek[4];
    int magic;
    magic = read_magic(&u, peek, (int)sizeof(peek));
    if ((magic & GZIP_MAGIC_MASK) == GZIP_MAGIC) {
        // Oops; must slap an input filter on this data.
        setup_gzin(&u);
        u.gzin->start(magic);
        u.gzin->gzcrc = 0;
        u.gzin->gzlen = 0;
        if (!u.aborting()) {
            u.start();
        }
    } else {
        u.start(peek, sizeof(peek));
    }*/
    //std::cout << JAVA_PACKAGE_MAGIC << //std::endl;

    char peek[4];
    int magic;
    magic = read_magic(&u, peek, (int)sizeof(peek));

    //std::cout << "read" << //std::endl;

    u.start(peek, sizeof(peek));

    //std::cout << "started" << //std::endl;

    // Note:  The checks to u.aborting() are necessary to gracefully
    // terminate processing when the first segment throws an error.

    for (;;) {
        if (u.aborting()) break;

        //std::cout << "unpacking a segment..." << //std::endl;

        // Each trip through this loop unpacks one segment
        // and then resets the unpacker.
        for (unpacker::file* filep; (filep = u.get_next_file()) != null; ) {
            //std::cout << "pre-writing" << //std::endl;
            if (u.aborting()) break;
            u.write_file_to_jar(filep);
            //std::cout << "done one" << //std::endl;
        }

        //std::cout << "wrote!" << //std::endl;

        if (u.aborting()) break;

        // Peek ahead for more data.
        magic = read_magic(&u, peek, (int)sizeof(peek));
        if (magic != (int)JAVA_PACKAGE_MAGIC) {
            if (magic != EOF_MAGIC) {}
                // maybe this shouldn't be ignored. It seems to work without this though!
                //u.abort("garbage after end of pack archive");
            break;   // all done
        }

        //std::cout << "resetting" << //std::endl;
        // Release all storage from parsing the old segment.
        u.reset();
        //std::cout << "reset" << //std::endl;
        // Restart, beginning with the peek-ahead.
        u.start(peek, sizeof(peek));
        //std::cout << "starting..." << //std::endl;
    }

    int status = 0;
    if (u.aborting()) {
        //std::cout << "ERROR: " << u.get_abort_message() << //std::endl;
        status = 1;
    }

    if (u.infileptr != null) {
        fclose(u.infileptr);
        u.infileptr = null;
    }

    if (!u.aborting() && remove_source) {
        remove(infile);
    }

    u.finish();
    u.free();
    set_current_unpacker(null);

    return status;

    //char* name = "unpack200";

    //char* args[] = {name, "--verbose", infile, outfile, nullptr};

    //std::cout << "exited with: " << unpacker::run(3, args) << //std::endl;
}

static PyObject * unpack_wrapper(PyObject * self, PyObject * args, PyObject * kwargs) {
    // (infile: String, outloc: String, remove_source: Boolean) -> None
    char * infile;
    char * outfile;
    int remove_source = false;
    PyObject * ret;

    static char* kwlist[] = {"infile", "outfile", "remove_source", NULL};

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|p", kwlist, &infile, &outfile, &remove_source)) {
        return NULL;
    }

    // run the actual function
    unpack_wrapper_extract(infile, outfile, remove_source);

    if (PyErr_Occurred()) {
        return NULL;
    }

    return Py_None;
}

static PyMethodDef unpack200_module_methods[] = {
        {"unpack", (PyCFunction)unpack_wrapper, METH_VARARGS | METH_KEYWORDS,
                "Unpack the pack200 file at <infile> into <outloc>. After extraction, <infile> can be deleted by specifying <remove_source>"},
        {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef unpack200module = {
        PyModuleDef_HEAD_INIT,
        "unpack200",
        "Java Unpack200 wrapper for Python",
        -1,
        unpack200_module_methods
};

PyMODINIT_FUNC PyInit_unpack200(void) {
    return PyModule_Create(&unpack200module);
}
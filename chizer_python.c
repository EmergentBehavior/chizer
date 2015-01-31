#include <Python.h>
#include "chizer.h"
 
#if 0
static PyObject *chizer_getHistogram(PyObject *self, PyObject *args){
    int i=0;
    unsigned char *buffer=NULL;
    int bytes;
    PyObject *hist_;
    long hist[256];
    for(i=0;i<256;i++) hist[i]=0;

    if (!PyArg_ParseTuple(args, "s#O", &buffer,&bytes,&hist_)){
        return NULL;
    }
    for(i=0;i<PyList_Size(hist_);i++){
        hist[i]=PyLong_AsLong(PyList_GetItem(hist_,i));
    }
    getHistogram(buffer, hist,bytes);
    for(i=0;i<256;i++){
      PyList_SetItem(hist_,i,PyLong_FromLong(hist[i]));
    }
    Py_RETURN_NONE;
}

static PyObject *chizer_calcChi(PyObject *self, PyObject *args){
    int i=0;
    int bytes;
    PyObject *hist_;
    long hist[256];
    for(i=0;i<256;i++) hist[i]=0;

    if (!PyArg_ParseTuple(args, "Oi",&hist_,&bytes)){
        return NULL;
    }
    for(i=0;i<PyList_Size(hist_);i++){
        hist[i]=PyLong_AsLong(PyList_GetItem(hist_,i));
    }
    double chi=calcChi(hist,bytes);
    return Py_BuildValue("d",chi);
}

static PyObject *chizer_getFileChi(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    double chi=getFileChi(fname);
    return Py_BuildValue("d", chi);
}
#endif

static PyObject *chizer_isFileEncrypted(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    int isEncrypted=isFileEncrypted(fname);
    return Py_BuildValue("b", isEncrypted);
}

static PyObject *chizer_isChunksEncrypted(PyObject *self, PyObject *args){
    char *fname=NULL;

    if (!PyArg_ParseTuple(args, "s", &fname)){
        return NULL;
    }
    int isEncrypted=isChunksEncrypted(fname);
    return Py_BuildValue("b", isEncrypted);
}

static PyMethodDef chizerMethods[] = {
    {"isFileEncrypted", chizer_isFileEncrypted, METH_VARARGS, "Judge file is Encrypted or not"},
    {"isChunksEncrypted", chizer_isChunksEncrypted, METH_VARARGS, "Judge chunks are Encrypted or not"},
#if 0
    {"getFileChi", chizer_getFileChi, METH_VARARGS, "get chi square distribution of file"},
    {"getHistogram", chizer_getHistogram, METH_VARARGS, "get histogram"},
    {"calcChi", chizer_calcChi, METH_VARARGS, "calculate chi"},
#endif
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef chizerModule = {
   PyModuleDef_HEAD_INIT,
   "chizer",   /* name of module */
   "judge file is Encrypted or not", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   chizerMethods
};
#endif


#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_chizer(void){
     return PyModule_Create(&chizerModule);
}
#else
void initchizer(void) {
    Py_InitModule("chizer", chizerMethods);
}
#endif

#define PY_SSIZE_T_CLEAN
#include <Python.h>

# include <math.h>  
#include "symnmf.h"


struct data_struct array_2d_to_data_structure(PyObject *data_2d_array){
    struct data_struct data;
    int i, j;
    
    data.data = (double **)calloc(PyObject_Length(data_2d_array), sizeof(double *));
    data.length = PyObject_Length(data_2d_array);
    data.dimention = PyObject_Length(PyList_GetItem(data_2d_array,0));
        
    /* initiate the clusters and set them to be the first k observations */
    for (i = 0; i < data.length; i++){
        data.data[i] = (double *)calloc(data.dimention, sizeof(double));
        for (j = 0; j < data.dimention; j++){
            data.data[i][j] = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(data_2d_array,i),j));
        }
    }
    return data;
}
PyObject *data_structure_to_array_2d(struct data_struct data){
    /* Convert back from data_struct to python 2d array */
    PyObject *data_2d_array, *row;
    int i, j;
    
    data_2d_array = PyList_New(data.length);
    
    for (i = 0; i < data.length; i++){
        row = PyList_New(data.dimention);
        for (j = 0; j < data.dimention; j++){
            PyList_SetItem(row, j, Py_BuildValue("d", data.data[i][j]));
        }
        PyList_SetItem(data_2d_array, i, row);
    }
    return data_2d_array;
}
static PyObject* symnmf_api(PyObject *self, PyObject *args)
{
    int k, n;
    
    
    PyObject *W;
    PyObject *H;
    
    struct data_struct W_struct;
    struct data_struct H_struct;
    
    struct data_struct optimezed_H;
    
    int i,j;
    
    if(!PyArg_ParseTuple(args, "OOi", &W, &H, &k)) {
        return NULL;
    }
    
    W_struct = array_2d_to_data_structure(W);
    H_struct = array_2d_to_data_structure(H);

    optimezed_H = symnmf(W.data,H.data,k,W.length);

    PyObject *H_py = data_structure_to_array_2d(optimezed_H);
    
    free_data_struct(W_struct);
    free_data_struct(H_struct);
    free_data_struct(optimezed_H);
    
    return H_py;
}
static PyObject* sym_api(PyObject *self, PyObject *args)
{
    int k, n;
    
    PyObject *X;
    
    struct data_struct X_struct;
    
    struct data_struct sym_X;
    
    int i,j;
    
    if(!PyArg_ParseTuple(args, "O", &X)) {
        return NULL;
    }
    
    X_struct = array_2d_to_data_structure(X);
    
    sym_X = sym(X_struct);
    
    /* Convert back from data_struct to python 2d array */
    PyObject *X_py = data_structure_to_array_2d(sym_X);
    
    return X_py;
}
static PyObject* ddg_api(PyObject *self, PyObject *args)
{
    int k, n;
    
    PyObject *X;
    
    struct data_struct X_struct;
    
    struct data_struct sym_X;
    
    int i,j;
    
    if(!PyArg_ParseTuple(args, "O", &X)) {
        return NULL;
    }
    
    X_struct = array_2d_to_data_structure(X);
    
    sym_X = ddg(X_struct);
    
    /* Convert back from data_struct to python 2d array */
    PyObject *X_py = data_structure_to_array_2d(sym_X);
    
    return X_py;
}
static PyObject* norm_api(PyObject *self, PyObject *args)
{
    int k, n;
    
    PyObject *X;
    
    struct data_struct X_struct;
    
    struct data_struct sym_X;
    
    int i,j;
    
    if(!PyArg_ParseTuple(args, "O", &X)) {
        return NULL;
    }
    
    X_struct = array_2d_to_data_structure(X);
    
    sym_X = norm(X_struct);
    
    /* Convert back from data_struct to python 2d array */
    PyObject *X_py = data_structure_to_array_2d(sym_X);
    
    return X_py;
}


static PyMethodDef capiMethods[] = {
    {"symnmf",
      (PyCFunction) symnmf_api,
      METH_VARARGS,
      PyDoc_STR("")},
    {"sym",
      (PyCFunction) sym_api,
      METH_VARARGS,
      PyDoc_STR("")},
    {"ddg",
      (PyCFunction) ddg_api,
      METH_VARARGS,
      PyDoc_STR("")},
    {"norm",
      (PyCFunction) norm_api,
      METH_VARARGS,
      PyDoc_STR("")},
    
    {NULL, NULL, 0, NULL}
};


static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "mysymnmfsp",
    NULL,
    -1,
    capiMethods
};

PyMODINIT_FUNC
PyInit_mysymnmfsp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m) {
        return NULL;
    }
    return m;
}

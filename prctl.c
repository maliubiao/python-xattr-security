#include <Python.h>
#include <sys/prctl.h>

PyDoc_STRVAR(prctl_capset_read_doc, "return True if the capability specified set is in the calling thread's capability bounding set or False if it is not.");

static PyObject * 
prctl_capset_read(PyObject *object, PyObject *args)
{
	unsigned long set;
	int ret;
	if (!PyArg_ParseTuple(args, "l:capset_read", &set)) {
		return NULL;
	}
	ret = prctl(PR_CAPBSET_READ, set, 0, 0, 0);
	if (ret < 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}
	if (ret == 1) {
		Py_RETURN_TRUE;
	} else {
		Py_RETURN_FALSE;
	} 
}

PyDoc_STRVAR(prctl_capset_drop_doc, "If the calling thread has the CAP_SETPCAP capability, then drop the capability specified by set from the calling thread's capability bounding set, Any Children of the calling thread will inherit the newly reduced bounding set.");


static PyObject * 
prctl_capset_drop(PyObject *object, PyObject *args)
{
	unsigned long set;
	int ret;
	if (!PyArg_ParseTuple(args, "l:capset_set", &set)) {
		return NULL;
	}
	ret = prctl(PR_CAPBSET_DROP, set, 0, 0, 0);
	if (ret < 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}
	Py_RETURN_NONE; 
}


static PyMethodDef prctl_methods[] = {
	{"capset_read", (PyCFunction)prctl_capset_read,
		METH_VARARGS, prctl_capset_read_doc},
	{"capset_drop", (PyCFunction)prctl_capset_drop,
		METH_VARARGS, prctl_capset_drop_doc},
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initprctl(void)
{
	PyObject *m;
	m = Py_InitModule("prctl", prctl_methods);
	if (m != NULL) {
		//
	}
} 


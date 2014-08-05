#include <Python.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/capability.h>

PyDoc_STRVAR(cap_capget_doc, "get capability of pid");

static PyObject *
cap_capget(PyObject *object, PyObject *args)
{
	int pid = 0; 
	int ret = 0;
	struct __user_cap_data_struct *cap_data = NULL; 
	struct __user_cap_header_struct cap_header;
	if (!PyArg_ParseTuple(args, "i:capget", &pid)) {
		return NULL;
	}
	cap_header.version = _LINUX_CAPABILITY_VERSION_3;
	cap_header.pid = pid;
	cap_data = PyMem_Malloc(_LINUX_CAPABILITY_U32S_3 * sizeof(struct __user_cap_data_struct));

	if (cap_data == NULL) {
		PyErr_SetString(PyExc_OSError, "failed to allocate memory");
		return NULL;
	}

	ret = syscall(SYS_capget, &cap_header, cap_data);

	if (ret != 0) { 
		PyErr_SetFromErrno(PyExc_OSError);
		PyMem_Free(cap_data);
		return NULL; 
	}

	unsigned i = 0; 
	PyObject *cap_ret_list = PyList_New(0);
	for (i = 0; i < _LINUX_CAPABILITY_U32S_3; i++) {
		PyObject *cap_dict = PyDict_New();
		PyDict_SetItemString(cap_dict, "effective",
				PyLong_FromUnsignedLong((cap_data + i)->effective));
		PyDict_SetItemString(cap_dict, "permitted",
				PyLong_FromUnsignedLong((cap_data + i)->permitted));
		PyDict_SetItemString(cap_dict, "inheritable",
				PyLong_FromUnsignedLong((cap_data +i)->inheritable));
		PyList_Append(cap_ret_list, cap_dict);

	}
	return cap_ret_list; 
}



PyDoc_STRVAR(cap_capset_doc, "set capabilities of thread(s)");

static PyObject *
cap_capset(PyObject *object, PyObject *args)
{
	int pid = 0; 
	int ret = 0;
	PyObject *sets = NULL; 
	PyObject *set = NULL; 
	PyObject *effective = NULL;
	PyObject *permitted = NULL;
	PyObject *inheritable = NULL;
	struct __user_cap_data_struct *cap_data = NULL; 
	struct __user_cap_header_struct cap_header;

	if (!PyArg_ParseTuple(args, "iO:capset", &pid, &sets)) {
		return NULL;
	}

	if ((sets == NULL) || (! PyList_Check(sets))) {
		PyErr_SetString(PyExc_TypeError, "require a list");
	} 


	cap_header.version = _LINUX_CAPABILITY_VERSION_3;
	cap_header.pid = pid;
	cap_data = PyMem_Malloc(_LINUX_CAPABILITY_U32S_3 * sizeof(struct __user_cap_data_struct));

	if (cap_data == NULL) {
		PyErr_SetString(PyExc_OSError, "failed to allocate memory");
		return NULL;
	}

	unsigned i;
	for (i = 0; i < _LINUX_CAPABILITY_U32S_3; i++) {
		if ((set = PyList_GetItem(sets, i)) == NULL) {
			PyErr_SetString(PyExc_ValueError, "need two sets in list");
			return NULL;
		} 
		effective = PyDict_GetItemString(set, "effective");
		permitted = PyDict_GetItemString(set, "permitted");
		inheritable = PyDict_GetItemString(set, "inheritable");
		if (!(effective && permitted && inheritable)) { 
			PyMem_Free(cap_data);
			PyErr_SetString(PyExc_KeyError,
					"required keys in sets: effective, permitted, inheritable");	
			return NULL;
		}
		if (!(PyLong_Check(effective) &&
					PyLong_Check(permitted) &&
					PyLong_Check(inheritable))) {
			PyMem_Free(cap_data);
			PyErr_SetString(PyExc_KeyError,
					"effective, permitted, inheritable must be long"); 
			return NULL;
		}
		cap_data[i].effective = PyLong_AsUnsignedLong(effective);
		cap_data[i].permitted = PyLong_AsUnsignedLong(permitted);
		cap_data[i].inheritable = PyLong_AsUnsignedLong(inheritable);
	} 

	ret = syscall(SYS_capset, &cap_header, cap_data);
	if (ret != 0) { 
		PyErr_SetFromErrno(PyExc_OSError);
		PyMem_Free(cap_data);
		return NULL; 
	} 
	Py_RETURN_NONE; 
}

static PyMethodDef cap_methods[] = {
	{"capset", (PyCFunction)cap_capset,
		METH_VARARGS, cap_capset_doc},
	{"capget", (PyCFunction)cap_capget,
		METH_VARARGS, cap_capget_doc},
	{NULL, NULL, 0, NULL}

};


PyMODINIT_FUNC init_cap(void)
{
	PyObject *m;
	m = Py_InitModule("_cap", cap_methods);
	if (m != NULL) {
		//
	}
}

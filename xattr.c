#include <Python.h>
#include <attr/xattr.h>

#define XATTR_BUF_SIZE  1024

PyDoc_STRVAR(xattr_setxattr_doc, "call setxattr");

static PyObject *
xattr_setxattr(PyObject *object, PyObject *args)
{
	PyObject *value;
	char *path;
	char *name; 
	unsigned flags;
	int ret;

	if (!PyArg_ParseTuple(args, "ssOl:setxattr", &path, &name, &value, &flags)) {
		return NULL;
	} 
	if (!PyString_Check(value)) {
		goto check_failed; 
	} 
	ret = setxattr(path, name, PyString_AS_STRING(value), PyString_GET_SIZE(value), flags); 
	if (ret < 0) {
		goto ret_failed;
	}
	Py_RETURN_NONE; 
check_failed: 
	PyErr_SetString(PyExc_TypeError, "value: need a str");
	return NULL;
ret_failed:
	PyErr_SetFromErrno(PyExc_OSError);
	return NULL;
}



PyDoc_STRVAR(xattr_lsetxattr_doc, "call lsetxattr");

static PyObject *
xattr_lsetxattr(PyObject *object, PyObject *args)
{
	PyObject *value;
	char *path;
	char *name; 
	unsigned flags;
	int ret;

	if (!PyArg_ParseTuple(args, "ssOl:setxattr", &path, &name, &value, &flags)) {
		return NULL;
	} 
	if (!PyString_Check(value)) {
		goto check_failed;

	} 
	ret = lsetxattr(path, name, PyString_AS_STRING(value), PyString_GET_SIZE(value), flags);
	if (ret < 0) {
		goto ret_failed;
	}
	Py_RETURN_NONE; 
check_failed:
	PyErr_SetString(PyExc_TypeError, "value: need a str");
	return NULL;
ret_failed: 
	PyErr_SetFromErrno(PyExc_OSError);
	return NULL;
}

PyDoc_STRVAR(xattr_fsetxattr_doc, "call fsetxattr"); 

static PyObject *
xattr_fsetxattr(PyObject *object, PyObject *args)
{
	PyObject *value;
	int fd;
	char *name; 
	unsigned flags;
	int ret;

	if (!PyArg_ParseTuple(args, "ssOl:setxattr", &fd, &name, &value, &flags)) {
		return NULL;
	} 
	if (!PyString_Check(value)) {
		goto check_failed;
	} 
	ret = fsetxattr(fd, name, PyString_AS_STRING(value), PyString_GET_SIZE(value), flags);
	if (ret < 0) {
		goto ret_failed;
	}
	Py_RETURN_NONE; 
check_failed:
	PyErr_SetString(PyExc_TypeError, "value: need a str");
	return NULL;
ret_failed: 
	PyErr_SetFromErrno(PyExc_OSError);
	return NULL; 
}


PyDoc_STRVAR(xattr_listxattr_doc, "call listxattr"); 

static PyObject *
xattr_listxattr(PyObject *object, PyObject *args)
{
	char *path; 
	char *buf;
	PyObject *ret; 
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "s:listxattr", &path)) {
		return NULL;
	} 
	buf = PyMem_Malloc(XATTR_BUF_SIZE); 
	if (!buf) {
		goto failed;
	}
	xattr_ret = listxattr(path, buf, XATTR_BUF_SIZE);
	if (xattr_ret < 0) {
		goto free_buf;
	} 
	ret = PyString_FromStringAndSize(buf, xattr_ret); 
	PyMem_Free(buf);
	return ret;
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) {
		PyErr_SetFromErrno(PyExc_OSError);
	}
	return NULL; 
}


PyDoc_STRVAR(xattr_llistxattr_doc, "call listxattr"); 

static PyObject *
xattr_llistxattr(PyObject *object, PyObject *args)
{
	char *path; 
	char *buf;
	PyObject *ret; 
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "s:llistxattr", &path)) {
		return NULL;
	} 
	buf = PyMem_Malloc(XATTR_BUF_SIZE); 
	if (!buf) {
		goto failed;
	}
	xattr_ret = llistxattr(path, buf, XATTR_BUF_SIZE);
	if (xattr_ret < 0) {
		goto free_buf;
	} 
	ret = PyString_FromStringAndSize(buf, xattr_ret);
	PyMem_Free(buf);
	return ret;
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) {
		PyErr_SetFromErrno(PyExc_OSError);
	}
	return NULL; 
}


PyDoc_STRVAR(xattr_flistxattr_doc, "call listxattr"); 

static PyObject *
xattr_flistxattr(PyObject *object, PyObject *args)
{
	int fd ; 
	char *buf;
	PyObject *ret; 
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "i:flistxattr", &fd)) {
		return NULL;
	} 
	buf = PyMem_Malloc(XATTR_BUF_SIZE); 
	if (!buf) {
		goto failed;
	}
	xattr_ret = flistxattr(fd, buf, XATTR_BUF_SIZE);
	if (xattr_ret < 0) {
		goto free_buf;
	} 
	ret = PyString_FromStringAndSize(buf, xattr_ret); 
	PyMem_Free(buf);
	return ret;
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) {
		PyErr_SetFromErrno(PyExc_OSError);
	}
	return NULL; 
}

PyDoc_STRVAR(xattr_removexattr_doc, "call removexattr");

static PyObject *
xattr_removexattr(PyObject *object, PyObject *args)
{
	char *path;
	char *name;
	int ret;
	if (!PyArg_ParseTuple(args, "ss:removexattr", &path, &name)) {
		return NULL;
	}
	ret = removexattr(path, name);
	if (ret < 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}
	Py_RETURN_NONE;	
}
	

PyDoc_STRVAR(xattr_lremovexattr_doc, "call lremovexattr");

static PyObject *
xattr_lremovexattr(PyObject *object, PyObject *args)
{
	char *path;
	char *name;
	int ret;
	if (!PyArg_ParseTuple(args, "ss:lremovexattr", &path, &name)) {
		return NULL;
	}
	ret = lremovexattr(path, name);
	if (ret < 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}
	Py_RETURN_NONE;	
}

PyDoc_STRVAR(xattr_fremovexattr_doc, "call fremovexattr");

static PyObject *
xattr_fremovexattr(PyObject *object, PyObject *args)
{
	int fd;
	char *name;
	int ret;
	if (!PyArg_ParseTuple(args, "is:fremovexattr", &fd, &name)) {
		return NULL;
	}
	ret = fremovexattr(fd, name);
	if (ret < 0) {
		PyErr_SetFromErrno(PyExc_OSError);
		return NULL;
	}
	Py_RETURN_NONE;	
}


PyDoc_STRVAR(xattr_getxattr_doc, "call getxattr"); 

static PyObject *
xattr_getxattr(PyObject *object, PyObject *args)
{ 
	char *path;
	char *name;
	char *buf;
	PyObject *ret;
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "ss:getxattr", &path, &name)) {
		return NULL;
	}
	buf = PyMem_Malloc(XATTR_BUF_SIZE);	
	if (buf == NULL) {
		goto failed;
	}
	memset(buf, 0, XATTR_BUF_SIZE);
	xattr_ret = getxattr(path, name, buf, XATTR_BUF_SIZE - 1);
	if (xattr_ret < 0) {
		goto free_buf;
	}
	ret = PyString_FromStringAndSize(buf, xattr_ret);
	PyMem_Free(buf);
	return ret;
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) {
		PyErr_SetFromErrno(PyExc_OSError);
	} 
	return NULL;
}


PyDoc_STRVAR(xattr_lgetxattr_doc, "call lgetxattr"); 

static PyObject *
xattr_lgetxattr(PyObject *object, PyObject *args)
{ 
	char *path;
	char *name;
	char *buf;
	PyObject *ret;
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "ss:lgetxattr", &path, &name)) {
		return NULL;
	}
	buf = PyMem_Malloc(XATTR_BUF_SIZE);	
	if (buf == NULL) {
		goto failed;
	}
	memset(buf, 0, XATTR_BUF_SIZE);
	xattr_ret = lgetxattr(path, name, buf, XATTR_BUF_SIZE - 1);
	if (ret < 0) {
		goto free_buf;
	}
	ret = PyString_FromStringAndSize(buf, xattr_ret);
	PyMem_Free(buf);
	return ret; 
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) { 
		PyErr_SetFromErrno(PyExc_OSError);
	}
	return NULL;
}

PyDoc_STRVAR(xattr_fgetxattr_doc, "call fgetxattr"); 

static PyObject *
xattr_fgetxattr(PyObject *object, PyObject *args)
{ 
	int fd;
	char *name;
	char *buf;
	PyObject *ret;
	int xattr_ret;

	if (!PyArg_ParseTuple(args, "is:fgetxattr", &fd, &name)) {
		return NULL;
	}
	buf = PyMem_Malloc(XATTR_BUF_SIZE);	
	if (buf == NULL) {
		goto failed;
	}
	memset(buf, 0, XATTR_BUF_SIZE);
	xattr_ret = fgetxattr(fd, name, buf, XATTR_BUF_SIZE - 1);
	if (ret < 0) {
		goto free_buf;
	}
	ret = PyString_FromStringAndSize(buf, xattr_ret);
	PyMem_Free(buf);
	return ret; 
free_buf:
	PyMem_Free(buf);
failed:
	if (!PyErr_Occurred()) {
		PyErr_SetFromErrno(PyExc_OSError);	
	} 
	return NULL;
}

#undef XATTR_BUF_SIZE

static PyMethodDef xattr_methods[] = { 
	{"setxattr", (PyCFunction)xattr_setxattr,
		METH_VARARGS, xattr_setxattr_doc},
	{"lsetxattr", (PyCFunction)xattr_lsetxattr,
		METH_VARARGS, xattr_lsetxattr_doc},
	{"fsetxattr", (PyCFunction)xattr_fsetxattr,
		METH_VARARGS, xattr_fsetxattr_doc},
	{"listxattr", (PyCFunction)xattr_listxattr,
		METH_VARARGS, xattr_listxattr_doc},
	{"llistxattr", (PyCFunction)xattr_llistxattr,
		METH_VARARGS, xattr_llistxattr_doc},
	{"flistxattr", (PyCFunction)xattr_flistxattr,
		METH_VARARGS, xattr_flistxattr_doc},
	{"removexattr", (PyCFunction)xattr_removexattr,
		METH_VARARGS, xattr_removexattr_doc},
	{"lremovexattr", (PyCFunction)xattr_lremovexattr,
		METH_VARARGS, xattr_lremovexattr_doc},
	{"fremovexattr", (PyCFunction)xattr_fremovexattr,
		METH_VARARGS, xattr_fremovexattr_doc}, 
	{"getxattr", (PyCFunction)xattr_getxattr,
		METH_VARARGS, xattr_getxattr_doc},
	{"lgetxattr", (PyCFunction)xattr_lgetxattr,
		METH_VARARGS, xattr_lgetxattr_doc},
	{"fgetxattr", (PyCFunction)xattr_fgetxattr,
		METH_VARARGS, xattr_fgetxattr_doc}, 
	{NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initxattr(void)
{
	PyObject *m;
	m = Py_InitModule("xattr", xattr_methods);
	if (m != NULL) { 
	/* setxattr contst */
	PyModule_AddObject(m, "XATTR_CREATE", PyInt_FromLong(XATTR_CREATE));
	PyModule_AddObject(m, "XATTR_REPLACE", PyInt_FromLong(XATTR_REPLACE));
	}
}
	

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Core.hpp"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>

extern struct PyObject;
typedef __int64 Py_ssize_t;

typedef void(*PythInit)();
typedef void(*PyFinalize)();
typedef void (*PyDECREF)(PyObject*);
typedef void (*PyINCREF)(PyObject*);
typedef int(*PythRun)(const char*);
typedef PyObject* (*PythUnicode)(const char*);
typedef PyObject* (*PyImport)(PyObject*);
typedef PyObject* (*PyGetAttr)(PyObject*, const char*);
typedef PyObject* (*PytCallNoArgs)(PyObject*);
typedef PyObject* (*PytCall)(PyObject*, PyObject*);
typedef PyObject* (*PyTuple)(int);
typedef int (*PyTupleSetItem)(PyObject*, Py_ssize_t, PyObject*);
typedef long (*PyLongAsLong)(PyObject*);
typedef PyObject* (*PyLongFromLong)(long);
typedef double (*PyFloatAsDouble)(PyObject*);
typedef PyObject* (*PyFloatFromDouble)(double);
typedef const char* (*PyBytesAsString)(PyObject*);
typedef PyObject* (*PyBytesFromString)(const char*);

HMODULE pyt;
PythInit Init;
PyFinalize Finit;

PyDECREF Py_DECREF;
PyINCREF Py_INCREF;

PythRun RunString;

PyImport PyImport_Import;
PythUnicode PyUnicode_DecodeFSDefault;

PyGetAttr PyObject_GetAttrString;
PytCall PyObject_CallObject;
PytCallNoArgs PyObject_CallNoArgs;
PyTuple PyTuple_New;
PyTupleSetItem PyTuple_SetItem;

PyLongAsLong PyLong_AsLong;
PyLongFromLong PyLong_FromLong;

PyFloatAsDouble PyFloat_AsDouble;
PyFloatFromDouble PyFloat_FromDouble;

PyBytesAsString PyBytes_AsString;
PyBytesFromString PyBytes_FromString;

extern "C"
{
	DllExport void Load(const char* path)
	{
		pyt = LoadLibraryA(path);

		Init = (PythInit)GetProcAddress(pyt, "Py_Initialize");
		Finit = (PyFinalize)GetProcAddress(pyt, "Py_Finalize");

		Py_INCREF = (PyINCREF)GetProcAddress(pyt, "Py_IncRef");
		Py_DECREF = (PyDECREF)GetProcAddress(pyt, "Py_DecRef");

		RunString = (PythRun)GetProcAddress(pyt, "PyRun_SimpleString");
		PyUnicode_DecodeFSDefault = (PythUnicode)GetProcAddress(pyt, "PyUnicode_DecodeFSDefault");
		PyImport_Import = (PyImport)GetProcAddress(pyt, "PyImport_Import");

		PyObject_GetAttrString = (PyGetAttr)GetProcAddress(pyt, "PyObject_GetAttrString");
		PyObject_CallObject = (PytCall)GetProcAddress(pyt, "PyObject_CallObject");
		PyObject_CallNoArgs = (PytCallNoArgs)GetProcAddress(pyt, "PyObject_CallNoArgs");

		PyTuple_New = (PyTuple)GetProcAddress(pyt, "PyTuple_New");
		PyTuple_SetItem = (PyTupleSetItem)GetProcAddress(pyt, "PyTuple_SetItem");

		PyLong_AsLong = (PyLongAsLong)GetProcAddress(pyt, "PyLong_AsLong");
		PyLong_FromLong = (PyLongFromLong)GetProcAddress(pyt, "PyLong_FromLong");

		PyFloat_AsDouble = (PyFloatAsDouble)GetProcAddress(pyt, "PyFloat_AsDouble");
		PyFloat_FromDouble = (PyFloatFromDouble)GetProcAddress(pyt, "PyFloat_FromDouble");

		PyBytes_AsString = (PyBytesAsString)GetProcAddress(pyt, "PyBytes_AsString");
		PyBytes_FromString = (PyBytesFromString)GetProcAddress(pyt, "PyBytes_FromString");
	}

	DllExport void InitPython()
	{
		Init();
	}

	DllExport void FreePython()
	{
		Finit();
	}

	DllExport void AlloateConsoleWnd()
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
	}

	DllExport void FreeConsoleWnd()
	{
		FreeConsole();
	}

	DllExport void RunPythonString(const char* string)
	{
		RunString(string);
	}

	DllExport long RunFunctionReturnLong(const char* module_name, const char* function_name, const char* format, ...)
	{
		PyObject* pName, * pModule, * pFunc;
		PyObject* pResult, * pArgs, * pValue;
		pValue = nullptr;

		pName = PyUnicode_DecodeFSDefault(module_name);

		pModule = PyImport_Import(pName);
		Py_DECREF(pName);

		pFunc = PyObject_GetAttrString(pModule, function_name);

		int argc = 0;
		int len = sizeof(format);
		for (int p = 0; p < len; p++)
		{
			if (format[p] == '%')
				argc++;
		}

		pArgs = PyTuple_New(argc);

		if (argc > 0)
		{
			int arg_p = 0;
			va_list args;
			va_start(args, format);

			for (int p = 0; p < len; p++)
			{
				if (format[p] == '%')
				{
					p++;
					if (p < len)
					{
						if (format[p] == 'd')
						{
							long val = va_arg(args, long);
							pValue = PyLong_FromLong(val);
						}
						else if (format[p] == 'f')
						{
							double val = va_arg(args, double);
							pValue = PyFloat_FromDouble(val);
						}
						else if (format[p] == 's')
						{
							char* val = va_arg(args, char*);
							pValue = PyBytes_FromString(val);
						}
						else
						{
							throw "Unsupported format!";
						}

						PyTuple_SetItem(pArgs, arg_p++, pValue);
					}
				}
			}

			va_end(args);
		}

		pResult = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);

		if (pFunc != NULL) {
			Py_DECREF(pFunc);
		}
		Py_DECREF(pModule);

		long output = PyLong_AsLong(pResult);

		return output;
	}

	DllExport double RunFunctionReturnDouble(const char* module_name, const char* function_name, const char* format, ...)
	{
		PyObject* pName, * pModule, * pFunc;
		PyObject* pResult, * pArgs, * pValue;
		pValue = nullptr;

		pName = PyUnicode_DecodeFSDefault(module_name);

		pModule = PyImport_Import(pName);
		Py_DECREF(pName);

		pFunc = PyObject_GetAttrString(pModule, function_name);

		int argc = 0;
		int len = sizeof(format);
		for (int p = 0; p < len; p++)
		{
			if (format[p] == '%')
				argc++;
		}

		pArgs = PyTuple_New(argc);

		if (argc > 0)
		{
			int arg_p = 0;
			va_list args;
			va_start(args, format);

			for (int p = 0; p < len; p++)
			{
				if (format[p] == '%')
				{
					p++;
					if (p < len)
					{
						if (format[p] == 'd')
						{
							long val = va_arg(args, long);
							pValue = PyLong_FromLong(val);
						}
						else if (format[p] == 'f')
						{
							double val = va_arg(args, double);
							pValue = PyFloat_FromDouble(val);
						}
						else if (format[p] == 's')
						{
							char* val = va_arg(args, char*);
							pValue = PyBytes_FromString(val);
						}
						else
						{
							throw "Unsupported format!";
						}

						PyTuple_SetItem(pArgs, arg_p++, pValue);
					}
				}
			}

			va_end(args);
		}

		pResult = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);

		if (pFunc != NULL) {
			Py_DECREF(pFunc);
		}
		Py_DECREF(pModule);

		double output = PyFloat_AsDouble(pResult);

		return output;
	}

	DllExport const char* RunFunctionReturnString(const char* module_name, const char* function_name, const char* format, ...)
	{
		PyObject* pName, * pModule, * pFunc;
		PyObject* pResult, * pArgs, * pValue;
		pValue = nullptr;

		pName = PyUnicode_DecodeFSDefault(module_name);

		pModule = PyImport_Import(pName);
		Py_DECREF(pName);

		pFunc = PyObject_GetAttrString(pModule, function_name);

		int argc = 0;
		int len = sizeof(format);
		for (int p = 0; p < len; p++)
		{
			if (format[p] == '%')
				argc++;
		}

		pArgs = PyTuple_New(argc);

		if (argc > 0)
		{
			int arg_p = 0;
			va_list args;
			va_start(args, format);

			for (int p = 0; p < len; p++)
			{
				if (format[p] == '%')
				{
					p++;
					if (p < len)
					{
						if (format[p] == 'd')
						{
							long val = va_arg(args, long);
							pValue = PyLong_FromLong(val);
						}
						else if (format[p] == 'f')
						{
							double val = va_arg(args, double);
							pValue = PyFloat_FromDouble(val);
						}
						else if (format[p] == 's')
						{
							char* val = va_arg(args, char*);
							pValue = PyBytes_FromString(val);
						}
						else
						{
							throw "Unsupported format!";
						}

						PyTuple_SetItem(pArgs, arg_p++, pValue);
					}
				}
			}

			va_end(args);
		}

		pResult = PyObject_CallObject(pFunc, pArgs);
		Py_DECREF(pArgs);

		if (pFunc != NULL) {
			Py_DECREF(pFunc);
		}
		Py_DECREF(pModule);

		const char* output = PyBytes_AsString(pResult);

		return output;
	}

	void Py_XDECREF(PyObject* op)
	{
		if (op != NULL) {
			Py_DECREF(op);
		}
	}
}
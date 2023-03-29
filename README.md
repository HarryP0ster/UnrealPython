# UnrealPython
Small API designed for easy access from C++ to Python. Load desired Python interpreter (link to python3x.dll file), and run python code from string or call a function from a module. Originally made to be used in Unreal Engine 5 plugin.


### void Load(const char*)
- call to initialize module 
- pass a link to python3x.dll file on your system

### void InitPython()
- call to initialize python module (Call after 'Load' and before using any other functions)

### void FreePython()
- call to free python module

### long RunFunctionReturnLong(const char*, const char*, const char*, ...)
- import python module (*.py file) and call the function with specified parameters 
- parameters 
  - python module name 
  - function name 
  - arguments format string 
  - arguments to be passed (supported argument types %d - int/long, %f - float/double, %s - char*)

### double RunFunctionReturnDouble/ const char* RunFunctionReturnString
- same as RunFunctionReturnLong, difference is in the return type

### void AlloateConsoleWnd()/FreeConsoleWnd()
- allocate/free windows cmd

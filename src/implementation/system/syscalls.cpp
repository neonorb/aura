#include <mish.h>

#include <modules/screen/screen.h>

// ----- syscalls -----

Value* printSyscallFunction(List<Value*>* arguments) {
	String message = ((StringValue*) arguments->get(0))->value;
	screen_terminal_writeString(message);

	if (arguments->size() == 2) {
		String message2 = ((StringValue*) arguments->get(1))->value;
		screen_terminal_writeString(message2);
	}

	return NULL;
}

Value* printlnSyscallFunction(List<Value*>* arguments) {
	printSyscallFunction(arguments);

	screen_terminal_writeString(L"\n\r");

	return NULL;
}

Value* clearSyscallFunction(List<Value*>* arguments) {
	screen_terminal_clear();

	return NULL;
}

Value* getTimeSyscallFunction(List<Value*>* arguments) {
	return (Value*) new StringValue(L"10 AM");
}

Value* compileAndExecuteFunction(List<Value*>* arguments) {
	String sourceCode = ((StringValue*) arguments->get(0))->value;

	Code* code = mish_compile(sourceCode);
	mish_execute(code);
	delete code;

	return NULL;
}

Value* dumpAllocatedFunction(List<Value*>* arguments) {
	dumpAllocated();

	return NULL;
}

extern bool probeLoop;
Value* exitFunction(List<Value*>* arguments) {
	probeLoop = false;

	return NULL;
}

int booleanCounter = 0;
Value* getBooleanFunction(List<Value*>* arguments) {
	if(booleanCounter == 1) {
		booleanCounter = 0;
		log(L"returning false");
		return (Value*) new BooleanValue(false);
	}
	booleanCounter++;
	log(L"returning true");
	return (Value*) new BooleanValue(true);
}

int booleanCounter2 = 0;
Value* getBoolean2Function(List<Value*>* arguments) {
	if(booleanCounter2 == 3) {
		booleanCounter2 = 0;
		return (Value*) new BooleanValue(false);
	}
	booleanCounter2++;
	return (Value*) new BooleanValue(true);
}

// ----- register & unregister syscalls -----

List<Function*> syscalls;

void registerSyscalls() {
	// __print
	List<ValueType>* printParameterTypes = new List<ValueType>();
	printParameterTypes->add(STRING_VALUE);
	Function* print = new Function(L"__print", printParameterTypes, VOID_VALUE,
			printSyscallFunction);
	syscalls.add(print);
	mish_syscalls.add(print);
	// 2
	List<ValueType>* print2ParameterTypes = new List<ValueType>();
	print2ParameterTypes->add(STRING_VALUE);
	print2ParameterTypes->add(STRING_VALUE);
	Function* print2 = new Function(L"__print", print2ParameterTypes,
			VOID_VALUE, printSyscallFunction);
	syscalls.add(print2);
	mish_syscalls.add(print2);

	// __println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function(L"__println", printlnParameterTypes,
			VOID_VALUE, printlnSyscallFunction);
	syscalls.add(println);
	mish_syscalls.add(println);
	// 2
	List<ValueType>* println2ParameterTypes = new List<ValueType>();
	println2ParameterTypes->add(STRING_VALUE);
	println2ParameterTypes->add(STRING_VALUE);
	Function* println2 = new Function(L"__println", println2ParameterTypes,
			VOID_VALUE, printlnSyscallFunction);
	syscalls.add(println2);
	mish_syscalls.add(println2);

	// __clear
	List<ValueType>* clearParameterTypes = new List<ValueType>();
	Function* clear = new Function(L"__clear", clearParameterTypes, VOID_VALUE,
			clearSyscallFunction);
	syscalls.add(clear);
	mish_syscalls.add(clear);

	// __getTime
	List<ValueType>* getTimeParameterTypes = new List<ValueType>();
	Function* getTime = new Function(L"__getTime", getTimeParameterTypes,
			STRING_VALUE, getTimeSyscallFunction);
	syscalls.add(getTime);
	mish_syscalls.add(getTime);

	// __compileAndExecute
	List<ValueType>* compileAndExecuteParameterTypes = new List<ValueType>();
	compileAndExecuteParameterTypes->add(STRING_VALUE);
	Function* compileAndExecute = new Function(L"__compileAndExecute",
			compileAndExecuteParameterTypes, VOID_VALUE,
			compileAndExecuteFunction);
	syscalls.add(compileAndExecute);
	mish_syscalls.add(compileAndExecute);

	// __dumpAllocated
	List<ValueType>* dumpAllocatedParameterTypes = new List<ValueType>();
	Function* dumpAllocated = new Function(L"__dumpAllocated",
			dumpAllocatedParameterTypes, VOID_VALUE, dumpAllocatedFunction);
	syscalls.add(dumpAllocated);
	mish_syscalls.add(dumpAllocated);

	// __exit
	List<ValueType>* exitParameterTypes = new List<ValueType>();
	Function* exit = new Function(L"__exit", exitParameterTypes, VOID_VALUE,
			exitFunction);
	syscalls.add(exit);
	mish_syscalls.add(exit);

	// __getBoolean
	List<ValueType>* getBooleanParameterTypes = new List<ValueType>();
	Function* getBoolean = new Function(L"__getBoolean",
			getBooleanParameterTypes, BOOLEAN_VALUE, getBooleanFunction);
	syscalls.add(getBoolean);
	mish_syscalls.add(getBoolean);

	// __getBoolean2
	List<ValueType>* getBoolean2ParameterTypes = new List<ValueType>();
	Function* getBoolean2 = new Function(L"__getBoolean2",
			getBoolean2ParameterTypes, BOOLEAN_VALUE, getBoolean2Function);
	syscalls.add(getBoolean2);
	mish_syscalls.add(getBoolean2);
}

void unregisterSyscalls() {
	Iterator<Function*> iterator = syscalls.iterator();
	while (iterator.hasNext()) {
		Function* function = iterator.next();
		mish_syscalls.remove(function);
		delete function;
	}

	syscalls.clear();
}

#include <mish.h>

#include <modules/screen/screen.h>

// ----- syscalls -----

Value* printFunction(List<Value*>* arguments) {
	String message = ((StringValue*) arguments->get(0))->value;
	screen_terminal_writeString(message);

	if (arguments->size() == 2) {
		String message2 = ((StringValue*) arguments->get(1))->value;
		screen_terminal_writeString(message2);
	}

	return NULL;
}

Value* printlnFunction(List<Value*>* arguments) {
	printFunction(arguments);

	screen_terminal_writeString("\n\r");

	return NULL;
}

Value* clearFunction(List<Value*>* arguments) {
	screen_terminal_clear();

	return NULL;
}

Value* getTimeFunction(List<Value*>* arguments) {
	return (Value*) new StringValue("10 AM");
}

Value* compileAndExecuteFunction(List<Value*>* arguments) {
	String sourceCode = ((StringValue*) arguments->get(0))->value;

	Code* code = mish_compile(sourceCode);
	mish_execute(code);
	delete code;

	return NULL;
}

#ifdef MEMORY_LOG
Value* dumpAllocatedFunction(List<Value*>* arguments) {
	dumpAllocated();

	return NULL;
}
#endif

extern bool probeLoop;
Value* exitFunction(List<Value*>* arguments) {
	probeLoop = false;

	return NULL;
}

int booleanCounter = 0;
Value* getBooleanFunction(List<Value*>* arguments) {
	if(booleanCounter == 1) {
		booleanCounter = 0;
		return (Value*) new BooleanValue(false);
	}
	booleanCounter++;
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
	Function* print = new Function("__print", printParameterTypes, VOID_VALUE,
			printFunction);
	syscalls.add(print);
	mish_syscalls.add(print);
	// 2
	List<ValueType>* print2ParameterTypes = new List<ValueType>();
	print2ParameterTypes->add(STRING_VALUE);
	print2ParameterTypes->add(STRING_VALUE);
	Function* print2 = new Function("__print", print2ParameterTypes,
			VOID_VALUE, printFunction);
	syscalls.add(print2);
	mish_syscalls.add(print2);

	// __println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function("__println", printlnParameterTypes,
			VOID_VALUE, printlnFunction);
	syscalls.add(println);
	mish_syscalls.add(println);
	// 2
	List<ValueType>* println2ParameterTypes = new List<ValueType>();
	println2ParameterTypes->add(STRING_VALUE);
	println2ParameterTypes->add(STRING_VALUE);
	Function* println2 = new Function("__println", println2ParameterTypes,
			VOID_VALUE, printlnFunction);
	syscalls.add(println2);
	mish_syscalls.add(println2);

	// __clear
	List<ValueType>* clearParameterTypes = new List<ValueType>();
	Function* clear = new Function("__clear", clearParameterTypes, VOID_VALUE,
			clearFunction);
	syscalls.add(clear);
	mish_syscalls.add(clear);

	// __getTime
	List<ValueType>* getTimeParameterTypes = new List<ValueType>();
	Function* getTime = new Function("__getTime", getTimeParameterTypes,
			STRING_VALUE, getTimeFunction);
	syscalls.add(getTime);
	mish_syscalls.add(getTime);

	// __compileAndExecute
	List<ValueType>* compileAndExecuteParameterTypes = new List<ValueType>();
	compileAndExecuteParameterTypes->add(STRING_VALUE);
	Function* compileAndExecute = new Function("__compileAndExecute",
			compileAndExecuteParameterTypes, VOID_VALUE,
			compileAndExecuteFunction);
	syscalls.add(compileAndExecute);
	mish_syscalls.add(compileAndExecute);

#ifdef MEMORY_LOG
	// __dumpAllocated
	List<ValueType>* dumpAllocatedParameterTypes = new List<ValueType>();
	Function* dumpAllocated = new Function("__dumpAllocated",
			dumpAllocatedParameterTypes, VOID_VALUE, dumpAllocatedFunction);
	syscalls.add(dumpAllocated);
	mish_syscalls.add(dumpAllocated);
#endif

	// __exit
	List<ValueType>* exitParameterTypes = new List<ValueType>();
	Function* exit = new Function("__exit", exitParameterTypes, VOID_VALUE,
			exitFunction);
	syscalls.add(exit);
	mish_syscalls.add(exit);

	// __getBoolean
	List<ValueType>* getBooleanParameterTypes = new List<ValueType>();
	Function* getBoolean = new Function("__getBoolean",
			getBooleanParameterTypes, BOOLEAN_VALUE, getBooleanFunction);
	syscalls.add(getBoolean);
	mish_syscalls.add(getBoolean);

	// __getBoolean2
	List<ValueType>* getBoolean2ParameterTypes = new List<ValueType>();
	Function* getBoolean2 = new Function("__getBoolean2",
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

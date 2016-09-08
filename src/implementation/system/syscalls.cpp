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

// ----- register & unregister -----

List<Function*> syscalls;

void registerSyscalls() {
	// print
	List<ValueType>* printParameterTypes = new List<ValueType>();
	printParameterTypes->add(STRING_VALUE);
	Function* print = new Function(L"__print", printSyscallFunction,
			printParameterTypes, VOID_VALUE);
	syscalls.add(print);
	mish_syscalls.add(print);
	// print2
	List<ValueType>* print2ParameterTypes = new List<ValueType>();
	print2ParameterTypes->add(STRING_VALUE);
	print2ParameterTypes->add(STRING_VALUE);
	Function* print2 = new Function(L"__print", printSyscallFunction,
			print2ParameterTypes, VOID_VALUE);
	syscalls.add(print2);
	mish_syscalls.add(print2);

	// println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function(L"__println", printlnSyscallFunction,
			printlnParameterTypes, VOID_VALUE);
	syscalls.add(println);
	mish_syscalls.add(println);
	// println2
	List<ValueType>* println2ParameterTypes = new List<ValueType>();
	println2ParameterTypes->add(STRING_VALUE);
	println2ParameterTypes->add(STRING_VALUE);
	Function* println2 = new Function(L"__println", printlnSyscallFunction,
			println2ParameterTypes, VOID_VALUE);
	syscalls.add(println2);
	mish_syscalls.add(println2);

	// clear
	List<ValueType>* clearParameterTypes = new List<ValueType>();
	Function* clear = new Function(L"__clear", clearSyscallFunction,
			clearParameterTypes, VOID_VALUE);
	syscalls.add(clear);
	mish_syscalls.add(clear);

	// getTime
	List<ValueType>* getTimeParameterTypes = new List<ValueType>();
	Function* getTime = new Function(L"__getTime", getTimeSyscallFunction,
			getTimeParameterTypes, STRING_VALUE);
	syscalls.add(getTime);
	mish_syscalls.add(getTime);

	// compileAndExecute
	List<ValueType>* compileAndExecuteParameterTypes = new List<ValueType>();
	compileAndExecuteParameterTypes->add(STRING_VALUE);
	Function* compileAndExecute = new Function(L"__compileAndExecute", compileAndExecuteFunction, compileAndExecuteParameterTypes, VOID_VALUE);
	syscalls.add(compileAndExecute);
	mish_syscalls.add(compileAndExecute);
}

void unregisterSyscalls() {
	Iterator<Function*> iterator = syscalls.iterator();
	while (iterator.hasNext()) {
		Function* function = iterator.next();
		function->parameterTypes->clear();
		mish_syscalls.remove(function);
		delete function;
	}

	syscalls.clear();
}

#include <mish.h>

#include <modules/screen/screen.h>

// ----- syscalls -----

Value* printSyscallFunction(List<Value*>* arguments) {
	String message = ((StringValue*) arguments->get(0))->value;

	screen_terminal_writeString(message);

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

// ----- register & unregister -----

List<Function*> syscalls;

void registerSyscalls() {
	// print
	List<ValueType>* printParameterTypes = new List<ValueType>();
	printParameterTypes->add(STRING_VALUE);
	Function* print = new Function(L"print", printSyscallFunction,
			printParameterTypes, VOID_VALUE);
	syscalls.add(print);
	mish_syscalls.add(print);

	// println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(STRING_VALUE);
	Function* println = new Function(L"println", printlnSyscallFunction, printlnParameterTypes, VOID_VALUE);
	syscalls.add(println);
	mish_syscalls.add(println);

	// clear
	List<ValueType>* clearParameterTypes = new List<ValueType>();
	Function* clear = new Function(L"clear", clearSyscallFunction, clearParameterTypes, VOID_VALUE);
	syscalls.add(clear);
	mish_syscalls.add(clear);

	// getTime
	List<ValueType>* getTimeParameterTypes = new List<ValueType>();
	Function* getTime = new Function(L"getTime", getTimeSyscallFunction, getTimeParameterTypes, STRING_VALUE);
	syscalls.add(getTime);
	mish_syscalls.add(getTime);
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

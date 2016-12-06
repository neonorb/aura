#include <modules/screen/screen.h>
#include <feta.h>
#include <mish.h>
#include <memory.h>

// ----- syscalls -----

Value* printFunction(List<Value*>* arguments) {
	String message = ((StringValue*) arguments->get(0))->value;
	screen_terminal_writeString(message);

	if (arguments->size() == 2) {
		String message2 = ((StringValue*) arguments->get(1))->value;
		screen_terminal_writeString(message2);
	}

	return VALUE_NOT_USED;
}

Value* printlnFunction(List<Value*>* arguments) {
	printFunction(arguments);

	screen_terminal_writeString("\n\r");

	return VALUE_NOT_USED;
}

Value* clearFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	screen_terminal_clear();

	return VALUE_NOT_USED;
}

Value* getTimeFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	return (Value*) new StringValue("10 AM");
}

Value* compileAndExecuteFunction(List<Value*>* arguments) {
	String sourceCode = ((StringValue*) arguments->get(0))->value;

	Code* code = mish::compile::compile(sourceCode);
	mish::execute::execute(code);
	delete code;

	return VALUE_NOT_USED;
}

#ifdef MEMORY_LOG
Value* dumpAllocatedFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	dumpAllocated();

	return VALUE_NOT_USED;
}
#endif

extern bool probeLoop;
Value* exitFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	probeLoop = false;

	return VALUE_NOT_USED;
}

int booleanCounter = 0;
Value* getBooleanFunction(List<Value*>* arguments) {
	UNUSED(arguments);

	if (booleanCounter == 1) {
		booleanCounter = 0;
		return (Value*) new BooleanValue(false);
	}
	booleanCounter++;
	return (Value*) new BooleanValue(true);
}

int booleanCounter2 = 0;
Value* getBoolean2Function(List<Value*>* arguments) {
	UNUSED(arguments);

	if (booleanCounter2 == 3) {
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
	printParameterTypes->add(ValueType::STRING);
	Function* print = new Function("__print", printParameterTypes,
			ValueType::VOID,
			BIND_FREE_CB(printFunction));
	syscalls.add(print);
	mish_syscalls.add(print);
	// 2
	List<ValueType>* print2ParameterTypes = new List<ValueType>();
	print2ParameterTypes->add(ValueType::STRING);
	print2ParameterTypes->add(ValueType::STRING);
	Function* print2 = new Function("__print", print2ParameterTypes,
			ValueType::VOID, BIND_FREE_CB(printFunction));
	syscalls.add(print2);
	mish_syscalls.add(print2);

	// __println
	List<ValueType>* printlnParameterTypes = new List<ValueType>();
	printlnParameterTypes->add(ValueType::STRING);
	Function* println = new Function("__println", printlnParameterTypes,
			ValueType::VOID, BIND_FREE_CB(printlnFunction));
	syscalls.add(println);
	mish_syscalls.add(println);
	// 2
	List<ValueType>* println2ParameterTypes = new List<ValueType>();
	println2ParameterTypes->add(ValueType::STRING);
	println2ParameterTypes->add(ValueType::STRING);
	Function* println2 = new Function("__println", println2ParameterTypes,
			ValueType::VOID, BIND_FREE_CB(printlnFunction));
	syscalls.add(println2);
	mish_syscalls.add(println2);

	// __clear
	List<ValueType>* clearParameterTypes = new List<ValueType>();
	Function* clear = new Function("__clear", clearParameterTypes,
			ValueType::VOID,
			BIND_FREE_CB(clearFunction));
	syscalls.add(clear);
	mish_syscalls.add(clear);

	// __getTime
	List<ValueType>* getTimeParameterTypes = new List<ValueType>();
	Function* getTime = new Function("__getTime", getTimeParameterTypes,
			ValueType::STRING, BIND_FREE_CB(getTimeFunction));
	syscalls.add(getTime);
	mish_syscalls.add(getTime);

	// __compileAndExecute
	List<ValueType>* compileAndExecuteParameterTypes = new List<ValueType>();
	compileAndExecuteParameterTypes->add(ValueType::STRING);
	Function* compileAndExecute = new Function("__compileAndExecute",
			compileAndExecuteParameterTypes, ValueType::VOID,
			BIND_FREE_CB(compileAndExecuteFunction));
	syscalls.add(compileAndExecute);
	mish_syscalls.add(compileAndExecute);

#ifdef MEMORY_LOG
	// __dumpAllocated
	List<ValueType>* dumpAllocatedParameterTypes = new List<ValueType>();
	Function* dumpAllocated = new Function("__dumpAllocated",
			dumpAllocatedParameterTypes, ValueType::VOID, dumpAllocatedFunction);
	syscalls.add(dumpAllocated);
	mish_syscalls.add(dumpAllocated);
#endif

	// __exit
	List<ValueType>* exitParameterTypes = new List<ValueType>();
	Function* exit = new Function("__exit", exitParameterTypes, ValueType::VOID,
			BIND_FREE_CB(exitFunction));
	syscalls.add(exit);
	mish_syscalls.add(exit);

	// __getBoolean
	List<ValueType>* getBooleanParameterTypes = new List<ValueType>();
	Function* getBoolean = new Function("__getBoolean",
			getBooleanParameterTypes, ValueType::BOOLEAN, BIND_FREE_CB(getBooleanFunction));
	syscalls.add(getBoolean);
	mish_syscalls.add(getBoolean);

	// __getBoolean2
	List<ValueType>* getBoolean2ParameterTypes = new List<ValueType>();
	Function* getBoolean2 = new Function("__getBoolean2",
			getBoolean2ParameterTypes, ValueType::BOOLEAN, BIND_FREE_CB(getBoolean2Function));
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

#include <mish.h>
#include <log.h>
#include <icxxabi.h>

// ----- syscalls -----

Value* printSyscallFunction(List<Value*>* arguments) {
	String toPrint = ((StringValue*) arguments->get(0))->value;
	String second = ((StringValue*) arguments->get(1))->value;

	log(toPrint);
	log(second);

	return NULL;
}

// ----- register & unregister -----

List<Function*> syscalls;

void registerSyscalls() {
	List<ValueType>* parameterTypes = new List<ValueType>();
	parameterTypes->add(STRING_VALUE);
	parameterTypes->add(STRING_VALUE);
	Function* print = new Function(L"print", printSyscallFunction,
			parameterTypes, VOID_VALUE);
	syscalls.add(print);
	mish_syscalls.add(print);
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

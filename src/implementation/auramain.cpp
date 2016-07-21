#include <memory.h>

#include <log.h>
#include <mish.h>
#include <syscall.h>
#include <value.h>

Value* printSyscallFunction(List<Value*>* arguments) {
	String toPrint = ((StringValue*) arguments->get(0))->value;

	log(toPrint);

	return 0;
}

void auraMain() {
	log("Starting Aura");

	//debug("-----------------------------");
	//dumpAllocated();
	//debug("-----------------------------");

	Function* printSyscall = new Function("print", printSyscallFunction);
	mish_addSyscall(printSyscall);

	while (true) {
		Code* code = mish_compile("__print('Hello world!')");
		code->execute();
		delete code;
	}

	mish_getSyscalls().remove(printSyscall);
	delete printSyscall;

	//debug("-----------------------------");
	//dumpAllocated();
	//debug("-----------------------------");
}

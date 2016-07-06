#include <memory.h>

#include <log.h>
#include <mish.h>
#include <syscall.h>
#include <value.h>

Value* printSyscallFunction(List<Value*>* arguments){
	String toPrint = ((StringValue*) arguments->get(0))->value;

	log(toPrint);

	return 0;
}

void auraMain() {
	log("Starting Aura");


	Syscall* printSyscall = new Syscall("print", &printSyscallFunction);
	mish_addSyscall(printSyscall);

	//while (true) {
		log("compiling");
		Code* code = mish_compile("__print('hi')");
		log("executing");
		code->execute();
		delete code->destroy();
	//}

	delete printSyscall->destroy();
}

#include <memory.h>

#include <log.h>
#include <mish.h>

void auramain() {
	log("Starting Aura");

	while (true) {
		Function* function = new Function();
		Bytecode* hi = new Bytecode(PRINTHI);
		Bytecode* bob = new Bytecode(PRINTBOB);
		if(hi == NULL){
			crash("out of memory");
		}
		function->bytecodes.add(hi);
		function->bytecodes.add(bob);
		execute(function);
		delete hi;
		delete bob;
		delete function;
	}
}

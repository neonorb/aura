#include <memory.h>

#include <log.h>
#include <mish.h>

void auramain() {
	log("Starting Aura");

	while (true) {
		Code* code = mish_compile("print(\"hi\")");
		code->execute();
		delete code->destroy();
	}
}

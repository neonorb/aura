.extern interrupt_handler

.global interrupt
interrupt:
	pushfl
	call interrupt_handler
	popfl
	leave
	iret

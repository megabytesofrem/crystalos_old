;
; Let the CBT begin
;

; Defined in sys/cpu/exceptions.hpp
extern exception_handler

%macro pushaq 0
	cld		; clear direction flag
	
	; push all registers to the stack
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popaq 0
	; pop all registers in reverse order, since on x86
	; the stack grows downwards 
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

;
; Helper macros to call exception_handler with an optional error code
;
%macro error_code 1
	; not sure why, too tired to figure out
	push qword [rsp+5*8]
	push qword [rsp+5*8]
	push qword [rsp+5*8]
	push qword [rsp+5*8]

	pushaq			; push all the registers
	mov rdi, %1		; rdi = exception code
	mov rdi, rsp	; call exception_handler and pass the registers
	mov rdx, qword [rsp+20*8] ; rdx = error code?
	call exception_handler

	popaq			; clean up after ourselves
	iretq
%endmacro

%macro no_error_code 1
	pushaq			; push all the registers
	mov rdi, %1		; rdi = exception code
	mov rsi, rsp	
	xor rdx, rdx	; clear rdx
	call exception_handler

	popaq			; clean up after ourselves
	iretq
%endmacro

section .text
; Exception handlers
global exc_div0			; Divide by Zero
global exc_debug		; Debug
global exc_nmi			; Non Maskable Interrupt
global exc_break		; Breakpoint
global exc_overflow		; Overflow
global exc_bounds		; Out of Bounds
global exc_inv_opcode	; Invalid Opcode
global exc_no_device	; No Device
global exc_double_fault	; Double Fault
global exc_inv_tss		; Invalid TSS
global exc_no_segment	; No Segment Present
global exc_ss_fault
global exc_gpf			; General Protection Fault
global exc_page_fault	; Page Fault
global exc_x87_fp		
global exc_align_check	; Alignment Check
global exc_machine_check; Machine Check
global exc_simd_fp		; SIMD Floating Point Exception
global exc_virt
global exc_security

exc_div0:
	no_error_code 0x0
exc_debug:
	no_error_code 0x1
exc_nmi:
	no_error_code 0x2
exc_break:
	no_error_code 0x3
exc_overflow:
	no_error_code 0x4
exc_bounds:
	no_error_code 0x5
exc_inv_opcode:
	no_error_code 0x6
exc_no_device:
	no_error_code 0x7
exc_double_fault:
	error_code 0x8
exc_inv_tss:
	error_code 0xa
exc_no_segment:
	error_code 0xb
exc_ss_fault:
	error_code 0xc
exc_gpf:
	error_code 0xd
exc_page_fault:
	error_code 0xe
exc_x87_fp:
	no_error_code 0x10
exc_align_check:
	error_code 0x11
exc_machine_check:
	no_error_code 0x12
exc_simd_fp:
	no_error_code 0x13
exc_virt:
	no_error_code 0x14
exc_security:
	error_code 0x1e



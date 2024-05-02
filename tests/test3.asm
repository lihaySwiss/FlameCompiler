BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	flag db 0
	he dd 0
	hi dd 0
_start:
	mov eax, 8
	mov [hi], eax
	mov eax, 5
loop_start0:
	mov edx, 0
	div word [tenDividerForPrintingNumbers]
	add edx, '0'
	cmp eax, 0
	jne loop_start0
	mov eax, 4
	mov ebx, 1
	int 0x80
	mov eax, hi
	mov eax, 1
	mov ebx, 0
	int 0x80

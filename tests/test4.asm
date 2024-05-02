BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	gg dd 0
	sun dd 0
	a dd 0
_start:
	mov eax, 83
	mov [gg], eax
	mov eax, a
	mov ebx, gg
	add eax, ebx
	mov [sun], eax
	mov eax, 1
	mov ebx, 0
	int 0x80

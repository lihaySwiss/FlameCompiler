BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	gg db 0
_start:
	mov eax, 'l'
	mov [gg], eax
	mov eax, 'v'
	mov [savedMemoryForPrinting], eax
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, savedMemoryForPrinting
	mov edx, 2
	int 0x80
	popa
	mov eax, 'v'

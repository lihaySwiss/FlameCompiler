BITS 32
section .text
global _start
section .data
	savedMemoryForPrinting db 100 dup(0)
	tenDividerForPrintingNumbers dw 10
	flag db 0
	hi db 0
	hello dd 0
_start:
	mov eax, 0
	mov [hello], eax
	mov eax, 6
	mov [hi], eax
	mov eax, "c"
	mov [flag], eax
	mov eax, 12
	mov [hello], eax
	mov eax, 0
	int 0x80

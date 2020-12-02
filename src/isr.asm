; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones
;
%include "print.mac"

BITS 32

sched_task_offset:     dd 0
sched_task_selector:   dw 0

;; PIC
extern pic_finish1
extern imprimir_excepcion
extern print_scan_code

extern desactivar_tarea

;; Sched
extern sched_next_task
extern cmpScanCode
extern modo_debug


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
     mov eax, %1
     ;push eax
     ;call imprimir_excepcion
     ;add esp, 4
     call desactivar_tarea  
     call sched_next_task            ; obtener indice de la proxima tarea a ejecutar
     mov [sched_task_selector], ax   ; carga el selector del segmento de la tarea a saltar
     jmp far [sched_task_offset]     ; intercambio de tareas
    
     jmp $

%endmacro


;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


;; Rutina de atención del RELOJ
global _isr32

_isr32:
     pushad
     ;avisar al pic que se recibio la interrupcion
     call pic_finish1
     ;imprimir el reloj de sistema
     call sched_next_task ; Crear esta funcion en C que basicamente cicle entre las tareas que hay y cuando llega a la ultima vuelva a la primera
     str cx
     cmp ax, cx           ; Me fijo si la proxima tarea no es la actual
     je .fin
     call next_clock
     mov word [sched_task_selector], ax
     
     jmp far [sched_task_offset]
     .fin:
     popad
iret

;; -------------------------------------------------------------------------- ;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
%define k_debug       0x15 ; Y

global _isr33

_isr33:
     pushad


     in al, 0x60
     cmp al, k_debug
     jne .fin
     call modo_debug
     .fin:
     ;avisar al pic que se recibio la interrupcion
     call pic_finish1
     popad
     iret

global _isr88

_isr88:
     pushad
     mov ebp, esp
     push eax
     call next_clock
     mov ax,0x80 ;idle
     mov word [sched_task_selector], ax  ; (cambiamos con nahu)
     jmp far [sched_task_offset]
     pop ebp
     popad
iret

global _isr89

_isr89:
     pushad
     mov ebp, esp
     push eax
     call next_clock
     mov ax,0x80 ;idle
     mov word [sched_task_selector], ax  ; (cambiamos con nahu)
     xchg bx, bx
     jmp far [sched_task_offset]
     pop ebp
     popad
iret

global _isr100

_isr100:
     pushad
     mov ebp, esp
     push eax
     call next_clock
     mov ax,0x80 ;idle
     mov word [sched_task_selector], ax  ; (cambiamos con nahu)
     xchg bx, bx
     jmp far [sched_task_offset]
     pop ebp
     popad
iret


global _isr123

_isr123:
     pushad
     mov ebp, esp
     push eax
     call next_clock
     mov ax,0x80 ;idle
     mov word [sched_task_selector], ax  ; (cambiamos con nahu)
     xchg bx, bx
     jmp far [sched_task_offset]
     pop ebp
     popad
iret

;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;
;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
isrNumber:           dd 0x00000000
isrClock:            db '|/-\'
next_clock:
        pushad
        inc DWORD [isrNumber]
        mov ebx, [isrNumber]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumber], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret

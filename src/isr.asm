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
extern capturar_excepcion

;; Sched
extern sched_next_task
extern cmpScanCode
extern sched_idle
extern desactivar_tarea
extern modo_debug

extern ticks_counter

;game 
extern clock_task

;; Syscalls
extern sys_meeseek
extern sys_move
extern sys_look
extern sys_use_portal_gun

extern modoDebug
extern imprimirRegistros
extern debug_mode_on_off




temp: dd 0         ; variable temporal

tempw1: db 0         ; variable temporal
tempw2: db 0         ; variable temporal

tempb1: db 0         ; variable temporal
tempb2: db 0         ; variable temporal

reg_backup: dd 0
eipActual:  dd 0
error_code: dd 0
e_flags:  dd 0

stack0: dd 0
stack2: dd 0



;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:

     xchg bx, bx
     push esp
     mov [reg_backup], eax
     
     ;Guardo el EIP
     ; push eax
     mov eax, [esp + 8] ;
     mov [eipActual], eax
     ; pop eax
     
     ; push eax
     mov eax, [esp + 4]
     mov [error_code], eax
     ; pop eax

     ; push eax
     mov eax, [esp + 16]
     mov [e_flags], eax
     ; pop eax

     ; push eax
     mov eax, [modoDebug] 
     cmp byte al, 1
     ; pop eax
     jne .fin

     ; pusheo error code
     mov eax, [error_code]
     push eax

     ;pusheo numero de excepcion
     mov eax, %1
     push eax

     mov eax, [e_flags]
     push eax

     push ss
     push gs
     push fs
     push es
     push ds
     push cs
     ; mov [reg_backup], eax
     mov eax, [eipActual]
     push eax
     mov eax, [reg_backup]
     ;push esp ; ver si no tendria que ser lo primero que se pushea, porque sino las cosas mueven el puntero a la pila
     push ebp 
     push edi
     push esi
     push edx
     push ecx
     push ebx
     push eax
     xchg bx, bx
     call imprimirRegistros
     add esp,68
     
     xchg bx, bx 


     ; si estoy en modo debug, entonces
     mov eax, [modoDebug] 
     cmp byte al, 1
     je .fin

     call desactivar_tarea  


     .fin:

     call sched_next_task            ; obtener indice de la proxima tarea a ejecutar
     mov [sched_task_selector], ax   ; carga el selector del segmento de la tarea a saltar
     jmp far [sched_task_offset]     ; intercambio de tareas


iret     

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
     call next_clock
     ;contador de ticks de para move
     call ticks_counter 
     

     call sched_next_task 
     str cx
     cmp ax, cx           ; Me fijo si la proxima tarea no es la actual
     je .fin
     mov word [sched_task_selector], ax
     jmp far [sched_task_offset]
     .fin:
     popad
iret

;; -------------------------------------------------------------------------- ;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
%define k_debug    0x15   ;  Y

global _isr33

_isr33:
     pushad 
     in al, 0x60
     cmp al, k_debug
     jne .fin
     call debug_mode_on_off
   
   .fin:
     call pic_finish1;avisar al pic que se recibio la interrupcion
     popad
     iret

global _isr88 



; _isr88:    
;      pushad
;      mov ebp, esp
     

;      push ecx       ; Código de la tarea Mr Meeseeks a ser ejecutada.
;      push ebx       ; Columna en el mapa donde crear el Mr Meeseeks.
;      push eax       ; Fila en el mapa donde crear el Mr Meeseeks

;      call sys_meeseek

;      mov [temp], eax ; mov eax a variable temporal
;      add esp,12     ; tenemos 3 parametros de entrada

     
;      call sched_idle
;      mov word [sched_task_selector], ax  
;      jmp far [sched_task_offset]

;      popad          ; recupero registros
;      mov eax, [temp] ; returneo en ceax   
;      ; pop ebp
; iret

_isr88:    
     pushad
     mov ebp, esp
     

     push ecx       ; Código de la tarea Mr Meeseeks a ser ejecutada.
     push ebx       ; Columna en el mapa donde crear el Mr Meeseeks.
     push eax       ; Fila en el mapa donde crear el Mr Meeseeks

     call sys_meeseek

     ; mov [temp], eax ; mov eax a variable temporal
     add esp,12     ; tenemos 3 parametros de entrada

     
     call sched_idle
     mov word [sched_task_selector], ax  
     jmp far [sched_task_offset]

     popad          ; recupero registros
     call take_meeseek

iret



global _isr89

_isr89:
     pushad
     mov ebp, esp

     call sys_use_portal_gun     

     call sched_idle
     mov word [sched_task_selector], ax
     jmp far [sched_task_offset]
     
     popad
iret




global _isr100


extern print_temps
extern take_look
extern take_move
extern take_meeseek

_isr100:
     pushad
     mov ebp, esp
     
     call sys_look

     call sched_idle
     mov word [sched_task_selector], ax 
     jmp far [sched_task_offset]

     popad

     push 0
     call take_look
     mov byte  [tempb1], al
     add esp, 4

     push 1
     call take_look
     mov byte  [tempb2], al
     add esp, 4

     mov byte al, [tempb1]
     mov byte bl, [tempb2]

iret



global _isr123


_isr123:
     pushad
     mov ebp,esp 
     
     push ebx
     push eax
     call sys_move
     ; mov [temp], eax ; mov eax a variable temporal
     add esp, 8

     call sched_idle
     mov word [sched_task_selector], ax  ; (cambiamos con nahu)
     jmp far [sched_task_offset]

     popad          ; recupero registros  
     call take_move
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


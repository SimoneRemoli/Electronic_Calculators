.org 0x800
.data
  .equ $pulsante_irq, 0x01
  .equ $pulsante_status, 0x02
  .equ $pulsante_reg, 0x03
  .equ $irq_timer, 0x04
  .equ $status_antenna, 0x05
  .equ $latitudine_antenna, 0x06
  .equ $longitudine_antenna, 0x07
  modalità: .byte 0  (#0=registrazione, 1=pausa)
  array: .fill 1024,8
  indice: .quad 0
  

.text
main:
  outb %al, $pulsante_status (#avvio il pulsante)
  sti 
  .loop:
  jmp .loop
  
  setting:
    push %rax (#dentro un contesto di interrupt)
    push %rdx
    movb %dil, %dl 
    outb %al, %dl (#zittisco la richiesta di interruzione)
    inb $pulsante_reg, %al
    movb %al, modalità (#in modalità abbiamo 0 o 1 )
    movb %sil, %dl
    outb %al, %dl (#riavvio il dispositivo, nuovamente pronto per essere interrotto)
    pop %rdx
    pop %rax
    ret
    
  setting2:
    push %rax
    push %rdx
    movb %dil, %dl 
    outb %al, %dl (#zittisco la richiesta di interruzione del timer)
    #------------------------------------------------------------------------------------
    #nota: timer non ha un flip flop di status poichè invia ogni 15 secondi una richiesta
    #di interruzione e pertanto non è necessario avviarlo esplicitamente.
    #------------------------------------------------------------------------------------
    cmpb $0, modalità 
    jz .registrazione 
    jnz .pausa
    .return:
      pop %rdx
      pop %rax 
      ret
    
  .registrazione:
    outb %al, $status_antenna (#richiedo all'antenna di produrre i dati)
    .bw1:
      inb $status_antenna, %al
      btb $0, %al 
      jnc .bw1
      (entrambi i dati sono stati prodotti)
      inq $latitudine_antenna, %rax
      movq indice, %rdx
      movq %rax, array(,rdx,8)
      addq $1, indice
      movq indice, %rdx
      inq $longitudine_antenna, %rax
      movq %rax, array(,rdx,8)
      addq $1, indice
      jmp .return
      
  .pausa:
    jmp .return
      
  .driver 0 (#driver pulsante)
    push %rdi
    push %rsi
    movq $pulsante_irq, %rdi (#io parametrico, rdi sarà visibile nella funzione chiamata)
    movq $pulsante_status, %rsi
    call setting
    pop %rsi
    pop %rdi
    iret
    
  .driver 1 (#driver timer)
    push %rdi
    movq $irq_timer, %rdi
    call setting2
    pop %rdi 
    iret
    
    
  

.org 0x800
.data
  .equ irq_timer, 0x01
  .equ elettrovalvola, 0x02
  .equ orario_accensione_regprogrammatore, 0x03
  .equ durata_irrigazione_regprogrammatore, 0x4
  .equ status_programmatore, 0x05
  .equ status_pluviometro, 0x06
  .equ pluviometro_mm_acqua_caduti_reg, 0x07
  orarioacc: .long 0
  duratairr: .long 0
  milliltri: .long 0
  index_orarioacc: .long 0
  index_durirr: .long 0
  flag_attivazione_elettrovalvola: .byte 0 #elettrovalvola spenta 


.text
main:
    outb %al, $status_programmatore
    .bw:
      inb $status_programmatore, %al 
      btb $0, %al 
      jnc .bw
      #i dati sono sicuramente pronti 
      inl $orario_accensione_regprogrammatore, %eax 
      movl %eax, orarioacc
      inl $durata_irrigazione_regprogrammatore, %eax
      movl %eax, duratairr
      
    #ora preleviamo pluviometro_mm_acqua_caduti_reg
    
    outb %al, $status_pluviometro
    .bw1:
      inb $status_pluviometro, %al 
      btb $0, %al 
      jnc .bw1
    inl $pluviometro_mm_acqua_caduti_reg, %eax
    movl %eax, milliltri
    #la durata cambia 
    cmpl $48, milliltri
    jz .almeno_48
    jns .almeno_48
    
    .almeno_48:
      cmpw $96, milliltri
      js .minore_96
      jns .maggiore_46
      
    .minore_96:
      shrl $1, duratairr #durata irrigazione viene dimezzata 
      jmp .loopback
      
    .maggiore_46:
      shrl $2, duratairr #1/4 
      jmp .loopback
    
    .loopback:
      sti
  
      
    .elettrovalvola:
      cmpb $0, flag_attivazione_elettrovalvola
      jz .active
      .back:
      addw $1, index_durirr
      cmpw duratairr, index_durirr
      jz .arresta_elettrovalvola
      jmp .retu
      
      
    .active:
      movb $1, flag_attivazione_elettrovalvola
      movb flag_attivazione_elettrovalvola, %al
      outb %al, elettrovalvola (# elettrovalvola booleana e la attivo)
      jmp .back
      
      addw $1, duratairr
      cmpw duratairr, index_durirr
      jz .arresta_elettrovalvola
      
    .arresta_elettrovalvola:
      movb $0, flag_attivazione_elettrovalvola
      movb flag_attivazione_elettrovalvola, %al
      outb %al, elettrovalvola (# elettrovalvola booleana e la disattivo)
      movw $0, index_orarioacc #azzero nuovamente i valori 
      movw $0, index_durirr #azzero nuovamente i valori 
      jmp .retu
      
      
    .increment:
      addw $1, index_orarioacc
      jmp .retu
    
  setting:
    push %rax #eseguito all'interno di un contesto di interrupt 
    push %rdx 
    movb %dil, %dl
    outb %al, %dl #zittisco la richiesta di interruzione
    cmpw orarioacc, index_orarioacc
    jz .elettrovalvola
    jnz .increment
    .retu:
    pop %rdx
    pop %rax 
    ret
    
    
      
      
  .driver 0 (#driver del timer)
    push %rdi 
    movq $irq_timer, %rdi
    call setting
    pop %rdi 
    iret 
      
  
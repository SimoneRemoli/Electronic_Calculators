.org 0x800
.data
  .equ $irq_cambio, 0x01
  .equ $status_cambio, 0x02
  .equ $reg_cambio, 0x03 (#0 = scalata verso il basso, 1 = scalata verso l'alto)
  .equ $status_marcia, 0x04
  .equ $reg_marcia, 0x05
  marcia_attualmente_ingranata: .byte 21 (#inizialmente la marcia_attualmente_ingranata è folle)
  tipologia_scalata: .byte 0
.text
main:
  outb %al, $status_cambio #attivo la periferica cambio esplicitamente 
  sti #abilito le interruzioni 
  .loop:
    jmp .loop
    
    
    
    .metti_a_1:
      movb $48, marcia_attualmente_ingranata
      jmp .ritorno
    .metti_a_2:
      movb $109, marcia_attualmente_ingranata
      jmp .ritorno
    .metti_a_3:
      movb $121, marcia_attualmente_ingranata
      jmp .ritorno
    .metti_a_4:
      movb $51, marcia_attualmente_ingranata
      jmp .ritorno
    .metti_a_5:
      movb $81, marcia_attualmente_ingranata
      jmp .ritorno
    .folle
      movb $21, marcia_attualmente_ingranata
      jmp .ritorno
      
      
    .scalata_verso_il_basso:
      cmpb $48, marcia_attualmente_ingranata #se è a 1 imposta a folle 
      jz .folle
      
      cmpb $109, marcia_attualmente_ingranata #se è a 2 imposta a 1
      jz .metti_a_1
      
      cmpb $121, marcia_attualmente_ingranata #se è a 3 imposta a 2
      jz .metti_a_2
      
      cmpb $51, marcia_attualmente_ingranata #se è a 4 imposta a 3 
      jz .metti_a_3
      
      cmpb $81, marcia_attualmente_ingranata #se è a 5 imposta a 4 
      jz .metti_a_4
    
    
    
    .scalata_verso_alto:
      #bisogna prendere la marcia_attualmente_ingranata, ma la ho in memoria
      cmpb $21, marcia_attualmente_ingranata #se è a folle imposta a 1 
      jz .metti_a_1
      
      cmpb $48, marcia_attualmente_ingranata #se è a 1 imposta a 2 
      jz .metti_a_2
      
      cmpb $109, marcia_attualmente_ingranata #se è a 2 imposta a 3
      jz .metti_a_3
      
      cmpb $121, marcia_attualmente_ingranata #se è a 3 imposta a 4
      jz .metti_a_4
      
      cmpb $51, marcia_attualmente_ingranata #se è a 4 imposta a 5 
      jz .metti_a_5
      
      .ritorno
      outb %al, $status_marcia #comunico al disposotivo che voglio scrivere un dato
      movb marcia_attualmente_ingranata, %al 
      outb %alm $reg_marcia
      .bw1:
        inb $status_marcia, %al 
        btb $0, %al 
        jnc .bw1
        #sicuramente qui il dato è stato consumato
      jmp .back
    
    
    
  setting:
    push %rax (#eseguita all'interno di un contesto di interrupt)
    push %rdx
    movb %sil, %dl
    outb %al, %dl (#zittisco la richiesta di interruzione)
    inb $reg_cambio, %al 
    movb %al, tipologia_scalata #dentro c'è 0 o 1 
    cmpb $0, tipologia_scalata
    jz .scalata_verso_il_basso
    jnz .scalata_verso_alto
    .back
    movb %dil, %dl 
    outb %al, %dl (#il dispositivo cambio è nuovamente pronto per essere interrotto)
    pop %rdx
    pop %rax
    ret
    
    
    
    
    
    
    
    
    
    
  .driver 0
    push %rdi
    push %rsi 
    movq $irq_cambio, %rsi
    movq $status_cambio, %rdi
    call setting 
    pop %rsi 
    pop %rdi 
    iret
  

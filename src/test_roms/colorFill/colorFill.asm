.segment "HEADER"

    .byte "NES"
    .byte $1a
    .byte $02
    .byte $01
    .byte %00000001
    .byte $00
    .byte $00
    .byte $00
    .byte $00
     Filler
    .byte $00, $00, $00, $00, $00

.segment "STARTUP"

.segment "ZEROPAGE"

.segment "VECTORS"

.segment "CHARS"

.segment "CODE"

reset:
  lda #$01	; square 1
  sta $4015
  lda #$08	; period low
  sta $4002
  lda #$02	; period high
  sta $4003
  lda #$bf	; volume
  sta $4000
forever:
  jmp forever

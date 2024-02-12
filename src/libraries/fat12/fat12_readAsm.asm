[bits 16]

file_table_segment: dw 0
file__table_offset: dw 0
file_name: times 11 db 0

section _TEXT class=CODE
global load_root_directory



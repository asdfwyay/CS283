typedef enum decode_state {
    START = 0x00,
    ONE = 0x01,
    ONE_ZERO = 0x02,
    END_SPACE = 0x20,
    END_PERCENT = 0x25,
    END_ATSIGN = 0x40,
    END_NEWLINE = 0x0A
} DECODE_STATE;

// EXTRA CREDIT - print the drexel dragon from the readme.md
void print_dragon();
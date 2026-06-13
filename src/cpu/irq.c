#include <cpu/irq.h>
#include <cpu/idt.h>

#include <drivers/pic.h>

#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1
#define PIC1_CMD  0x20
#define PIC2_CMD  0xA0
#define PIC_EOI   0x20

/* Kernel code segment as set by limine's GDT */
#define KERNEL_CS  0x28
/* Present, DPL=0, 64-bit interrupt gate */
#define IDT_INT_GATE 0x8E

static irq_handler irq_handlers[16];

static void pic_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC2_CMD, PIC_EOI);
    outb(PIC1_CMD, PIC_EOI);
}

void irq_set_mask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    outb(port, inb(port) | (1 << irq));
}

void irq_clear_mask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC1_DATA : PIC2_DATA;
    if (irq >= 8) irq -= 8;
    outb(port, inb(port) & ~(1 << irq));
}

void irq_install_handler(int irq, irq_handler handler) {
    irq_handlers[irq] = handler;
}

void irq_uninstall_handler(int irq) {
    irq_handlers[irq] = 0;
}

static void irq_dispatch(int irq, interrupt_frame*frame) {
    if (irq_handlers[irq])
        irq_handlers[irq](frame);
    pic_eoi(irq);
}

/* One stub per IRQ using __attribute__((interrupt)) so GCC handles the iretq (I really do not wanna use asm!!) */
#define IRQ_STUB(n)                                    \
    __attribute__((interrupt))                         \
    static void irq##n##_stub(interrupt_frame*frame) { \
        irq_dispatch(n, frame);                        \
    }

IRQ_STUB(0)  IRQ_STUB(1)  IRQ_STUB(2)  IRQ_STUB(3)
IRQ_STUB(4)  IRQ_STUB(5)  IRQ_STUB(6)  IRQ_STUB(7)
IRQ_STUB(8)  IRQ_STUB(9)  IRQ_STUB(10) IRQ_STUB(11)
IRQ_STUB(12) IRQ_STUB(13) IRQ_STUB(14) IRQ_STUB(15)

void irq_init(void) {
    /* IRQ0-7 -> vectors 0x20-0x27, IRQ8-15 -> 0x28-0x2F (matches pic_remap) */
#define INSTALL(n) idt_set_gate(0x20 + n, (uint32_t)irq##n##_stub, KERNEL_CS, IDT_INT_GATE)
    INSTALL(0);  INSTALL(1);  INSTALL(2);  INSTALL(3);
    INSTALL(4);  INSTALL(5);  INSTALL(6);  INSTALL(7);
    INSTALL(8);  INSTALL(9);  INSTALL(10); INSTALL(11);
    INSTALL(12); INSTALL(13); INSTALL(14); INSTALL(15);
#undef INSTALL

    /* All IRQs masked by default, caller enables what it needs via irq_clear_mask or smth like that*/
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

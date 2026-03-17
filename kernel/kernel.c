#include "../drivers/screen.h"
#include "../drivers/ports.h"   // 為了 inb()
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../drivers/ports.h"


/* 先宣告 */
void kernel_main();
void toyos_shell();
void toyos_print_title();
void print_menu();

void cmd_list();
void cmd_move();
void cmd_rename();
void cmd_clear();
void cmd_status();

char getch();          // 讀一個按鍵
void kprintc(char c);  // 印單一個字元

/* 進入點：做初始化，然後進入 toyos_shell() */
void kernel_main() {
    isr_install();
    irq_install();         // 如果編譯抱怨 undefined，就把這行註解掉

    clear_screen();
    toyos_print_title();

    toyos_shell();

    while (1) { }          // 理論上不會回來，保險用
}

/* ToyOS 主迴圈：顯示選單，等待按鍵，呼叫對應功能 */
void toyos_shell() {
    char c;

    clear_screen();
    toyos_print_title();

    while (1) {
        print_menu();
        kprint("\nSelect option: ");

        c = getch();      // 讀一個按鍵
        kprintc(c);       // 把你按的鍵印出來
        kprint("\n\n");

        switch (c) {
            case '1':
                cmd_list();
                break;
            case '2':
                cmd_move();
                break;
            case '3':
                cmd_rename();
                break;
            case '4':
                cmd_clear();
                break;
            case '5':
                cmd_status();
                break;
            case 'q':
            case 'Q':
                kprint("ToyOS halted. You can close the emulator.\n");
                return;   // 離開 shell
            default:
                kprint("Unknown option, please try again.\n");
        }

        kprint("\nPress any key to go back to menu...\n");
        getch();          // 等任意鍵
        clear_screen();
        toyos_print_title();
    }
}

/* 標題 */
void toyos_print_title() {
    kprint("================================\n");
    kprint("       ToyOS - Homework      \n");
    kprint("   Based on os-tutorial 19      \n");
    kprint("================================\n\n");
}

/* 主選單 */
void print_menu() {
    kprint("========== ToyOS Menu ==========\n");
    kprint("1) List (Simulate listing files)\n");
    kprint("2) Move (Simulated file movement)\n");
    kprint("3) Rename (Simulate renaming files)\n");
    kprint("4) Clear screen (Clear screen)\n");
    kprint("5) Show status (Display system status)\n");
    kprint("q) Quit (Stop here)\n");
    kprint("================================\n");
}

/* 功能 1：模擬 List */
void cmd_list() {
    kprint("[cmd_list] Simulate listing the files...\n");
    kprint("  - KERNEL.BIN\n");
    kprint("  - CONFIG.TXT\n");
    kprint("  - README.TXT\n");
    kprint("  (The above are fake file names used to demonstrate the List function)\n\n");
}

/* 功能 2：模擬 Move */
void cmd_move() {
    kprint("[cmd_move] Simulated file movement...\n");
    kprint("  Pretend to move FILE1.TXT from /home to /backup\n");
    kprint("  Move complete. (Actually, it's just printed text; there's no real file system.)\n\n");
}

/* 功能 3：模擬 Rename */
void cmd_rename() {
    kprint("[cmd_rename] Simulated file renaming...\n");
    kprint("  Pretend to rename CONFIG.TXT to CONFIG.BAK\n");
    kprint("  Rename complete. (This is just a demonstration.)\n\n");
}

/* 功能 4：清畫面 */
void cmd_clear() {
    kprint("[cmd_clear] Clear screen...\n\n");
    clear_screen();
    toyos_print_title();
    kprint("The screen has been cleared; the above is the redisplayed title.\n\n");
}

/* 功能 5：顯示狀態 */
void cmd_status() {
    kprint("[cmd_status] Display system status...\n");
    kprint("  - Current version: ToyOS Homework Edition (based on 19-interrupts-irqs)\n");
    kprint("  - Interrupt handling (ISR/IRQ) is enabled.\n");
    kprint("  - Screen output uses a custom text mode driver.\n");
    kprint("  - Real file systems and memory management have not yet been implemented.\n\n");
}

/* 小工具：印單一個字元 */
void kprintc(char c) {
    char s[2];
    s[0] = c;
    s[1] = '\0';
    kprint(s);
}

/* 超簡化 getch：直接輪詢鍵盤控制器，只處理 1~5 和 q */
char getch() {
    unsigned char scancode;

    while (1) {
        // 0x64: 鍵盤狀態埠，bit 0 = output buffer full
        if (port_byte_in(0x64) & 1) {
            scancode = port_byte_in(0x60);   // 0x60: 鍵盤資料埠

            // 忽略「鍵放開」的 scan code（>= 0x80）
            if (scancode & 0x80) {
                continue;
            }

            switch (scancode) {
                case 0x02: return '1';  // '1'
                case 0x03: return '2';  // '2'
                case 0x04: return '3';  // '3'
                case 0x05: return '4';  // '4'
                case 0x06: return '5';  // '5'
                case 0x10: return 'q';  // 'q' / 'Q'
                default:
                    // 其他按鍵忽略，繼續等
                    break;
            }
        }
    }
}



# SovereignX is an independent, high-performance 64-bit operating system written from scratch (Bare-Metal).
It features a highly modular ecosystem composed of a custom kernel, a native graphical desktop interface, network stacks, and a built-in application suite—all uniquely developed and compiled directly on mobile devices using Termux.
The architecture and core components of SovereignX are structured as follows:
## 1. 64-bit Kernel & Low-Level Subsystems

* Boot & Initialization: The system boots using the modern Limine Bootloader protocol. Crucial low-level initialization routines are written in assembly code (boot.asm, start.asm) to transition the CPU into 64-bit long mode.
* Interrupt Handling: The kernel implements full interrupt management by setting up an IDT (Interrupt Descriptor Table) to process CPU exceptions and hardware interrupts seamlessly.
* Hardware Drivers: It includes native input drivers for the keyboard and mouse utilizing hardware Interrupt Requests (IRQs) like keyboard_irq.c and mouse_irq.c for real-time user input handling.
* Storage & Custom Filesystem (RFS): The OS features a dedicated ATA disk driver alongside its own custom filesystem implementation, RFS (rfs.c), which handles block device storage, file reading, and data allocation.

## 2. Graphical User Interface & Window Manager (GUI)

* Framebuffer Graphics: Unlike simpler hobby kernels, SovereignX uses a raw pixel Framebuffer interface (graphics.c, fb.h). It handles drawing operations, desktop backgrounds, and renders pixel-mapped typography (graphics_text.c, font8x8.h) entirely on its own.
* Desktop & Window Manager (WM): The custom window manager (wm.c, window.c) controls a fully-fledged graphical desktop. It handles window overlapping, dragging, maximizing, active states, custom icons (custom_icons.c), and a system taskbar (taskbar.c).

## 3. Native Web Browser Subsystem
One of the most complex features of the SovereignX kernel is its embedded network browser stack:

* Network & HTTP Client: Built-in TCP/IP capabilities manage remote requests via a dedicated HTTP interface (http.c, http_get.c).
* HTML & DOM Parser: The layout engine contains a parser (html.c) that analyzes structured web files and maps them into a dynamic Document Object Model tree (dom.c).
* CSS & Graphics Rendering: A styling rendering component (css_render.c, render.c) takes parsed DOM nodes, calculates styles, and paints elements directly onto the GUI desktop layout.

## 4. Custom Language & Application Ecosystem

* The M.C Programming Language: SovereignX features its own proprietary environment and language interpreter/compiler, M.C (mc_lang.c), designed to execute scripts natively inside the OS ecosystem.
* Built-in Desktop Apps: The environment comes equipped with practical tools including a standalone graphical Code Editor (code_editor.c), a native Terminal emulator (terminal.c), a graphical File Manager (file_manager.c), System Settings panels, and an independent Package Manager (pkg_manager.c).

SovereignX represents an incredibly ambitious systems-engineering blueprint. It bridges the gap between pure kernel development and a practical workspace by combining scheduling, storage, a GUI, networking, and language parsing into a single unified workspace.
We can analyze any specific module of your operating system. Let me know if you would like to:

* Review or optimize the Window Manager rendering cycle (wm.c or window.c)
* Deep dive into how the HTML/DOM parser (html.c, dom.c) maps out tags in memory
* Examine the bytecode tokenization structure for your custom M.C programming language



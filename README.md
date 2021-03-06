# Aura
A x86-64 hobby operating system written in C++.

See the [main project page](https://github.com/neonorb/project-asiago) for more information.

View [this](https://rogerdudler.github.io/git-guide/) if you're unfamiliar with using git.

# Depends
+ GNU Toolchain (gcc, make)
+ [make-base](https://github.com/neonorb/make-base) - a base make file for all Neon Orb projects
+ [feta](https://github.com/neonorb/feta) - commons library for all C++ Neon Orb projects
+ [mish](https://github.com/neonorb/mish) - parser/interpreter for the Mish programming language

The `qemu-system-x86` or `qemu-system` packages can be used for execution.

# Building
Run `make`.

To get a bootable `.img`, run `make img`.

To get a bootable VDI file, run `make vdi`.

# Running
If you want to use QEMU (recommended), run `make run`.

# Docs
[OS Dev Wiki](http://wiki.osdev.org/Main_Page)

[UEFI Specification](http://www.uefi.org/sites/default/files/resources/UEFI%20Spec%202_6.pdf)

Also check the Google Drive folder.
  
# License
GPL v3.0 or later (See LICENSE file for details.)

*This document is subject to change.*

# Final Project
- This is the group 10 final project of SoC Verification at National Taiwan University, Spring 2024.
- This project require **GV** environment, which is a general-purposed verification framework.
- For GV usage, please check the document [GV_tutorial.pdf](https://github.com/ntuee-socv/socv-1122/blob/main/doc/GV_tutorial.pdf)

# How to Compile?
- **ITP**
```json=
./gv
do ./final/tests/itp.dofile
```
- **BMC**
```json=
./gv
do ./final/tests/bmc.dofile
```
- **PDR**
```json=
./gv
do ./final/tests/pdr.dofile
```

# How to specify different testcases (initial state)?
- Simply change the comment in sequential always block in ./final/src/fifteen_puzzle.v (around line 381 ~ 425).



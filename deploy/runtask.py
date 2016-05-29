#!/usr/bin/env python
import os
for i in range(10):
    os.system("qsub single.sh -o data/%d.dat"%(i))

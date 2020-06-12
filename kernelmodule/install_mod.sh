#!/bin/bash
sudo insmod vmm.ko;
if [ 0 -ne $? ]; then
  echo "re install module";
  sudo rmmod vmm;
  sudo insmod vmm.ko;
fi


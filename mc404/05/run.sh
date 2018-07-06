#!/bin/bash
lasm -o tarefa05 tarefa05.s
legsim -l tarefa05 -d devices.d

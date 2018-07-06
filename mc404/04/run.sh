#!/bin/bash
lasm -o tarefa04 tarefa04.s
legsim -l tarefa04 -d devices.d

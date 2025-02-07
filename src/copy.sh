#!/bin/bash

scp /home/gitlab-runner/builds/WYKAktHo/0/students/DO6_CICD.ID_356283/blinkyma_student.21_school.ru/DO6_CICD-2/src/cat/s21_cat aero@10.10.0.1:/usr/local/bin/
scp /home/gitlab-runner/builds/WYKAktHo/0/students/DO6_CICD.ID_356283/blinkyma_student.21_school.ru/DO6_CICD-2/src/grep/s21_grep aero@10.10.0.1:/usr/local/bin/
ssh user@10.10.0.1 ls -lah /usr/local/bin
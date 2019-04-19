/*
    IFMG Sabara - Arquitetura de Computadores
    Gerador de binarios MIPS
    Alunos:
        Daniel Elias
        Jonathan
*/

#include <iostream>
#include "io_dados.h"
#include <stdlib.h>
#include <sstream>
#include <string>
#include <bitset>

using namespace std;

    string toBin32Bits(int valor) {
        return bitset<32>(valor).to_string();
    }

    string toBin5Bits(int valor) {
        return bitset<5>(valor).to_string();
    }

    string toBin6Bits(int valor) {
        return bitset<6>(valor).to_string();
    }

int main(int argc, char *argv[]){

    enum registradores{
        $zero,
        $at,
        $v0,
        $v1,
        $a0,
        $a1,
        $a2,
        $a3,
        $t0,
        $t1,
        $t2,
        $t3,
        $t4,
        $t5,
        $t6,
        $t7,
        $s0,
        $s1,
        $s2,
        $s3,
        $s4,
        $s5,
        $s6,
        $s7,
        $t8,
        $t9,
        $k0,
        $k1,
        $gp,
        $sp,
        $fp,
        $ra,
    };

    string s;
    string a;
    a = '$a2';
    registradores reg;
    reg = (registradores) a;
    s = toBin5Bits(reg);
    cout << s;

    return 0;
}

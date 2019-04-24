/*
    IFMG Sabara - Arquitetura de Computadores
    Gerador de binarios MIPS
    Alunos:
        Daniel Elias
        Jonathan Félix
*/

#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <bitset>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include <functional>
#include <utility>

using namespace std;

    //verifica se existem espacos duplicados. retorna true se existir
    bool BothAreSpaces(char lhs, char rhs) {
        return (lhs == rhs) && (lhs == ' ');
    }

    //verifica a regencia da instrucao. retorna true se encontrar ocorrencia da regencia na instrucao
    bool regexOk(string &str){
        // regex original {"[a-z]{2,4}[\s](([\$][a|f|g|k|r|v|s|t][a|t|p|\d]\,\s[\d]*\([\$][(a|f|g|k|r|v|s|t)][a|t|p|[\d]\)|(\$szero\,\s[\d]*\([\$][(a|f|g|k|r|v|s|t)][a|t|p|[\d]\)|(-?[\d]*\,\s[\d]*\([\$][(a|f|g|k|r|v|s|t)][a|t|p|[\d]\))))|([\$][a|f|g|k|r|v|s|t][a|t|p|\d]\,\s|(\$szero\,\s|(-?[\d]*\,\s)))([\$][a|f|g|k|r|v|s|t][a|t|p|\d]\,\s|(\$szero\,\s|(-?[\d]*\,\s)))([\$][a|f|g|k|r|v|s|t][a|t|p|\d]|(\$szero|(-?[\d]*))))"}
        regex express("[a-z]{2,4}[\\s](([\\$][a|f|g|k|r|v|s|t][a|t|p|\\d]\\,\\s[\\d]*\\([\\$][(a|f|g|k|r|v|s|t)][a|t|p|[\\d]\\)|(\\$szero\\,\\s[\\d]*\\([\\$][(a|f|g|k|r|v|s|t)][a|t|p|[\\d]\\)|(-?[\\d]*\\,\\s[\\d]*\\([\\$][(a|f|g|k|r|v|s|t)][a|t|p|[\\d]\\))))|([\\$][a|f|g|k|r|v|s|t][a|t|p|\\d]\\,\\s|(\\$szero\\,\\s|(-?[\\d]*\\,\\s)))([\\$][a|f|g|k|r|v|s|t][a|t|p|\\d]\\,\\s|(\\$szero\\,\\s|(-?[\\d]*\\,\\s)))([\\$][a|f|g|k|r|v|s|t][a|t|p|\\d]|(\\$szero|(-?[\\d]*))))");
        return regex_search(str, express);
    }

    /*
        funcao de leitura de arquivo de instrucoes
        parametros entrada:
            string nome: nome do arquivo que serah lido
            vector lista: ponteiro do vector que recebera a lista de instrucoes lida do arquivo
            retorno: void
        funcoes:
            le linhas do arquivo, remove espacos duplicados, muda instrucoes para minusculo e testa regencia e carrega linhas num vector.
    */
    void lerArquivo(string nome, vector<string> &linhas){
        ifstream arquivo;
        string linha;
        arquivo.open(nome);
        try{
            while(getline(arquivo, linha)){
                //remover espacos duplicados
                string::iterator new_end = unique(linha.begin(), linha.end(), BothAreSpaces);
                linha.erase(new_end, linha.end());
                //transforma string toda em minusculo
                transform(linha.begin(), linha.end(), linha.begin(), ::tolower);
                //verifica regex e insere resultado ou erro no vector
                if (regexOk(linha)){
                    //remove virgula
                    boost::replace_all(linha, ",", " ");
                    //remove parentese
                    boost::replace_all(linha, "(", " ");
                    //remove parentese
                    boost::replace_all(linha, ")", " ");
                    //remover espacos duplicados
                    string::iterator new_end = unique(linha.begin(), linha.end(), BothAreSpaces);
                    linha.erase(new_end, linha.end());
                    linhas.push_back(linha);
                } else {
                    linhas.push_back("Erro. Instrucao nao reconhecida.");
                }
                //regexOk(linha) ? (linhas.push_back(linha)) : (linhas.push_back("Erro. Instrucao nao reconhecida."));
            }
        } catch (exception& erro) {
            cout << "Erro no processamento do arquivo: " << erro.what() << endl;
        }
    };

    struct Instructions{
        string atributo;
        int opcode;
        int rs;
        int rt;
        int rd;
        int shamt;
        int functionI;
        void ini(string patributo, int popcode, int prs, int prt, int prd, int pshamt, int pfunctionI){
            opcode = popcode;
            rs = prs;
            rt = prt;
            rd = prd;
            shamt = pshamt;
            functionI = pfunctionI;
        }
        bool operator<(const Instructions &other) const {
            return (atributo < other.atributo);
        }
    };

    struct R {
        int bin;
        int r(){
            return bin;
        }
    };

    class T {
    private:
        std::string(*expression)(Instructions &s, vector<int>& v);
    public:
        T(std::string(*exp)(Instructions &s, vector<int>& v)) : expression(exp) {}
        std::string execute(Instructions &s, vector<int>& v) {
            return this->expression(s, v);
        }
    };

    string toBin16Bits(int valor) {
        return bitset<16>(valor).to_string();
    }

    string toBin5Bits(int valor) {
        return bitset<5>(valor).to_string();
    }

    string toBin6Bits(int valor) {
        return bitset<6>(valor).to_string();
    }

    string toBin26Bits(int valor) {
        return bitset<26>(valor).to_string();
    }

    /*
        funcao de gravacao do arquivo com os binarios correspondentes das instrucoes
        cria instrucoes
    */
    void criaInstrucoes(vector<string> &listaArquivo, vector<string> &listaInstrucoes){
        //instrucoes do MIPS
        Instructions LUI, BLTZ, BLEZ, BGTZ, BGEZ, BLTZAL, BGEZAL, BEQ, BNE, ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI, LB, LH, LWL, LW, LBU, LHU, LWR, SB, SH, SWL, SW, SWR, J, JAL, SLLV, SRLV, SRAV, ADD, ADDU, SUB, SUBU, AND, OR, XOR, NOR, SLT, SLTU, JR, MTHI, MTLO, MULT, MULTU, DIV, DIVU, JALR, MFHI, MFLO, SLL, SRL, SRA;

        //inicializando instrucoes com valores ja conhecidos
        LUI.ini("lui", 15, 0, 0, 0, 0, 0);
        BLTZ.ini("bltz", 1, 0, 0, 0, 0, 0);
        BLEZ.ini("blez", 6, 0, 0, 0, 0, 0);
        BGTZ.ini("bgtz", 7, 0, 0, 0, 0, 0);
        BGEZ.ini("bgez", 1, 0, 1, 0, 0, 0);
        BLTZAL.ini("bltzal", 1, 0, 16, 0, 0, 0);
        BGEZAL.ini("bgezal", 1, 0, 17, 0, 0, 0);
        BEQ.ini("beq", 4, 0, 0, 0, 0, 0);
        BNE.ini("bne", 5, 0, 0, 0, 0, 0);
        ADDI.ini("addi", 8, 0, 0, 0, 0, 0);
        ADDIU.ini("addiu", 9, 0, 0, 0, 0, 0);
        SLTI.ini("slti", 10, 0, 0, 0, 0, 0);
        SLTIU.ini("sltiu", 11, 0, 0, 0, 0, 0);
        ANDI.ini("andi", 12, 0, 0, 0, 0, 0);
        ORI.ini("ori", 13, 0, 0, 0, 0, 0);
        XORI.ini("xori", 14, 0, 0, 0, 0, 0);
        LB.ini("lb", 32, 0, 0, 0, 0, 0);
        LH.ini("lh", 33, 0, 0, 0, 0, 0);
        LWL.ini("lwl", 34, 0, 0, 0, 0, 0);
        LW.ini("lw", 35, 0, 0, 0, 0, 0);
        LBU.ini("lbu", 36, 0, 0, 0, 0, 0);
        LHU.ini("lhu", 37, 0, 0, 0, 0, 0);
        LWR.ini("lwr", 38, 0, 0, 0, 0, 0);
        SB.ini("sb", 40, 0, 0, 0, 0, 0);
        SH.ini("sh", 41, 0, 0, 0, 0, 0);
        SWL.ini("swl", 42, 0, 0, 0, 0, 0);
        SW.ini("sw", 43, 0, 0, 0, 0, 0);
        SWR.ini("swr", 46, 0, 0, 0, 0, 0);
        J.ini("j", 2, 0, 0, 0, 0, 0);
        JAL.ini("jal", 3, 0, 0, 0, 0, 0);
        SLLV.ini("sllv", 0, 0, 0, 0, 0, 4);
        SRLV.ini("srlv", 0, 0, 0, 0, 0, 6);
        SRAV.ini("srav", 0, 0, 0, 0, 0, 7);
        ADD.ini("add", 0, 0, 0, 0, 0, 32);
        ADDU.ini("addu", 0, 0, 0, 0, 0, 33);
        SUB.ini("sub", 0, 0, 0, 0, 0, 34);
        SUBU.ini("subu", 0, 0, 0, 0, 0, 35);
        AND.ini("and", 0, 0, 0, 0, 0, 36);
        OR.ini("or", 0, 0, 0, 0, 0, 37);
        XOR.ini("xor", 0, 0, 0, 0, 0, 38);
        NOR.ini("nor", 0, 0, 0, 0, 0, 39);
        SLT.ini("slt", 0, 0, 0, 0, 0, 42);
        SLTU.ini("sltu", 0, 0, 0, 0, 0, 43);
        JR.ini("jr", 0, 0, 0, 0, 0, 8);
        MTHI.ini("mthi", 0, 0, 0, 0, 0, 17);
        MTLO.ini("mtlo", 0, 0, 0, 0, 0, 19);
        MULT.ini("mult", 0, 0, 0, 0, 0, 24);
        MULTU.ini("multu", 0, 0, 0, 0, 0, 25);
        DIV.ini("div", 0, 0, 0, 0, 0, 26);
        DIVU.ini("divu", 0, 0, 0, 0, 0, 27);
        JALR.ini("jalr", 0, 0, 0, 0, 0, 9);
        MFHI.ini("mfhi", 0, 0, 0, 0, 0, 16);
        MFLO.ini("mflo", 0, 0, 0, 0, 0, 18);
        SLL.ini("sll", 0, 0, 0, 0, 0, 0);
        SRL.ini("srl", 0, 0, 0, 0, 0, 2);
        SRA.ini("sra", 0, 0, 0, 0, 0, 3);

        //funcao lambda de montagem da instrucao LUI
        T instrI1([](Instructions& instr, vector<int> &result)->string {
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4;
            var1 = instr.opcode;
            var2 = instr.rs;
            var3 = result[1];
            var4 = result[2];
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin16Bits(var4);
            str = ss.str();
            return (str);
        });

        T instrI2([](Instructions& instr, std::vector<int> &result)->string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4;
            var1 = instr.opcode;
            var2 = result[3];
            var3 = instr.rs;
            var4 = result[2];
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin16Bits(var4);
            str = ss.str();
            return (str);
        });

        T instrI3([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4;
            var1 = instr.opcode;
            var2 = result[3];
            var3 = result[1];
            var4 = result[2];
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin16Bits(var4);
            str = ss.str();
            return (str);
        });

        T instrJ([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2;
            var1 = instr.opcode;
            var2 = result[1];
            ss << toBin6Bits(var1) << toBin26Bits(var2);
            str = ss.str();
            return (str);
        });

        T instrR1([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = result[2];
            var3 = result[3];
            var4 = result[1];
            var5 = instr.shamt;
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        T instrR2([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = result[2];
            var3 = instr.rt;
            var4 = instr.rd;
            var5 = instr.shamt;
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        T instrR3([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = result[2];
            var3 = result[3];
            var4 = instr.rd;
            var5 = instr.shamt;
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        T instrR4([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = result[2];
            var3 = instr.rt;
            var4 = result[1];
            var5 = instr.shamt;
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        T instrR5([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = instr.rs;
            var3 = instr.rt;
            var4 = result[1];
            var5 = instr.shamt;
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        T instrR6([](Instructions& instr, std::vector<int> &result)->std::string{
            string str;
            std::stringstream ss;
            int var1, var2, var3, var4, var5, var6;
            var1 = instr.opcode;
            var2 = instr.rs;
            var3 = result[3];
            var4 = result[1];
            var5 = result[2];
            var6 = instr.functionI;
            ss << toBin6Bits(var1) << toBin5Bits(var2) << toBin5Bits(var3) << toBin5Bits(var4) << toBin5Bits(var5) << toBin6Bits(var6);
            str = ss.str();
            return (str);
        });

        pair<Instructions, T> par1 = {LUI, instrI1};
        pair<Instructions, T> par2 = {BLTZ, instrI2};
        pair<Instructions, T> par3 = {BLEZ, instrI2};
        pair<Instructions, T> par4 = {BGTZ, instrI2};
        pair<Instructions, T> par5 = {BGEZ, instrI2};
        pair<Instructions, T> par6 = {BLTZAL, instrI2};
        pair<Instructions, T> par7 = {BGEZAL, instrI2};
        pair<Instructions, T> par8 = {BEQ, instrI3};
        pair<Instructions, T> par9 = {BNE, instrI3};
        pair<Instructions, T> par10 = {ADDI, instrI3};
        pair<Instructions, T> par11 = {ADDIU, instrI3};
        pair<Instructions, T> par12 = {SLTI, instrI3};
        pair<Instructions, T> par13 = {SLTIU, instrI3};
        pair<Instructions, T> par14 = {ANDI, instrI3};
        pair<Instructions, T> par15 = {ORI, instrI3};
        pair<Instructions, T> par16 = {XORI, instrI3};
        pair<Instructions, T> par17 = {LB, instrI3};
        pair<Instructions, T> par18 = {LH, instrI3};
        pair<Instructions, T> par19 = {LWL, instrI3};
        pair<Instructions, T> par20 = {LW, instrI3};
        pair<Instructions, T> par21 = {LBU, instrI3};
        pair<Instructions, T> par22 = {LHU, instrI3};
        pair<Instructions, T> par23 = {LWR, instrI3};
        pair<Instructions, T> par24 = {SB, instrI3};
        pair<Instructions, T> par25 = {SH, instrI3};
        pair<Instructions, T> par26 = {SWL, instrI3};
        pair<Instructions, T> par27 = {SW, instrI3};
        pair<Instructions, T> par28 = {SWR, instrI3};
        pair<Instructions, T> par29 = {J, instrJ};
        pair<Instructions, T> par30 = {JAL, instrJ};
        pair<Instructions, T> par31 = {SLLV, instrR1};
        pair<Instructions, T> par32 = {SRLV, instrR1};
        pair<Instructions, T> par33 = {SRAV, instrR1};
        pair<Instructions, T> par34 = {ADD, instrR1};
        pair<Instructions, T> par35 = {ADDU, instrR1};
        pair<Instructions, T> par36 = {SUB, instrR1};
        pair<Instructions, T> par37 = {SUBU, instrR1};
        pair<Instructions, T> par38 = {AND, instrR1};
        pair<Instructions, T> par39 = {OR, instrR1};
        pair<Instructions, T> par40 = { XOR, instrR1};
        pair<Instructions, T> par41 = {NOR, instrR1};
        pair<Instructions, T> par42 = {SLT, instrR1};
        pair<Instructions, T> par43 = {SLTU, instrR1};
        pair<Instructions, T> par44 = {JR, instrR2};
        pair<Instructions, T> par45 = {MTHI, instrR2};
        pair<Instructions, T> par46 = {MTLO, instrR2};
        pair<Instructions, T> par47 = {MULT, instrR3};
        pair<Instructions, T> par48 = {MULTU, instrR3};
        pair<Instructions, T> par49 = {DIV, instrR3};
        pair<Instructions, T> par50 = {DIVU, instrR3};
        pair<Instructions, T> par51 = {JALR, instrR4};
        pair<Instructions, T> par52 = {MFHI, instrR5};
        pair<Instructions, T> par53 = {MFLO, instrR5};
        pair<Instructions, T> par54 = {SLL, instrR6};
        pair<Instructions, T> par55 = {SRL, instrR6};
        pair<Instructions, T> par56 = {SRA, instrR6};


        //map que associa a string pesquisada a funcao lambda de conversao
        map<Instructions, T> mapMIPS;
        mapMIPS.insert(par1);
        mapMIPS.insert(par2);
        mapMIPS.insert(par3);
        mapMIPS.insert(par4);
        mapMIPS.insert(par5);
        mapMIPS.insert(par6);
        mapMIPS.insert(par7);
        mapMIPS.insert(par8);
        mapMIPS.insert(par9);
        mapMIPS.insert(par10);
        mapMIPS.insert(par11);
        mapMIPS.insert(par12);
        mapMIPS.insert(par13);
        mapMIPS.insert(par14);
        mapMIPS.insert(par15);
        mapMIPS.insert(par16);
        mapMIPS.insert(par17);
        mapMIPS.insert(par18);
        mapMIPS.insert(par19);
        mapMIPS.insert(par20);
        mapMIPS.insert(par21);
        mapMIPS.insert(par22);
        mapMIPS.insert(par23);
        mapMIPS.insert(par24);
        mapMIPS.insert(par25);
        mapMIPS.insert(par26);
        mapMIPS.insert(par17);
        mapMIPS.insert(par28);
        mapMIPS.insert(par29);
        mapMIPS.insert(par30);
        mapMIPS.insert(par31);
        mapMIPS.insert(par32);
        mapMIPS.insert(par33);
        mapMIPS.insert(par34);
        mapMIPS.insert(par35);
        mapMIPS.insert(par36);
        mapMIPS.insert(par37);
        mapMIPS.insert(par38);
        mapMIPS.insert(par39);
        mapMIPS.insert(par40);
        mapMIPS.insert(par41);
        mapMIPS.insert(par42);
        mapMIPS.insert(par43);
        mapMIPS.insert(par44);
        mapMIPS.insert(par45);
        mapMIPS.insert(par46);
        mapMIPS.insert(par47);
        mapMIPS.insert(par48);
        mapMIPS.insert(par49);
        mapMIPS.insert(par50);
        mapMIPS.insert(par51);
        mapMIPS.insert(par52);
        mapMIPS.insert(par53);
        mapMIPS.insert(par54);
        mapMIPS.insert(par55);
        mapMIPS.insert(par56);

        map<string, Instructions> mapInstr;
        mapInstr.insert({"lui", LUI});
        mapInstr.insert({"bltz", BLTZ});
        mapInstr.insert({"blez", BLEZ});
        mapInstr.insert({"bgtz", BGTZ});
        mapInstr.insert({"bgez", BGEZ});
        mapInstr.insert({"bltzal", BLTZAL});
        mapInstr.insert({"bgezal", BGEZAL});
        mapInstr.insert({"beq", BEQ});
        mapInstr.insert({"bne", BNE});
        mapInstr.insert({"addi", ADDI});
        mapInstr.insert({"addiu", ADDIU});
        mapInstr.insert({"slti", SLTI});
        mapInstr.insert({"sltiu", SLTIU});
        mapInstr.insert({"andi", ANDI});
        mapInstr.insert({"ori", ORI});
        mapInstr.insert({"xori", XORI});
        mapInstr.insert({"lb", LB});
        mapInstr.insert({"lh", LH});
        mapInstr.insert({"lwl", LWL});
        mapInstr.insert({"lw", LW});
        mapInstr.insert({"lbu", LBU});
        mapInstr.insert({"lhu", LHU});
        mapInstr.insert({"lwr", LWR});
        mapInstr.insert({"sb", SB});
        mapInstr.insert({"sh", SH});
        mapInstr.insert({"swl", SWL});
        mapInstr.insert({"sw", SW});
        mapInstr.insert({"swr", SWR});
        mapInstr.insert({"j", J});
        mapInstr.insert({"jal", JAL});
        mapInstr.insert({"sllv", SLLV});
        mapInstr.insert({"srlv", SRLV});
        mapInstr.insert({"srav", SRAV});
        mapInstr.insert({"add", ADD});
        mapInstr.insert({"addu", ADDU});
        mapInstr.insert({"sub", SUB});
        mapInstr.insert({"subu", SUBU});
        mapInstr.insert({"and", AND});
        mapInstr.insert({"or", OR});
        mapInstr.insert({"xor", XOR});
        mapInstr.insert({"nor", NOR});
        mapInstr.insert({"slt", SLT});
        mapInstr.insert({"sltu", SLTU});
        mapInstr.insert({"jr", JR});
        mapInstr.insert({"mthi", MTHI});
        mapInstr.insert({"mtlo", MTLO});
        mapInstr.insert({"mult", MULT});
        mapInstr.insert({"multu", MULTU});
        mapInstr.insert({"div", DIV});
        mapInstr.insert({"divu", DIVU});
        mapInstr.insert({"jalr", JALR});
        mapInstr.insert({"mfhi", MFHI});
        mapInstr.insert({"mflo", MFLO});
        mapInstr.insert({"sll", SLL});
        mapInstr.insert({"srl", SRL});
        mapInstr.insert({"sra", SRA});

        R $a0, $a1, $a2, $a3, $at, $fp, $gp, $k0, $k1, $ra, $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $sp, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9, $v0, $v1, $zero;

        $a0.bin = 4;
        $a1.bin = 5;
        $a2.bin = 6;
        $a3.bin = 7;
        $at.bin = 1;
        $fp.bin = 30;
        $gp.bin = 28;
        $k0.bin = 26;
        $k1.bin = 27;
        $ra.bin = 31;
        $s0.bin = 16;
        $s1.bin = 17;
        $s2.bin = 18;
        $s3.bin = 19;
        $s4.bin = 20;
        $s5.bin = 21;
        $s6.bin = 22;
        $s7.bin = 23;
        $sp.bin = 29;
        $t0.bin = 8;
        $t1.bin = 9;
        $t2.bin = 10;
        $t3.bin = 11;
        $t4.bin = 12;
        $t5.bin = 13;
        $t6.bin = 14;
        $t7.bin = 15;
        $t8.bin = 24;
        $t9.bin = 25;
        $v0.bin = 2;
        $v1.bin = 3;
        $zero.bin = 0;

        map<string, R>rgstrMIPS;
        rgstrMIPS.insert({"$a0", $a0});
        rgstrMIPS.insert({"$a1", $a1});
        rgstrMIPS.insert({"$a2", $a2});
        rgstrMIPS.insert({"$a3", $a3});
        rgstrMIPS.insert({"$at", $at});
        rgstrMIPS.insert({"$fp", $fp});
        rgstrMIPS.insert({"$gp", $gp});
        rgstrMIPS.insert({"$k0", $k0});
        rgstrMIPS.insert({"$k1", $k1});
        rgstrMIPS.insert({"$ra", $ra});
        rgstrMIPS.insert({"$s0", $s0});
        rgstrMIPS.insert({"$s1", $s1});
        rgstrMIPS.insert({"$s2", $s2});
        rgstrMIPS.insert({"$s3", $s3});
        rgstrMIPS.insert({"$s4", $s4});
        rgstrMIPS.insert({"$s5", $s5});
        rgstrMIPS.insert({"$s6", $s6});
        rgstrMIPS.insert({"$s7", $s7});
        rgstrMIPS.insert({"$sp", $sp});
        rgstrMIPS.insert({"$t0", $t0});
        rgstrMIPS.insert({"$t1", $t1});
        rgstrMIPS.insert({"$t2", $t2});
        rgstrMIPS.insert({"$t3", $t3});
        rgstrMIPS.insert({"$t4", $t4});
        rgstrMIPS.insert({"$t5", $t5});
        rgstrMIPS.insert({"$t6", $t6});
        rgstrMIPS.insert({"$t7", $t7});
        rgstrMIPS.insert({"$t8", $t8});
        rgstrMIPS.insert({"$t9", $t9});
        rgstrMIPS.insert({"$v0", $v0});
        rgstrMIPS.insert({"$v1", $v1});
        rgstrMIPS.insert({"$szero", $zero});

        //vector string que recebe as instrucoes das linhas
        //vector<string> result;

        map<string, R>:: iterator itmap;

        //montagem da string de instrucoes
        for(auto i:listaArquivo){
            //pega todas as linhas e faz um split

            istringstream iss(i);
            vector<std::string> result(std::istream_iterator<std::string>{iss}, istream_iterator<std::string>());
            //teste se a linha contem erro pego pelo regex
            if(result[0].compare("Erro.") != 0){ //0: instrucao incorreta. !=0 insstrucao correta
                vector<int> vInt;
                vInt.push_back(0);
                //substitui todos os registradores do vetor pelos respectivos valores inteiros
                for (int k = 1; k < result.size(); k++){
                    itmap = rgstrMIPS.find(result[k]);
                    if(itmap != rgstrMIPS.end()){
                        vInt.push_back(itmap->second.r());
                    } else {
                        int var = stoi(result[k]);
                        vInt.push_back(var);
                    }
                }
                string tipoT = result[0];
                string str = mapMIPS.at(mapInstr.at(tipoT)).execute(mapInstr.at(tipoT), vInt);
                listaInstrucoes.push_back(str);
            }
            result.clear();
        }
        rgstrMIPS.clear();
        mapMIPS.clear();
    }


    void gravarArquivo(string nome, vector<string> &listaInstrucoes){
        ofstream arquivoSaida;
        arquivoSaida.open(nome); //, ios::app); parâmetro para abrir o arquivo e posicionar o cursor no final do arquivo.
        for (auto i : listaInstrucoes){
            arquivoSaida << i << "\n";
        }
        arquivoSaida.close();
    }

int main(int argc, char *argv[]){

    string nomeArquivo = "programa3.txt";
    //vector com linhas do arquivo
    vector<string> listaArquivo;

    lerArquivo(nomeArquivo, listaArquivo);

    vector<string> listaInstrucoes;

    criaInstrucoes(listaArquivo, listaInstrucoes);

    nomeArquivo = "bin_of_" + nomeArquivo;
    gravarArquivo(nomeArquivo, listaInstrucoes);

    cout << "Concluido! Os binarios foram gravados no arquivo " << nomeArquivo << endl;
    //limpa memoria
    listaArquivo.clear();
    listaInstrucoes.clear();

return 0;
}

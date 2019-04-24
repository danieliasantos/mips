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
                regexOk(linha) ? (linhas.push_back(linha)) : (linhas.push_back("Erro. Instrucao nao reconhecida."));
            }
        } catch (exception& erro) {
            cout << "Erro no processamento do arquivo: " << erro.what() << endl;
        }
    };

    struct Instructions{
        string opcode;
        string rs;
        string rt;
        string rd;
        string shamt;
        string functionI;
        void ini(string popcode, string prs, string prt, string prd, string pshamt, string pfunctionI){
            opcode = popcode;
            rs = prs;
            rt = prt;
            rd = prd;
            shamt = pshamt;
            functionI = pfunctionI;
        }
    };

    struct Registers{
        string bin;
        void ini(string t){
            bin = t;
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

    //funcao lambda de montagem da instrucao LUI
    string instrI1 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << instr.rs << result[1] << toBin16Bits(stoi(result[2]));
        txt = ss.str();
        return txt;
    }

    string instrI2 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[3] << instr.rs << toBin16Bits(stoi(result[2]));
        txt = ss.str();
        return txt;
    }

    string instrI3 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[3] << result[1] << toBin16Bits(stoi(result[2]));
        txt = ss.str();
        return txt;
    }

    string instrJ (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << toBin26Bits(stoi(result[1]));
        txt = ss.str();
        return txt;
    }

    string instrR1 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[2] << result[3] << result[1] << instr.shamt << instr.functionI;
        txt = ss.str();
        return txt;
    }

    string instrR2 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[2] << instr.rt << instr.rd << instr.shamt << instr.functionI;
        txt = ss.str();
        return txt;
    }

    string instrR3 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[2] << result[3] << instr.rd << instr.shamt << instr.functionI;
        txt = ss.str();
        return txt;
    }

    string instrR4 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << result[2] << instr.rt << result[1] << instr.shamt << instr.functionI;
        txt = ss.str();
    }

    string instrR5 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << instr.rs << instr.rt << result[1] << instr.shamt << instr.functionI;
        txt = ss.str();
        return txt;
    }

    string instrR6 (Instructions instr, vector<string> &result){
        string txt;
        std::stringstream ss;
        ss << instr.opcode << instr.rs << result[3] << result[1] << result[2] << instr.functionI;
        txt = ss.str();
        return txt;
    }

    /*
        cria instrucoes
    */

    void criaInstrucoes(vector<string> &listaArquivo, vector<string> &listaInstrucoes){
        //vector string que recebe as instrucoes das linhas
        vector<string> result;

        //instrucoes do MIPS
        Instructions LUI, BLTZ, BLEZ, BGTZ, BGEZ, BLTZAL, BGEZAL, BEQ, BNE, ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI, LB, LH, LWL, LW, LBU, LHU, LWR, SB, SH, SWL, SW, SWR, J, JAL, SLLV, SRLV, SRAV, ADD, ADDU, SUB, SUBU, AND, OR, XOR, NOR, SLT, SLTU, JR, MTHI, MTLO, MULT, MULTU, DIV, DIVU, JALR, MFHI, MFLO, SLL, SRL, SRA;

        //inicializando instrucoes com valores ja conhecidos
        LUI.ini("001111", "00000", "", "", "", "");
        BLTZ.ini("000001", "", "00000", "", "", "");
        BLEZ.ini("000110", "", "00000", "", "", "");
        BGTZ.ini("000111", "", "00000", "", "", "");
        BGEZ.ini("000001", "", "00001", "", "", "");
        BLTZAL.ini("000001", "", "10000", "", "", "");
        BGEZAL.ini("000001", "", "10001", "", "", "");
        BEQ.ini("000100", "", "", "", "", "");
        BNE.ini("000101", "", "", "", "", "");
        ADDI.ini("001000", "", "", "", "", "");
        ADDIU.ini("001001", "", "", "", "", "");
        SLTI.ini("001010", "", "", "", "", "");
        SLTIU.ini("001011", "", "", "", "", "");
        ANDI.ini("001100", "", "", "", "", "");
        ORI.ini("001101", "", "", "", "", "");
        XORI.ini("001110", "", "", "", "", "");
        LB.ini("100000", "", "", "", "", "");
        LH.ini("100001", "", "", "", "", "");
        LWL.ini("100010", "", "", "", "", "");
        LW.ini("100011", "", "", "", "", "");
        LBU.ini("100100", "", "", "", "", "");
        LHU.ini("100101", "", "", "", "", "");
        LWR.ini("100110", "", "", "", "", "");
        SB.ini("101000", "", "", "", "", "");
        SH.ini("101001", "", "", "", "", "");
        SWL.ini("101010", "", "", "", "", "");
        SW.ini("101011", "", "", "", "", "");
        SWR.ini("101110", "", "", "", "", "");
        J.ini("000010", "", "", "", "", "");
        JAL.ini("000011", "", "", "", "", "");
        SLLV.ini("000000", "", "", "", "00000", "000100");
        SRLV.ini("000000", "", "", "", "00000", "000110");
        SRAV.ini("000000", "", "", "", "00000", "000111");
        ADD.ini("000000", "", "", "", "00000", "100000");
        ADDU.ini("000000", "", "", "", "00000", "100001");
        SUB.ini("000000", "", "", "", "00000", "100010");
        SUBU.ini("000000", "", "", "", "00000", "100011");
        AND.ini("000000", "", "", "", "00000", "100100");
        OR.ini("000000", "", "", "", "00000", "100101");
        XOR.ini("000000", "", "", "", "00000", "100110");
        NOR.ini("000000", "", "", "", "00000", "100111");
        SLT.ini("000000", "", "", "", "00000", "101101");
        SLTU.ini("000000", "", "", "", "00000", "101011");
        JR.ini("000000", "", "00000", "00000", "00000", "001000");
        MTHI.ini("000000", "", "00000", "00000", "00000", "010001");
        MTLO.ini("000000", "", "00000", "00000", "00000", "010011");
        MULT.ini("000000", "", "", "00000", "00000", "011000");
        MULTU.ini("000000", "", "", "00000", "00000", "011001");
        DIV.ini("000000", "", "", "00000", "00000", "011010");
        DIVU.ini("000000", "", "", "00000", "00000", "011011");
        JALR.ini("000000", "", "00000", "", "00000", "001001");
        MFHI.ini("000000", "00000", "00000", "", "00000", "010000");
        MFLO.ini("000000", "00000", "00000", "", "00000", "010010");
        SLL.ini("000000", "00000", "", "", "", "000000");
        SRL.ini("000000", "00000", "", "", "", "000010");
        SRA.ini("000000", "00000", "", "", "", "000011");

        //map<string, string>rgstrMIPS;
        Registers $a0, $a1, $a2, $a3, $at, $fp, $gp, $k0, $k1, $ra, $s0, $s1, $s2, $s3, $s4, $s5, $s6, $s7, $sp, $t0, $t1, $t2, $t3, $t4, $t5, $t6, $t7, $t8, $t9, $v0, $v1, $zero;

        $a0.ini("00100");
        $a1.ini("00101");
        $a2.ini("00110");
        $a3.ini("00111");
        $at.ini("00001");
        $fp.ini("11110");
        $gp.ini("11100");
        $k0.ini("11010");
        $k1.ini("11011");
        $ra.ini("11111");
        $s0.ini("10000");
        $s1.ini("10001");
        $s2.ini("10010");
        $s3.ini("10011");
        $s4.ini("10100");
        $s5.ini("10101");
        $s6.ini("10110");
        $s7.ini("10111");
        $sp.ini("11101");
        $t0.ini("01000");
        $t1.ini("01001");
        $t2.ini("01010");
        $t3.ini("01011");
        $t4.ini("01100");
        $t5.ini("01101");
        $t6.ini("01110");
        $t7.ini("01111");
        $t8.ini("11000");
        $t9.ini("11001");
        $v0.ini("00010");
        $v1.ini("00011");
        $zero.ini("00000");

        //montagem da string de binario
        for(auto i:listaArquivo){
            //substitui virgula por espaco
            boost::replace_all(i, ",", "");
            //substitui parentese por espaco
            boost::replace_all(i, "(", " ");
            //substitui parentese por espaco
            boost::replace_all(i, ")", " ");
            //pega todas as linhas e faz um split
            boost::split(result, i, boost::is_any_of(" "));
            //teste se o split foi correto

            if(result[0].compare("Erro.") != 0){ //0: instrucao incorreta. !=0 insstrucao correta
                for(int j = 1; j < result.size(); j++){
                    if (result[j].compare("$a0") == 0){
                        result[j] = $a0.bin;
                    } else if (result[j].compare("$a1") == 0){
                        result[j] = $a1.bin;
                    } else if (result[j].compare("$a2") == 0){
                        result[j] = $a2.bin;
                    } else if (result[j].compare("$a3") == 0){
                        result[j] = $a3.bin;
                    } else if (result[j].compare("$at") == 0){
                        result[j] = $at.bin;
                    } else if (result[j].compare("$fp") == 0){
                        result[j] = $fp.bin;
                    } else if (result[j].compare("$gp") == 0){
                        result[j] = $gp.bin;
                    } else if (result[j].compare("$k0") == 0){
                        result[j] = $k0.bin;
                    } else if (result[j].compare("$k1") == 0){
                        result[j] = $k1.bin;
                    } else if (result[j].compare("$ra") == 0){
                        result[j] = $ra.bin;
                    } else if (result[j].compare("$s0") == 0){
                        result[j] = $s0.bin;
                    } else if (result[j].compare("$s1") == 0){
                        result[j] = $s1.bin;
                    } else if (result[j].compare("$s2") == 0){
                        result[j] = $s2.bin;
                    } else if (result[j].compare("$s3") == 0){
                        result[j] = $s3.bin;
                    } else if (result[j].compare("$s4") == 0){
                        result[j] = $s4.bin;
                    } else if (result[j].compare("$s5") == 0){
                        result[j] = $s4.bin;
                    } else if (result[j].compare("$s6") == 0){
                        result[j] = $s6.bin;
                    } else if (result[j].compare("$s7") == 0){
                        result[j] = $s7.bin;
                    } else if (result[j].compare("$sp") == 0){
                        result[j] = $sp.bin;
                    } else if (result[j].compare("$t0") == 0){
                        result[j] = $t0.bin;
                    } else if (result[j].compare("$t1") == 0){
                        result[j] = $t1.bin;
                    } else if (result[j].compare("$t2") == 0){
                        result[j] = $t2.bin;
                    } else if (result[j].compare("$t3") == 0){
                        result[j] = $t3.bin;
                    } else if (result[j].compare("$t4") == 0){
                        result[j] = $t4.bin;
                    } else if (result[j].compare("$t5") == 0){
                        result[j] = $t5.bin;
                    } else if (result[j].compare("$t6") == 0){
                        result[j] = $t6.bin;
                    } else if (result[j].compare("$t7") == 0){
                        result[j] = $t7.bin;
                    } else if (result[j].compare("$t8") == 0){
                        result[j] = $t8.bin;
                    } else if (result[j].compare("$t9") == 0){
                        result[j] = $t9.bin;
                    } else if (result[j].compare("$v0") == 0){
                        result[j] = $v0.bin;
                    } else if (result[j].compare("$v1") == 0){
                        result[j] = $v1.bin;
                    } else if (result[j].compare("$zero") == 0){
                        result[j] = $zero.bin;
                    }
                }
                if(result[0].compare("lui") == 0){

                } else if (result[0].compare("bltz") == 0){

                } else if (result[0].compare("blez") == 0){

                } else if (result[0].compare("bgtz") == 0){

                } else if (result[0].compare("bgez") == 0){

                } else if (result[0].compare("bltzal") == 0){

                } else if (result[0].compare("bgezal") == 0){

                } else if (result[0].compare("beq") == 0){

                } else if (result[0].compare("bne") == 0){

                } else if (result[0].compare("addi") == 0){

                } else if (result[0].compare("addiu") == 0){

                } else if (result[0].compare("slti") == 0){

                } else if (result[0].compare("sltiu") == 0){

                } else if (result[0].compare("andi") == 0){

                } else if (result[0].compare("ori") == 0){

                } else if (result[0].compare("xori") == 0){

                } else if (result[0].compare("lb") == 0){

                } else if (result[0].compare("lh") == 0){

                } else if (result[0].compare("lwl") == 0){

                } else if (result[0].compare("lw") == 0){

                } else if (result[0].compare("lbu") == 0){

                } else if (result[0].compare("lhu") == 0){

                } else if (result[0].compare("lwr") == 0){

                } else if (result[0].compare("sb") == 0){

                } else if (result[0].compare("sh") == 0){

                } else if (result[0].compare("swl") == 0){

                } else if (result[0].compare("sw") == 0){

                } else if (result[0].compare("swr") == 0){

                } else if (result[0].compare("j") == 0){

                } else if (result[0].compare("jal") == 0){

                } else if (result[0].compare("sllv") == 0){

                } else if (result[0].compare("srlv") == 0){

                } else if (result[0].compare("srav") == 0){

                } else if (result[0].compare("add") == 0){

                } else if (result[0].compare("addu") == 0){

                } else if (result[0].compare("sub") == 0){

                } else if (result[0].compare("subu") == 0){

                } else if (result[0].compare("and") == 0){

                } else if (result[0].compare("or") == 0){

                } else if (result[0].compare("xor") == 0){

                } else if (result[0].compare("nor") == 0){

                } else if (result[0].compare("slt") == 0){

                } else if (result[0].compare("sltu") == 0){

                } else if (result[0].compare("jr") == 0){

                } else if (result[0].compare("mthi") == 0){

                } else if (result[0].compare("mtlo") == 0){

                } else if (result[0].compare("mult") == 0){

                } else if (result[0].compare("multu") == 0){

                } else if (result[0].compare("div") == 0){

                } else if (result[0].compare("divu") == 0){

                } else if (result[0].compare("jalr") == 0){

                } else if (result[0].compare("mfhi") == 0){

                } else if (result[0].compare("mflo") == 0){

                } else if (result[0].compare("sll") == 0){

                } else if (result[0].compare("srl") == 0){

                } else if (result[0].compare("sra") == 0){

                }
                //Instructions itr = mapMIPS.find(result[0]);
                //string txt = mapMIPS.find(result[0]).second.execute(itr, result);
                //listaInstrucoes.push_back(string);
            } else {
                //listaInstrucoes.push_back("Erro. Instrucao nao reconhecida.");
            }
        }
        result.clear();
    }

int main(int argc, char *argv[]){

    //vector com linhas do arquivo
    vector<string> listaArquivo;

    lerArquivo("teste.txt", listaArquivo);

    vector<string> listaInstrucoes;
    criaInstrucoes(listaArquivo, listaInstrucoes);

    //gravarArquivo("", listaInstrucoes);

    //limpa memoria
    listaArquivo.clear();
    listaInstrucoes.clear();

    return 0;
}


/*        //map que associa a string pesquisada a funcao lambda de conversao
        map<string, Instructions> mapMIPS;
        mapMIPS.insert({"lui", LUI});
        mapMIPS["bltz"] = make_pair(BLTZ, instrI2);
        mapMIPS["blez"] = make_pair(BLEZ, instrI2);
        mapMIPS["bgtz"] = make_pair(BGTZ, instrI2);
        mapMIPS["bgez"] = make_pair(BGEZ, instrI2);
        mapMIPS["bltzal"] = make_pair(BLTZAL, instrI2);
        mapMIPS["bgezal"] = make_pair(BGEZAL, instrI2);
        mapMIPS["beq"] = make_pair(BEQ, instrI3);
        mapMIPS["bne"] = make_pair(BNE, instrI3);
        mapMIPS["addi"] = make_pair(ADDI, instrI3);
        mapMIPS["addiu"] = make_pair(ADDIU, instrI3);
        mapMIPS["slti"] = make_pair(SLTI, instrI3);
        mapMIPS["sltiu"] = make_pair(SLTIU, instrI3);
        mapMIPS["andi"] = make_pair(ANDI, instrI3);
        mapMIPS["ori"] = make_pair(ORI, instrI3);
        mapMIPS["xori"] = make_pair(XORI, instrI3);
        mapMIPS["lb"] = make_pair(LB, instrI3);
        mapMIPS["lh"] = make_pair(LH, instrI3);
        mapMIPS["lwl"] = make_pair(LWL, instrI3);
        mapMIPS["lw"] = make_pair(LW, instrI3);
        mapMIPS["lbu"] = make_pair(LBU, instrI3);
        mapMIPS["lhu"] = make_pair(LHU, instrI3);
        mapMIPS["lwr"] = make_pair(LWR, instrI3);
        mapMIPS["sb"] = make_pair(SB, instrI3);
        mapMIPS["sh"] = make_pair(SH, instrI3);
        mapMIPS["swl"] = make_pair(SWL, instrI3);
        mapMIPS["sw"] = make_pair(SW, instrI3);
        mapMIPS["swr"] = make_pair(SWR, instrI3);
        mapMIPS["j"] = make_pair(J, instrJ);
        mapMIPS["jal"] = make_pair(JAL, instrJ);
        mapMIPS["sllv"] = make_pair(SLLV, instrR1);
        mapMIPS["srlv"] = make_pair(SRLV, instrR1);
        mapMIPS["srav"] = make_pair(SRAV, instrR1);
        mapMIPS["add"] = make_pair(ADD, instrR1);
        mapMIPS["addu"] = make_pair(ADDU, instrR1);
        mapMIPS["sub"] = make_pair(SUB, instrR1);
        mapMIPS["subu"] = make_pair(SUBU, instrR1);
        mapMIPS["and"] = make_pair(AND, instrR1);
        mapMIPS["or"] = make_pair(OR, instrR1);
        mapMIPS["xor"] = make_pair(XOR, instrR1);
        mapMIPS["nor"] = make_pair(NOR, instrR1);
        mapMIPS["slt"] = make_pair(SLT, instrR1);
        mapMIPS["sltu"] = make_pair(SLTU, instrR1);
        mapMIPS["jr"] = make_pair(JR, instrR2);
        mapMIPS["mthi"] = make_pair(MTHI, instrR2);
        mapMIPS["mtlo"] = make_pair(MTLO, instrR2);
        mapMIPS["mult"] = make_pair(MULT, instrR3);
        mapMIPS["multu"] = make_pair(MULTU, instrR3);
        mapMIPS["div"] = make_pair(DIV, instrR3);
        mapMIPS["divu"] = make_pair(DIVU, instrR3);
        mapMIPS["jalr"] = make_pair(JALR, instrR4);
        mapMIPS["mfhi"] = make_pair(MFHI, instrR5);
        mapMIPS["mflo"] = make_pair(MFLO, instrR5);
        mapMIPS["sll"] = make_pair(SLL, instrR6);
        mapMIPS["srl"] = make_pair(SRL, instrR6);
        mapMIPS["sra"] = make_pair(SRA, instrR6);


    class T {
    private:
        string (*expression)();
    public:
        T(string (*exp)()) : expression(exp) {}
        string execute() {
            return this->expression();
        }
    };
*/

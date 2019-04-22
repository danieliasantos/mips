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

using namespace std;

    //verifica se existem espacos duplicados. retorna true se existir
    bool BothAreSpaces(char lhs, char rhs) {
        return (lhs == rhs) && (lhs == ' ');
    }

    //verifica a regencia da instrucao. retorna true se encontrar ocorrencia da regencia na instrucao
    bool regexOk(string &str){
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

        //funcao lambda de montagem da instrucao LUI
        auto instrI1 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << instr.rs << result[1] << toBin16Bits(stoi(result[2]));
            return (ss.str());
        };

        auto instrI2 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[3] << instr.rs << toBin16Bits(stoi(result[2]));
            return (ss.str());
        };

        auto instrI3 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[3] << result[1] << toBin16Bits(stoi(result[2]));
            return (ss.str());
        };

        auto instrJ = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << toBin26Bits(stoi(result[1]));
            return (ss.str());
        };

        auto instrR1 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[2] << result[3] << result[1] << instr.shamt << instr.functionI;
            return (ss.str());
        };

        auto instrR2 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[2] << instr.rt << instr.rd << instr.shamt << instr.functionI;
            return (ss.str());
        };

        auto instrR3 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[2] << result[3] << instr.rd << instr.shamt << instr.functionI;
            return (ss.str());
        };

        auto instrR4 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << result[2] << instr.rt << result[1] << instr.shamt << instr.functionI;
            return (ss.str());
        };

        auto instrR5 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << instr.rs << instr.rt << result[1] << instr.shamt << instr.functionI;
            return (ss.str());
        };

        auto instrR6 = [](Instructions instr, vector<string> &result) -> string{
            std::stringstream ss;
            ss << instr.opcode << instr.rs << result[3] << result[1] << result[2] << instr.functionI;
            return (ss.str());
        };

        //map que associa a string pesquisada a funcao lambda de conversao
        map<string, function<string(Instructions, vector<string>)>> instrMIPS;
        //instrMIPS.insert({"lui", instrI1(LUI, result)});
        /*instrMIPS["bltz"] = BLTZ;
        instrMIPS["blez"] = BLEZ;
        instrMIPS["bgtz"] = BGTZ;
        instrMIPS["bgez"] = BGEZ;
        instrMIPS["bltzal"] = BLTZAL;
        instrMIPS["bgezal"] = BGEZAL;
        instrMIPS["beq"] = BEQ;
        instrMIPS["bne"] = BNE;
        instrMIPS["addi"] = ADDI;
        instrMIPS["addiu"] = ADDIU;
        instrMIPS["slti"] = SLTI;
        instrMIPS["sltiu"] = SLTIU;
        instrMIPS["andi"] = ANDI;
        instrMIPS["ori"] = ORI;
        instrMIPS["xori"] = XORI;
        instrMIPS["lb"] = LB;
        instrMIPS["lh"] = LH;
        instrMIPS["lwl"] = LWL;
        instrMIPS["lw"] = LW;
        instrMIPS["lbu"] = LBU;
        instrMIPS["lhu"] = LHU;
        instrMIPS["lwr"] = LWR;
        instrMIPS["sb"] = SB;
        instrMIPS["sh"] = SH;
        instrMIPS["swl"] = SWL;
        instrMIPS["sw"] = SW;
        instrMIPS["swr"] = SWR;
        instrMIPS["j"] = J;
        instrMIPS["jal"] = JAL;
        instrMIPS["sllv"] = SLLV;
        instrMIPS["srlv"] = SRLV;
        instrMIPS["srav"] = SRAV;
        instrMIPS["add"] = ADD;
        instrMIPS["addu"] = ADDU;
        instrMIPS["sub"] = SUB;
        instrMIPS["subu"] = SUBU;
        instrMIPS["and"] = AND;
        instrMIPS["or"] = OR;
        instrMIPS["xor"] = XOR;
        instrMIPS["nor"] = NOR;
        instrMIPS["slt"] = SLT;
        instrMIPS["sltu"] = SLTU;
        instrMIPS["jr"] = JR;
        instrMIPS["mthi"] = MTHI;
        instrMIPS["mtlo"] = MTLO;
        instrMIPS["mult"] = MULT;
        instrMIPS["multu"] = MULTU;
        instrMIPS["div"] = DIV;
        instrMIPS["divu"] = DIVU;
        instrMIPS["jalr"] = JALR;
        instrMIPS["mfhi"] = MFHI;
        instrMIPS["mflo"] = MFLO;
        instrMIPS["sll"] = SLL;
        instrMIPS["srl"] = SRL;
        instrMIPS["sra"] = SRA;*/

        map<string, string>rgstrMIPS;
        rgstrMIPS["$a0"] = "00100";
        rgstrMIPS["$a1"] = "00101";
        rgstrMIPS["$a2"] = "00110";
        rgstrMIPS["$a3"] = "00111";
        rgstrMIPS["$at"] = "00001";
        rgstrMIPS["$fp"] = "11110";
        rgstrMIPS["$gp"] = "11100";
        rgstrMIPS["$k0"] = "11010";
        rgstrMIPS["$k1"] = "11011";
        rgstrMIPS["$ra"] = "11111";
        rgstrMIPS["$s0"] = "10000";
        rgstrMIPS["$s1"] = "10001";
        rgstrMIPS["$s2"] = "10010";
        rgstrMIPS["$s3"] = "10011";
        rgstrMIPS["$s4"] = "10100";
        rgstrMIPS["$s5"] = "10101";
        rgstrMIPS["$s6"] = "10110";
        rgstrMIPS["$s7"] = "10111";
        rgstrMIPS["$sp"] = "11101";
        rgstrMIPS["$t0"] = "01000";
        rgstrMIPS["$t1"] = "01001";
        rgstrMIPS["$t2"] = "01010";
        rgstrMIPS["$t3"] = "01011";
        rgstrMIPS["$t4"] = "01100";
        rgstrMIPS["$t5"] = "01101";
        rgstrMIPS["$t6"] = "01110";
        rgstrMIPS["$t7"] = "01111";
        rgstrMIPS["$t8"] = "11000";
        rgstrMIPS["$t9"] = "11001";
        rgstrMIPS["$v0"] = "00010";
        rgstrMIPS["$v1"] = "00011";
        rgstrMIPS["$zero"] = "00000";

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

            } else{

            }
            result.clear();
        }
    }


    void gravarArquivo(string nome, vector<string> &listaInstrucoes){
/*
        ofstream arquivoSaida;

        arquivoSaida.open("programa1_bin.txt"); //, ios::app); parâmetro para abrir o arquivo e posicionar o cursor no final do arquivo.

        arquivoSaida << s << "\n";

        arquivoSaida.close();
*/
    }


int main(int argc, char *argv[]){

    //vector com linhas do arquivo
    vector<string> listaArquivo;

    lerArquivo("teste.txt", listaArquivo);

    vector<string> listaInstrucoes;

    criaInstrucoes(listaArquivo, listaInstrucoes);

    gravarArquivo("", listaInstrucoes);
    //limpa memoria
    listaArquivo.clear();
    listaInstrucoes.clear();

    return 0;
}

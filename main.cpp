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

    string instrI1(Instructions instr, vector<string> &result){
        string instrucao;
        std::stringstream ss;
        ss << instr.opcode << instr.rs << result[1] << toBin16Bits(stoi(result[2]));

        return (instrucao = ss.str());
    }
    /*
        funcao de gravacao do arquivo com os binarios correspondentes das instrucoes

    */

    //cria instrucoes
    void criaInstrucoes(vector<string> &listaArquivo, vector<string> &listaInstrucoes, map<string, Instructions> &mpInstr, map<string, string> &mpRgstr){
        //vetor que recebe as instrucoes das linhas
        vector<string> result;
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


    void gravarArquivo(string nome);


int main(int argc, char *argv[]){

    //instrucoes do MIPS
    Instructions LUI, BLTZ, BLEZ, BGTZ, BGEZ, BLTZAL, BGEZAL, BEQ, BNE, ADDI, ADDIU, SLTI, SLTIU, ANDI, ORI, XORI, LB, LH, LWL, LW, LBU, LHU, LWR, SB, SH, SWL, SW, SWR, J, JAL, SLLV, SRLV, SRAV, ADD, ADDU, SUB, SUBU, AND, OR, XOR, NOR, SLT, SLTU, JR, MTHI, MTLO, MULT, MULTU, DIV, DIVU, JALR, MFHI, MFLO, SLL, SRL, SRA;

    //inicializando instrucoes com valores ja conhecidos
    LUI.ini("001111", "00000", "", "", "", "");
//    BLTZ.ini("000001", "", "00000", "", "", "");

    //Instructions LUI;
    map<string,Instructions>instrMIPS; //map que associa a string
    instrMIPS["lui"] = LUI;
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

    //vector com linhas do arquivo
    vector<string> listaArquivo;

    lerArquivo("teste.txt", linhasArquivo);

    vector<string> listaInstrucoes;

    criaInstrucoes(listaArquivo, listaInstrucoes, instrMIPS, rgstrMIPS);

    return 0;
}
/*    //
    string instrStringToBin(vector<string> &result){
    }



/*    enum registradores{
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

    map<string, Instructions>instrucoes{
        {'', },
        {

        }
    };




    //vector<string>listaInstrucoes;


    ofstream arquivoSaida;

    arquivoSaida.open("programa1_bin.txt"); //, ios::app); parâmetro para abrir o arquivo e posicionar o cursor no final do arquivo.

    arquivoSaida << s << "\n";

    arquivoSaida.close();
*/

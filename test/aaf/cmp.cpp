#include <algorithm>
//
// Created by asib1 on 12.03.2021.
//

#include "../../src/cadical.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <cstring>

using namespace std;

#define DEBUG 0

#define LOG(S) if (DEBUG) cout << S

#define ADD(X) LOG(X); solver->add(X)

#define SPACE LOG(' ')
#define NL LOG('\n')

#define ADD_S(X) ADD(X); SPACE
#define TERM_CL ADD(0); NL

int main(__unused int argc, char *argv[]) {
    auto *solver = new CaDiCaL::Solver;
    solver->set("quiet", 1);

    unordered_map<std::string, int> arg2lit;
    unordered_map<int, std::string> lit2arg;

#define B_SIZE 20

    char b[B_SIZE];

    char ch;
    fstream fin(argv[1], fstream::in);

    int argcount = 0;

    bool args_finished = false;

    size_t *pre_counts;

    unordered_map<int, int *> pre;

    while (fin >> noskipws >> ch) {
        if (ch == 'a') {
            fin >> noskipws >> ch;
            if (ch == 'r') {
                fin >> noskipws >> ch;
                if (ch == 'g') {
                    fin >> noskipws >> ch;
                    if (ch == '(') {
                        fin >> noskipws >> ch;
                        unsigned int counter = 0;
                        while (ch != ')') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        argcount++;
                        std::string s(b, counter);
                        arg2lit[s] = argcount;
                        lit2arg[argcount] = s;
                    }
                }
            } else if (ch == 't') {
                fin >> noskipws >> ch;
                if (ch == 't') {
                    fin >> noskipws >> ch;
                    if (ch == '(') {
                        if (!args_finished) {
                            args_finished = true;
                            pre_counts = new size_t[argcount];
                            memset(pre_counts, 0, argcount);
                            for (int i = 0; i < argcount; i++) {
                                pre[i] = new int[argcount];
                            }
                            LOG("conflictfree\n");
                        }
                        // conflictfree
                        int lit1;
                        int lit2;
                        fin >> noskipws >> ch;
                        unsigned int counter = 0;
                        while (ch != ',') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        lit1 = arg2lit[std::string(b, counter)];
                        ADD_S(-lit1);
                        fin >> noskipws >> ch;
                        counter = 0;
                        while (ch != ')') {
                            if (counter > B_SIZE) exit(1);
                            b[counter] = ch;
                            fin >> noskipws >> ch;
                            counter++;
                        }
                        lit2 = arg2lit[std::string(b, counter)];
                        ADD_S(-lit2);
                        TERM_CL;
                        pre[lit2 - 1][pre_counts[lit2 - 1]] = lit1;
                        pre_counts[lit2 - 1]++;

                    }
                }
            }
        }
    }


    // might be faster to cache the clauses and add them once to the solver at the end of one loop over argcount

#define ATT pre[i][pre_count]
#define ATT_I ATT - 1

    for (int i = 0; i < argcount; i++) {
#       define loop_lit (i + 1)
        // admissible
        for (size_t pre_count = 0; pre_count < pre_counts[i]; pre_count++) {
            LOG("adm ");
            for (size_t pre_pre_count = 0; pre_pre_count < pre_counts[ATT_I]; pre_pre_count++) {
                ADD_S(pre[ATT_I][pre_pre_count]);
            }
            ADD_S(- (loop_lit));
            TERM_CL;
        }

        // completeconstraint
        LOG("cmp ");
        for (size_t pre_count = 0; pre_count < pre_counts[i]; pre_count++) {
            ADD_S(ATT + argcount);
        }
        ADD_S(loop_lit);
        TERM_CL;

        // substitutions
        LOG("sub ");
        for (size_t pre_count = 0; pre_count < pre_counts[i]; pre_count++) {
            ADD_S(ATT);
        }
        ADD_S(loop_lit + argcount);
        TERM_CL;

    }
#if DEBUG
    for (const auto &a : arg2lit) {
        cout << a.first << " " << a.second << '\n';
    }
    cout << argcount << '\n';
#endif

    int sol_buff[argcount];

    int breached_buff[2 * argcount];

    while (solver->solve() == 10) {

        bool breach = false;

        for (int sub = 1 + argcount; sub < 2 * argcount + 1 && !breach; sub++) {
            if (solver->val(sub) > 0) {
# define I sub - argcount - 1
                for (size_t pre_count = 0; pre_count < pre_counts[I]; pre_count++) {
                    if ((breach = solver->val(pre[I][pre_count]) > 0)) break;
                }
            }
        }

        if (breach) {
            for (int lit = 1; lit < 2 * argcount + 1; lit++) {
                breached_buff[lit - 1] = solver->val(lit);
            }
            for (int signed_lit : breached_buff){
                solver->add(-signed_lit);
            }
            solver->add(0);
            continue;
        }

        bool first_out = true;
        for (int lit = 1; lit < argcount + 1; lit++) {
            if ((sol_buff[lit - 1] = solver->val(lit)) > 0) {
                if (first_out) {
                    cout << '"' << lit2arg[lit];
                    first_out = false;
                } else {
                    cout << ' ' << lit2arg[lit];
                }
            }
        }
        if (first_out) cout << '"';
        cout << "\"\n";
        for (int signed_lit : sol_buff) {
            solver->add(-signed_lit);
        }
        solver->add(0);
    }

    delete solver;

    for (const auto &a : pre) {
        delete a.second;
    }

    delete pre_counts;

    return 0;
}

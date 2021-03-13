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
            ADD_S(-(loop_lit));
            TERM_CL;
        }
    }
#if DEBUG
    for (const auto &a : arg2lit) {
        cout << a.first << " " << a.second << '\n';
    }
    cout << argcount << '\n';
#endif

    int sol_buff[argcount];

    while (solver->solve() == 10) {
        // after one execution of this body a single preferred solution has to be found

        int res;
        do {
            // copy solution
            for (int lit = 1; lit < argcount + 1; lit++) {
                sol_buff[lit - 1] = solver->val(lit);
            }
            for (int signed_lit : sol_buff) {
                // block this particular solution
                solver->add(-signed_lit);
                // assume positive part of the solution which means it can only be extended in another solve call
                if (signed_lit > 0) solver->assume(signed_lit);
            }
            solver->add(0);
            res = solver->solve();
            cout << res << '\n';
        } while (res == 10);

        bool first_out = true;
        for (int signed_lit : sol_buff){
            if (signed_lit > 0) {
                if (first_out) {
                    cout << '"' << lit2arg[signed_lit];
                    first_out = false;
                } else {
                    cout << ' ' << lit2arg[signed_lit];
                }
            }
        }
        if (first_out) cout << '"';
        cout << "\"\n";
        for (int signed_lit : sol_buff) {
            if (signed_lit < 0) solver->add(-signed_lit);
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
